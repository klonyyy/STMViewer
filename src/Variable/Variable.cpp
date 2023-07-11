#include "Variable.hpp"

#include <limits>

Variable::Variable(std::string name)
{
	this->name = name;
	value = 0.0;
	address = 0x20000000;
	varType = type::UNKNOWN;
}

Variable::Variable(std::string name, Variable::type type, double value)
{
	this->name = name;
	this->varType = type;
	this->value = value;
}

void Variable::setAddress(uint32_t addr)
{
	address = addr;
}
uint32_t Variable::getAddress() const
{
	return address;
}

bool Variable::getIsFound() const
{
	return isFound;
}

void Variable::setIsFound(bool found)
{
	isFound = found;
}