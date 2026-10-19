#include "City.h"
#include "EconomyErrors.h"
#include "MembersData.h"

#include <iostream>

City::City(const std::vector<MemberData>& MembersData, const std::set<MemberKey>& membersWithoutAccount, Money cashPerPerson)
	: m_members(CreateMembers(MembersData, cashPerPerson))
	, m_bank(CountCash())
{
	m_startCapital = m_bank.GetCash();
	OpenAccounts(membersWithoutAccount);
}

std::ostream& City::StartSimulation(std::ostream& output, int duration)
{
	for (m_currDay = 0; m_currDay < duration; m_currDay++)
	{
		try
		{
			SimulateMove(output);
		}
		catch (const EconomyMemberActionError& e)
		{
			throw CitySimulationError(e.what());
		}
		catch (const BankOperationError& e)
		{
			throw CitySimulationError(e.what());
		}
		catch (const std::out_of_range& e)
		{
			throw CitySimulationError(e.what());
		}
	}
	return output;
}

std::ostream& City::PrintSimulationResult(std::ostream& output) const
{
	output << "\nSIMULATION RESULT:\n";

	auto bankCash = m_bank.GetCash();
	output << "Total cash owned by people: " << CountCash() << "\n";
	output << "Total cash registered in bank: " << bankCash << "\n";

	auto balancesSum = m_bank.BalancesSum();
	output << "Balances sum: " << balancesSum << "\n";
	output << "Total money: " << bankCash + balancesSum << "\n";
	output << "Start capital:  " << m_startCapital << "\n";
	return output;
}

MembersMap City::CreateMembers(const std::vector<MemberData>& membersData, Money cashPerPerson)
{
	MembersMap members;

	for (auto data : membersData)
	{
		members.insert({ data.key, EconomyMember(data.name, cashPerPerson, data.action) });
	}

	return members;
}

Money City::CountCash() const
{
	Money cash = 0;
	for (const auto& [name, member] : m_members)
	{
		cash += member.GetCash();
	}
	return cash;
}

void City::OpenAccounts(const std::set<MemberKey>& excludedMembers)
{
	for (auto& [name, member] : m_members)
	{
		if (excludedMembers.count(name))
		{
			continue;
		}

		auto id = m_bank.OpenAccount();
		member.SetAccountId(id);
	}
}

std::ostream& City::PrintMemberMoney(std::ostream& output, const EconomyMember& member)
{
	auto cash = member.GetCash();
	output << "Cash: " << cash << "\n";

	auto id = member.GetAccountId();
	if (id.has_value())
	{
		auto balance = m_bank.GetAccountBalance(id.value());
		output << "Balance: " << balance << "\n";
		output << "Total: " << balance + cash << "\n";
	}
	else
	{
		output << "Balance: Doesn't have account\n";
		output << "Total: " << cash << "\n";
	}

	return output;
}

std::ostream& City::SimulateMove(std::ostream& output)
{
	for (auto& [name, member] : m_members)
	{
		output << '\n'
			   << member.GetName() << " before move: \n";
		PrintMemberMoney(output, member);

		member.DoAction(m_members, m_bank, m_currDay);

		output << member.GetName() << " after move: \n";
		PrintMemberMoney(output, member);
	}
	return output;
}
