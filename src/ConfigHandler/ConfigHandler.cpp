#include "ConfigHandler.hpp"

ConfigHandler::ConfigHandler(std::string configFilePath, PlotHandler* plotHandler) : configFilePath(configFilePath), plotHandler(plotHandler)
{
	ini = std::make_unique<mINI::INIStructure>();
	file = std::make_unique<mINI::INIFile>(configFilePath);
}

bool ConfigHandler::changeConfigFile(std::string newConfigFilePath)
{
	configFilePath = newConfigFilePath;
	file.reset();
	file = std::make_unique<mINI::INIFile>(configFilePath);
	return true;
}
std::string ConfigHandler::getElfFilePath()
{
	return std::string(ini->get("elf").get("file_path"));
}

bool ConfigHandler::readConfigFile(std::vector<Variable>& vars, std::string& elfPath)
{
	if (!file->read(*ini))
		return false;

	uint32_t varId = 0;
	Variable newVar("xxx");

	elfPath = ini->get("elf").get("file_path");

	while (!newVar.getName().empty())
	{
		newVar.setName(ini->get(std::string("var" + std::to_string(varId))).get("name"));
		newVar.setAddress(atoi(ini->get(std::string("var" + std::to_string(varId))).get("address").c_str()));
		newVar.setType(static_cast<Variable::type>(atoi(ini->get(std::string("var" + std::to_string(varId))).get("type").c_str())));

		varId++;

		if (!newVar.getName().empty())
		{
			vars.push_back(newVar);
			std::cout << "ADDING VARIABLE: " << newVar.getName() << std::endl;
		}
	}

	std::string plotName("xxx");
	uint32_t plotNumber = 0;

	while (!plotName.empty())
	{
		std::string sectionName("plot" + std::to_string(plotNumber++));
		plotName = ini->get(sectionName).get("name");

		if (!plotName.empty())
		{
			uint32_t plotId = plotHandler->addPlot(plotName);

			std::cout << "ADDING PLOT: " << plotName << std::endl;

			uint32_t seriesNumber = 0;

			for (auto& var : vars)
			{
				std::string varIdStr = ini->get(sectionName).get(std::string("series" + std::to_string(seriesNumber++)));

				if (!varIdStr.empty())
				{
					uint32_t varId = atoi(varIdStr.c_str());
					plotHandler->getPlot(plotId)->addSeries(vars[varId]);
					std::cout << "ADDING SERIES: " << vars[varId].getName() << std::endl;
				}
			}
		}
	}

	return true;
}