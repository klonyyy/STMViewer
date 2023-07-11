#include "ElfReader.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#if defined(unix) || defined(__unix__) || defined(__unix)
#define _UNIX
#endif

ElfReader::ElfReader(std::string& filename, std::shared_ptr<spdlog::logger> logger) : elfname(filename), logger(logger)
{
}

bool ElfReader::updateVariableMap(std::map<std::string, std::shared_ptr<Variable>>& vars)
{
	if (elfname.empty())
		return false;

	std::string startCmd = std::string("gdb -batch -ex \"set trace-commands on\" -ex ") + "\"file " + elfname + "\" ";
	std::string cmdFull = startCmd;
	std::string out = "";

	for (auto& [name, var] : vars)
	{
		var->setIsFound(false);
		cmdFull += (std::string("-ex ") + "\"p /d &" + name + "\" ");
		cmdFull += (std::string("-ex ") + "\"ptype " + name + "\" ");

		if (cmdFull.size() > maxGdbCmdLendth - (3 * maxNameLength))
		{
			out += executeCommand(cmdFull.c_str());
			cmdFull = startCmd;
			logger->info("Dividing command into smaller chunks...");
		}
	}
	out += executeCommand(cmdFull.c_str());

	std::string delimiter = "+p /d &";
	int32_t start = 0;
	/* get rid of file and other start commands */
	out.erase(0, out.find(delimiter));

	while (out.length() > 0 && (start = out.find(delimiter)) != -1)
	{
		std::string::size_type end = out.find(delimiter, start + 1);
		if (end == std::string::npos)
			end = out.length();

		std::string temp = out.substr(start, end - start);

		std::string::size_type addrPos = temp.find('$', 0);
		std::string::size_type typePos = temp.find("type = ", 0);

		if (addrPos < end && typePos < end && addrPos > 0 && typePos > 0)
		{
			std::string varName = temp.substr(delimiter.length(), temp.find('$', 0) - delimiter.length() - 1);
			vars.at(varName)->setIsFound(true);
			uint8_t offset = temp.find(" = ", addrPos) + 2;
			vars.at(varName)->setAddress(atoi(temp.substr(offset, temp.find('\n', addrPos)).c_str()));
			std::string type = temp.substr(typePos + 7, temp.find('\n', typePos));
			vars.at(varName)->setType(getTypeFromString(type));
			logger->info("NAME: {}", vars.at(varName)->getName());
			logger->info("ADDRESS: {}", vars.at(varName)->getAddress());
			logger->info("TYPE: {}", static_cast<uint32_t>(vars[varName]->getType()));
		}
		out.erase(0, temp.length());
	}
	return true;
}

VariableBase::type ElfReader::getTypeFromString(const std::string& strType)
{
	const std::vector<std::pair<std::string, VariableBase::type>> typeVector = {
		{"unsigned 8-bit", VariableBase::type::U8},
		{"unsigned char", VariableBase::type::U8},
		{"bool", VariableBase::type::U8},
		{"signed 8-bit", VariableBase::type::I8},
		{"signed char", VariableBase::type::I8},
		{"unsigned 16-bit", VariableBase::type::U16},
		{"unsigned short", VariableBase::type::U16},
		{"signed 16-bit", VariableBase::type::I16},
		{"signed short", VariableBase::type::I16},
		{"unsigned 32-bit", VariableBase::type::U32},
		{"unsigned int", VariableBase::type::U32},
		{"unsigned long", VariableBase::type::U32},
		{"volatile int", VariableBase::type::I32},
		{"signed 32-bit", VariableBase::type::I32},
		{"signed int", VariableBase::type::I32},
		{"signed long", VariableBase::type::I32},
		{"float", VariableBase::type::F32},
		{"short", VariableBase::type::I16},
		{"long", VariableBase::type::I32}};

	for (auto entry : typeVector)
	{
		if (strType.find(entry.first) != std::string::npos)
			return entry.second;
	}
	return VariableBase::type::UNKNOWN;
}

std::string ElfReader::executeCommand(const char* cmd)
{
	std::array<char, 128> buffer;
	std::string result;

#ifdef _UNIX
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
#elif _WIN32
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
#else
#error "Your system is not supported!"
#endif

	if (!pipe)
		throw std::runtime_error("popen() failed!");
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
		result += buffer.data();
	return result;
}