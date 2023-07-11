#include "VariableDerived.hpp"

#include <limits>

VariableDerived::VariableDerived(std::string name)
{
	this->name = name;
	value = 0.0;
	varType = type::D64;
}

VariableDerived::VariableDerived(std::string name, VariableBase::type type, double value)
{
	this->name = name;
	this->value = value;
	this->varType = type;
}
