#include "City.h"
#include "EconomyErrors.h"
#include "MembersData.h"
#include "ReadDays.h"
#include <iostream>

const int CASH_PER_PERSON = 100;

int main(int argc, char* argv[])
{
	try
	{
		int days = ReadDaysCount(std::cin, argc, argv);

		City city(MEMBERS_DATA, MEMBERS_KEYS_WITHOUT_ACCOUNT, CASH_PER_PERSON);

		city.StartSimulation(std::cout, days);
		city.PrintSimulationResult(std::cout);
	}
	catch (const ParseInputError& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const CitySimulationError& e)
	{
		std::cout << "Error occured during simulation. Check if you provided valid people actions: \n";
		std::cout << e.what();
	}
}