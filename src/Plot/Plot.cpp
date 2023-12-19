#include "Plot.hpp"

#include <unistd.h>

#include <bitset>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "implot.h"

Plot::Plot(std::string name) : name(name)
{
}

void Plot::setName(const std::string& newName)
{
	name = newName;
}

std::string Plot::getName() const
{
	return name;
}

std::string& Plot::getNameVar()
{
	return name;
}

void Plot::setAlias(const std::string& newAlias)
{
	alias = newAlias;
}

std::string Plot::getAlias() const
{
	return alias;
}

bool Plot::addSeries(Variable& var)
{
	std::string name = var.getName();
	seriesMap[name] = std::make_shared<Series>();
	seriesMap[name]->buffer = std::make_unique<ScrollingBuffer<double>>();
	seriesMap[name]->var = &var;
	return true;
}

std::shared_ptr<Plot::Series> Plot::getSeries(const std::string& name)
{
	return seriesMap[name];
}

std::map<std::string, std::shared_ptr<Plot::Series>>& Plot::getSeriesMap()
{
	return seriesMap;
}

ScrollingBuffer<double>& Plot::getTimeSeries()
{
	return time;
}

bool Plot::removeSeries(const std::string& name)
{
	if (seriesMap.find(name) == seriesMap.end())
		return false;

	seriesMap.erase(name);
	return true;
}
bool Plot::removeAllVariables()
{
	for (auto& [name, var] : seriesMap)
	{
		seriesMap[name]->buffer.reset();
		seriesMap[name].reset();
	}
	seriesMap.clear();
	return true;
}

std::vector<uint32_t> Plot::getVariableAddesses() const
{
	std::vector<uint32_t> addresses;
	for (auto& [name, ser] : seriesMap)
		addresses.push_back(ser->var->getAddress());

	return addresses;
}

std::vector<Variable::type> Plot::getVariableTypes() const
{
	std::vector<Variable::type> types;
	for (auto& [name, ser] : seriesMap)
		types.push_back(ser->var->getType());

	return types;
}

bool Plot::addPoint(const std::string& varName, double value)
{
	seriesMap[varName]->var->setValue(value);
	seriesMap[varName]->buffer->addPoint(value);
	return true;
}

bool Plot::addTimePoint(double t)
{
	time.addPoint(t);
	return true;
}

void Plot::erase()
{
	time.erase();

	for (auto& [name, ser] : seriesMap)
		ser->buffer->erase();
}

void Plot::setVisibility(bool state)
{
	visibility = state;
}
bool Plot::getVisibility() const
{
	return visibility;
}
bool& Plot::getVisibilityVar()
{
	return visibility;
}

void Plot::setType(Type newType)
{
	type = newType;
}
Plot::Type Plot::getType() const
{
	return type;
}

void Plot::setDomain(Domain newDomain)
{
	domain = newDomain;
}

Plot::Domain Plot::getDomain() const
{
	return domain;
}

void Plot::setTraceVarType(TraceVarType newTraceVarType)
{
	traceVarType = newTraceVarType;
}

Plot::TraceVarType Plot::getTraceVarType() const
{
	return traceVarType;
}

Plot::displayFormat Plot::getSeriesDisplayFormat(const std::string& name) const
{
	return seriesMap.at(name)->format;
}
void Plot::setSeriesDisplayFormat(const std::string& name, displayFormat format)
{
	seriesMap.at(name)->format = format;
}

std::string Plot::getSeriesValueString(const std::string& name, double value)
{
	Variable::type type = seriesMap.at(name)->var->getType();

	if (type == Variable::type::F32)
		return std::to_string(value);

	switch (seriesMap.at(name)->format)
	{
		case displayFormat::DEC:
			return std::to_string(static_cast<int64_t>(value));
		case displayFormat::HEX:
		{
			std::stringstream ss;
			ss << std::hex << static_cast<int64_t>(value);
			return std::string("0x") + ss.str();
		}
		case displayFormat::BIN:
		{
			switch (type)
			{
				case Variable::type::I8:
				case Variable::type::U8:
				{
					std::bitset<8> binaryValue(value);
					return std::string("0b") + binaryValue.to_string();
				}
				case Variable::type::I16:
				case Variable::type::U16:
				{
					std::bitset<16> binaryValue(value);
					return std::string("0b") + binaryValue.to_string();
				}
				case Variable::type::I32:
				case Variable::type::U32:
				{
					std::bitset<32> binaryValue(value);
					return std::string("0b") + binaryValue.to_string();
				}
				default:
					break;
			}
			break;
		}
		default:
			return std::string("");
	}
	return std::string("");
}

void Plot::setIsHovered(bool isHovered)
{
	this->isHoveredOver = isHovered;
}

bool Plot::isHovered() const
{
	return isHoveredOver;
}
