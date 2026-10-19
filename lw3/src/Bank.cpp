#include "Bank.h"
#include "EconomyErrors.h"
#include <limits>
#include <string>

Bank::Bank(Money cash)
{
	if (cash < 0)
	{
		throw std::invalid_argument("Can't initialize bank with negative cash sum");
	}
	m_cash = cash;
	m_lastAccountId = 0;
}

void Bank::SendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Can't send negative amount of money");
	}

	if (!m_accountBalances.count(srcAccountId) || !m_accountBalances.count(dstAccountId))
	{
		throw BankOperationError("Source or destination Account ID doesn't exist");
	}

	auto srcIt = m_accountBalances.find(srcAccountId);
	auto dstIt = m_accountBalances.find(dstAccountId);

	if (srcIt->second < amount)
	{
		throw BankOperationError("Can't send more money than src account has");
	}

	srcIt->second -= amount;
	dstIt->second += amount;
}

Money Bank::GetCash() const
{
	return m_cash;
}

Money Bank::GetAccountBalance(AccountId accountId) const
{
	if (!m_accountBalances.count(accountId))
	{
		throw BankOperationError("Account with id " + std::to_string(accountId) + " doesn't exist");
	}

	return m_accountBalances.find(accountId)->second;
}

Money Bank::WithdrawMoney(AccountId account, Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Can't withdraw negative amount of money");
	}

	if (!m_accountBalances.count(account))
	{
		throw BankOperationError("Account doesn't exist");
	}

	auto balanceIt = m_accountBalances.find(account);
	if (balanceIt->second < amount)
	{
		throw BankOperationError("Can't withdraw amount more than balance");
	}

	balanceIt->second -= amount;
	m_cash += amount;

	return amount;
}

void Bank::DepositMoney(AccountId account, Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Can't deposit negative amount of money");
	}

	if (m_cash < amount)
	{
		throw BankOperationError("Can't deposit amount more than total cash");
	}

	if (!m_accountBalances.count(account))
	{
		throw BankOperationError("Account doesn't exist");
	}

	auto balanceIt = m_accountBalances.find(account);

	balanceIt->second += amount;
	m_cash -= amount;
}

AccountId Bank::OpenAccount()
{
	if (m_lastAccountId == std::numeric_limits<AccountId>::max())
	{
		throw BankOperationError("Couldn't open new account. Max account amount has reached");
	}

	m_accountBalances.insert({ ++m_lastAccountId, 0 });

	return m_lastAccountId;
}

Money Bank::CloseAccount(AccountId accountId)
{
	if (!m_accountBalances.count(accountId))
	{
		throw BankOperationError("Can't close unexisting account");
	}

	auto it = m_accountBalances.find(accountId);
	m_cash += it->second;

	Money money = it->second;
	m_accountBalances.erase(it);

	return money;
}

Money Bank::BalancesSum() const
{
	Money total = 0;
	for (auto& [accountId, money] : m_accountBalances)
	{
		total += money;
	}
	return total;
}
