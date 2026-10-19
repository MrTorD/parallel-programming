#pragma once

#include "EconomyMember.h"
#include "MembersData.h"
#include <set>

class City
{
public:
	City(const std::vector<MemberData>& membersData, const std::set<MemberKey>& membersWithoutAcc, Money cashPerPerson);

	std::ostream& StartSimulation(std::ostream& output, int duration);
	std::ostream& PrintSimulationResult(std::ostream& output) const;

	Money CountCash() const;
private:
	MembersMap CreateMembers(const std::vector<MemberData>& membersData, Money cashPerPerson);

	void OpenAccounts(const std::set<MemberKey>& excludedMembers);
    std::ostream& PrintMemberMoney(std::ostream& output, const EconomyMember& member);

    std::ostream& SimulateMove(std::ostream& output);

	MembersMap m_members;
	Bank m_bank;
	int m_currDay;
    Money m_startCapital;
};