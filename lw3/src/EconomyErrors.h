#pragma once

#include <stdexcept>

class BankOperationError : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

class EconomyMemberActionError : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

class CitySimulationError : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};

class ParseInputError : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};