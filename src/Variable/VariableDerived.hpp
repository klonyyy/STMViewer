#ifndef __VARIABLE_HPP
#define __VARIABLE_HPP

#include "VariableBase.hpp"

class VariableDerived : public VariableBase
{
   public:
	VariableDerived(std::string name);
	VariableDerived(std::string name, type type, double value);
};

#endif