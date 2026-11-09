#pragma once

#include <iostream>

int ReadDaysCount(std::istream& input, int argc, char* argv[]);
int StringToDaysCountStrictly(const std::string& str);
bool ReadIsParallel(std::istream& input)
{
	std::string str;
	if (input >> str)
	{
		throw std::invalid_argument("Can't read isParallel flag from input");
	}

	return str == "IsParallel";
}
