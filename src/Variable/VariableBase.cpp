#include "VariableBase.hpp"

#include <limits>

#include "Variable.hpp"

VariableBase::VariableBase(std::string name) : name(name)
{
	value = 0.0;
	varType = type::UNKNOWN;
}

VariableBase::VariableBase(std::string name, VariableBase::type type, double value) : name(name), varType(type), value(value)
{
}

void VariableBase::setType(type varType)
{
	this->varType = varType;
}

VariableBase::type VariableBase::getType() const
{
	return varType;
}

std::string VariableBase::getTypeStr() const
{
	return std::string(types[static_cast<uint8_t>(varType)]);
}

void VariableBase::setValue(double val)
{
	value = val;
}

double VariableBase::getValue() const
{
	return value;
}

std::string& VariableBase::getName()
{
	return name;
}

void VariableBase::setName(const std::string& name_)
{
	name = name_;
}

void VariableBase::setColor(float r, float g, float b, float a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void VariableBase::setColor(uint32_t AaBbGgRr)
{
	using u8 = std::numeric_limits<uint8_t>;
	color.r = static_cast<float>((AaBbGgRr & 0x000000ff) / static_cast<float>(u8::max()));
	color.g = static_cast<float>(((AaBbGgRr & 0x0000ff00) >> 8) / static_cast<float>(u8::max()));
	color.b = static_cast<float>(((AaBbGgRr & 0x00ff0000) >> 16) / static_cast<float>(u8::max()));
	color.a = static_cast<float>(((AaBbGgRr & 0xff000000) >> 24) / static_cast<float>(u8::max()));
}

VariableBase::Color& VariableBase::getColor()
{
	return color;
}

uint32_t VariableBase::getColorU32() const
{
	using u8 = std::numeric_limits<uint8_t>;
	uint32_t a = u8::max() * color.a;
	uint32_t r = u8::max() * color.r;
	uint32_t g = u8::max() * color.g;
	uint32_t b = u8::max() * color.b;

	return static_cast<uint32_t>((a << 24) | (b << 16) | (g << 8) | r);
}
