#include "ReadDays.h"
#include "EconomyErrors.h"
#include <string>

int ReadDaysCount(std::istream& input, int argc, char* argv[])
{
	std::string daysStr;

	if (argc >= 2)
	{
		daysStr = std::string(argv[1]);
	}
	else if (!(input >> daysStr))
	{
		throw std::invalid_argument("Can't read days count from input");
	}

	int days = StringToDaysCountStrictly(daysStr);
	return days;
}

int StringToDaysCountStrictly(const std::string& str)
{
	size_t pos;
	try
	{
		int value = std::stoi(str, &pos);
		if (pos != str.length())
		{
			throw ParseInputError("Days count parsing error. Provide positive integer as a day count");
		}

		if (value < 0)
		{
			throw ParseInputError("Days count should be positive integer");
		}

		return value;
	}
	catch (std::invalid_argument)
	{
		throw ParseInputError("Days count parsing error. Provide positive integer as a day count");
	}
	catch (std::out_of_range)
	{
		throw ParseInputError("Days count is overflowed. Provide valid positive integer as a day count");
	}
}