#pragma once

#include "Economy.h"
#include "Bank.h"

#include <functional>
#include <optional>
#include <string>

class EconomyMember
{
public:
	EconomyMember(std::string name, Money cash, Action action);

	std::string GetName() const;
	std::optional<AccountId> GetAccountId() const;
	Money GetCash() const;

	void Withdraw(Bank& bank, Money amount);
	void DepositCash(Bank& bank, Money amount);
	void GiveCash(EconomyMember& reciever, Money money);
	void RecieveCash(Money cash);
	void SetAccountId(AccountId accountId);
	void SetAction(Action action);

	void DoAction(MembersMap& members, Bank& bank, int day);

private:
	std::string m_name;
	std::optional<AccountId> m_accountId;
	Money m_cash;
	Action m_action;
};