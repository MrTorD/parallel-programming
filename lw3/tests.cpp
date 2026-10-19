#include "EconomyErrors.h"
#include "MembersData.h"
#include "ReadDays.h"
#include "City.h"
#include <gtest/gtest.h>

TEST(Bank, ValidInit)
{
	Money cash = 0;
	Bank bank(cash);
	ASSERT_EQ(bank.GetCash(), cash);
}

TEST(Bank, NegativeCashInit)
{
	Money cash = -1;
	ASSERT_THROW(Bank bank(cash), std::invalid_argument);
}

TEST(Bank, NegativeAmountSendMoney)
{
	Bank bank(0);
	auto id1 = bank.OpenAccount();
	auto id2 = bank.OpenAccount();

	ASSERT_THROW(bank.SendMoney(id1, id2, -1), std::out_of_range);
}

TEST(Bank, MoreThanBalanceSendMoney)
{
	Bank bank(100);
	auto id1 = bank.OpenAccount();
	auto id2 = bank.OpenAccount();
	bank.DepositMoney(id1, 100);

	ASSERT_THROW(bank.SendMoney(id1, id2, 101), BankOperationError);
}

TEST(Bank, NoSrcSendMoney)
{
	Bank bank(100);
	auto id2 = bank.OpenAccount();

	ASSERT_THROW(bank.SendMoney(1, id2, 5), BankOperationError);
}

TEST(Bank, NoDstSendMoney)
{
	Bank bank(100);
	auto id1 = bank.OpenAccount();
	bank.DepositMoney(id1, 100);

	ASSERT_THROW(bank.SendMoney(id1, 2, 200), BankOperationError);
}

TEST(Bank, CorrectSendMoney)
{
	Bank bank(150);
	auto id1 = bank.OpenAccount();
	auto id2 = bank.OpenAccount();
	bank.DepositMoney(id1, 150);

	bank.SendMoney(id1, id2, 50);

	ASSERT_EQ(bank.GetAccountBalance(id1), 100);
	ASSERT_EQ(bank.GetAccountBalance(id2), 50);
}

TEST(Bank, NoAccGetAccountBalance)
{
	Bank bank(100);
	ASSERT_THROW((void)bank.GetAccountBalance(1), BankOperationError);
}

TEST(Bank, CorrectAccGetAccountBalance)
{
	Bank bank(100);
	auto id = bank.OpenAccount();
	Money deposit = 100;
	bank.DepositMoney(id, deposit);

	ASSERT_EQ(bank.GetAccountBalance(id), deposit);
}

TEST(Bank, NegativeAmountWithdrawMoney)
{
	Bank bank(100);
	auto id = bank.OpenAccount();

	ASSERT_THROW(bank.WithdrawMoney(id, -1), std::out_of_range);
}

TEST(Bank, NoAccWithdrawMoney)
{
	Bank bank(100);

	ASSERT_THROW(bank.WithdrawMoney(1, 0), BankOperationError);
}

TEST(Bank, NotEnoughMoneyWithdrawMoney)
{
	Bank bank(100);
	auto id = bank.OpenAccount();
	bank.DepositMoney(id, 100);

	ASSERT_THROW(bank.WithdrawMoney(id, 101), BankOperationError);
}

TEST(Bank, CorrectWithdrawMoney)
{
	Bank bank(150);
	auto id = bank.OpenAccount();
	bank.DepositMoney(id, 100);

	bank.WithdrawMoney(id, 50);

	ASSERT_EQ(bank.GetCash(), 100);
}

TEST(Bank, MoreThanTotalCashDepositMoney)
{
	Bank bank(100);
	auto id = bank.OpenAccount();

	ASSERT_THROW(bank.DepositMoney(id, 101), BankOperationError);
}

TEST(Bank, NegativeDepositMoney)
{
	Bank bank(100);
	auto id = bank.OpenAccount();

	ASSERT_THROW(bank.DepositMoney(id, -1), std::out_of_range);
}

TEST(Bank, NoAccDepositMoney)
{
	Bank bank(100);

	ASSERT_THROW(bank.DepositMoney(1, 50), BankOperationError);
}

TEST(Bank, CorrectDepositMoney)
{
	Bank bank(100);
	auto id = bank.OpenAccount();

	bank.DepositMoney(id, 75);
	ASSERT_EQ(bank.GetCash(), 25);
}

TEST(Bank, OpenAccount)
{
	Bank bank(100);
	auto id1 = bank.OpenAccount();
	auto id2 = bank.OpenAccount();

	ASSERT_EQ(bank.GetAccountBalance(id1), 0);
	ASSERT_EQ(bank.GetAccountBalance(id2), 0);
	ASSERT_NE(id1, id2);
}

TEST(Bank, CloseAccount)
{
	Bank bank(100);
	auto id = bank.OpenAccount();
	bank.DepositMoney(id, 30);

	(void)bank.CloseAccount(id);

	ASSERT_EQ(bank.GetCash(), 100);
}

TEST(Bank, BalancesSum)
{
	Bank bank(100);
	auto id1 = bank.OpenAccount();
	auto id2 = bank.OpenAccount();
	bank.DepositMoney(id1, 30);
	bank.DepositMoney(id1, 60);

	ASSERT_EQ(bank.BalancesSum(), 90);
}

TEST(EconomyMember, NegativeCashInit)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};

	ASSERT_THROW(EconomyMember member(name, -1, action), std::out_of_range);
}

TEST(EconomyMember, CorrectInit)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};

	EconomyMember member(name, 5, action);

	ASSERT_EQ(member.GetCash(), 5);
}

TEST(EconomyMember, GetName)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};

	EconomyMember member(name, 5, action);

	ASSERT_EQ(member.GetName(), name);
}

TEST(EconomyMember, NoAccGetAccountId)
{
	std::string name = "Ivan";

	Action action = [](MembersMap&, Bank&, int) {};

	EconomyMember member(name, 5, action);

	ASSERT_EQ(member.GetAccountId().has_value(), false);
}

TEST(EconomyMember, CorrectAccGetAccountId)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};

	EconomyMember member(name, 5, action);
	member.SetAccountId(1);

	ASSERT_EQ(member.GetAccountId().value(), 1);
}

TEST(EconomyMember, NoAccWithdraw)
{
	Bank bank(5);
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};
	EconomyMember member(name, 5, action);

	ASSERT_THROW(member.Withdraw(bank, 1), EconomyMemberActionError);
}

TEST(EconomyMember, CorrectlyWithdraw)
{
	Bank bank(50);
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};
	EconomyMember member(name, 50, action);
	member.SetAccountId(bank.OpenAccount());

	member.DepositCash(bank, 30);
	member.Withdraw(bank, 15);

	ASSERT_EQ(member.GetCash(), 35);
	ASSERT_EQ(bank.BalancesSum(), 15);
}

TEST(EconomyMember, NegativeAmountRecieveCash)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};
	EconomyMember member(name, 5, action);

	ASSERT_THROW(member.RecieveCash(-1), EconomyMemberActionError);
}

TEST(EconomyMember, CorrectRecieveCash)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};
	EconomyMember member(name, 5, action);

	member.RecieveCash(15);

	ASSERT_EQ(member.GetCash(), 20);
}

TEST(EconomyMember, MoreThanOwnGiveCash)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};
	EconomyMember member1(name, 5, action);
	EconomyMember member2(name, 10, action);

	ASSERT_THROW(member1.GiveCash(member2, 7), EconomyMemberActionError);
}

TEST(EconomyMember, CorrectGiveCash)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};
	EconomyMember member1(name, 5, action);
	EconomyMember member2(name, 10, action);

	member1.GiveCash(member2, 4);

	ASSERT_EQ(member1.GetCash(), 1);
	ASSERT_EQ(member2.GetCash(), 14);
}

TEST(EconomyMember, SetAccountId)
{
	std::string name = "Ivan";
	Action action = [](MembersMap&, Bank&, int) {};
	EconomyMember member(name, 5, action);

	ASSERT_EQ(member.GetAccountId().has_value(), false);
	member.SetAccountId(1);

	ASSERT_EQ(member.GetAccountId().has_value(), true);
	ASSERT_EQ(member.GetAccountId().value(), 1);
}

TEST(City, InitCity)
{
	Money cashPerPerson = 50;
	City city(MEMBERS_DATA, MEMBERS_KEYS_WITHOUT_ACCOUNT, cashPerPerson);

	auto totalCash = city.CountCash();
	ASSERT_EQ(cashPerPerson * MEMBERS_DATA.size(), totalCash);
}

TEST(ReadDaysCount, ReadNegativeDaysCount)
{
	std::stringstream input("-142");

	ASSERT_THROW(ReadDaysCount(input, 1, nullptr), ParseInputError);
}

TEST(ReadDaysCount, InvalidDaysCount)
{
	std::stringstream input("abc15");

	ASSERT_THROW(ReadDaysCount(input, 1, nullptr), ParseInputError);
}

TEST(ReadDaysCount, InvalidNonDigitsDaysCount)
{
	std::stringstream input("15abc");

	ASSERT_THROW(ReadDaysCount(input, 1, nullptr), ParseInputError);
}

TEST(ReadDaysCount, ValidDaysCount)
{
	std::stringstream input("15");

	auto days = ReadDaysCount(input, 1, nullptr);

	ASSERT_EQ(days, 15);
}


int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}