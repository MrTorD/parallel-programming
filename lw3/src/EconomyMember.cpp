#include "EconomyMember.h"
#include "EconomyErrors.h"

#include <iostream>
#include <stdexcept>

EconomyMember::EconomyMember(std::string name, Money cash, Action action)
{
	if (cash < 0)
	{
		throw std::out_of_range("Can't create economy member with negative amount of cash");
	}

	m_name = name;
	m_cash = cash;
	m_action = action;
}

std::string EconomyMember::GetName() const
{
	return m_name;
}

std::optional<AccountId> EconomyMember::GetAccountId() const
{
	return m_accountId;
}

Money EconomyMember::GetCash() const
{
	return m_cash;
}

void EconomyMember::Withdraw(Bank& bank, Money amount)
{
	if (!m_accountId.has_value())
	{
		throw EconomyMemberActionError("Can't withdraw money, member doesn't have an account");
	}

	auto cash = bank.WithdrawMoney(m_accountId.value(), amount);
	RecieveCash(cash);
}

void EconomyMember::DepositCash(Bank& bank, Money amount)
{
	if (!m_accountId.has_value())
	{
		throw EconomyMemberActionError("Can't deposit money, member doesn't have an account");
	}

	bank.DepositMoney(m_accountId.value(), amount);
	m_cash -= amount;
}

void EconomyMember::GiveCash(EconomyMember& reciever, Money money)
{
	if (money > m_cash)
	{
		throw EconomyMemberActionError("Can't give more cash than member has");
	}

	m_cash -= money;
	reciever.RecieveCash(money);
}

void EconomyMember::RecieveCash(Money cash)
{
	if (cash < 0)
	{
		throw EconomyMemberActionError("Can't recieve negative amount of cash");
	}

	m_cash += cash;
}

void EconomyMember::SetAccountId(AccountId accountId)
{
	m_accountId = accountId;
}

void EconomyMember::SetAction(Action action)
{
	m_action = action;
}

void EconomyMember::DoAction(MembersMap& members, Bank& bank, int day)
{
	m_action(members, bank, day);
}
