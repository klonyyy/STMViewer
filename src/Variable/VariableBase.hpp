#ifndef _IVARIABLE_HPP
#define _IVARIABLE_HPP

#include <string>

class VariableBase
{
   public:
	enum class type
	{
		UNKNOWN = 0,
		U8 = 1,
		I8 = 2,
		U16 = 3,
		I16 = 4,
		U32 = 5,
		I32 = 6,
		F32 = 7,
		D64 = 8
	};

	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	VariableBase() = default;
	VariableBase(std::string name);
	VariableBase(std::string name, VariableBase::type type, double value);
	virtual ~VariableBase() = default;

	void setType(type varType);
	type getType() const;
	std::string getTypeStr() const;

	void setValue(double val);
	double getValue() const;

	std::string& getName();
	void setName(const std::string& name_);

	void setColor(float r, float g, float b, float a);
	void setColor(uint32_t AaBbGgRr);

	Color& getColor();
	uint32_t getColorU32() const;

   protected:
	static constexpr uint32_t maxVariableNameLength = 100;

	std::string name;
	type varType;
	double value;
	Color color;
	const char* types[9] = {"UNKNOWN",
							"U8",
							"I8",
							"U16",
							"I16",
							"U32",
							"I32",
							"F32",
							"D64"};
};

#endif