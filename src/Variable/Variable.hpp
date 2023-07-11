#ifndef __VARIABLE_HPP
#define __VARIABLE_HPP

#include "VariableBase.hpp"

class Variable : public VariableBase
{
   public:
	Variable(std::string name);
	Variable(std::string name, Variable::type type, double value);

	void setAddress(uint32_t addr);
	uint32_t getAddress() const;

	bool getIsFound() const;
	void setIsFound(bool found);

   private:
	uint32_t address;
	bool isFound = false;
};

#endif