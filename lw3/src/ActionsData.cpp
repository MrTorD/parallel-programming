#include "ActionsData.h"
#include "MembersData.h"

#include "EconomyErrors.h"
#include <iostream>
#include <stdexcept>

Action HOMER_ACTION = [](MembersMap& members, Bank& bank, int) {
	EconomyMember& homer = members.at(MemberKey::Homer);
	EconomyMember& marge = members.at(MemberKey::Marge);
	EconomyMember& lisa = members.at(MemberKey::Lisa);
	EconomyMember& bart = members.at(MemberKey::Bart);
	EconomyMember& burns = members.at(MemberKey::Burns);

	Money forMarge = 30;
	Money forBart = 10;
	Money forLisa = 10;
	Money forBurns = 20;

	auto totalSpends = forMarge + forBart + forLisa + forBurns;

	auto cash = homer.GetCash();
	auto id = homer.GetAccountId();
	if (!id.has_value())
	{
		throw CitySimulationError("Homer's account doesn't exist. Check if initalization was done properly");
	}

	auto balance = bank.GetAccountBalance(id.value()); 

	if (cash + balance < totalSpends)
	{
		std::cout << "Homer's skipping move\n";
		return;
	}

	auto margeId = marge.GetAccountId();
	auto burnsId = burns.GetAccountId();

	if (!margeId.has_value() || !burnsId.has_value())
	{
		throw CitySimulationError("Marge's or Burn's account doesn't exist. Check if initalization was done properly");
	}

	if (balance < forMarge + forBurns)
	{
		auto deposit = forMarge + forBurns - balance;
		std::cout << "Homer's deposing: " << deposit << "\n";
		homer.DepositCash(bank, deposit);
	}

	std::cout << "Homer's doing transaction to Marge: " << forMarge << "\n";
	bank.SendMoney(id.value(), margeId.value(), forMarge);
	std::cout << "Homer's doing transaction to Burns: " << forMarge << "\n";
	bank.SendMoney(id.value(), burnsId.value(), forBurns);

	if (cash < forLisa + forBart)
	{
		homer.Withdraw(bank, forLisa + forBart - cash);
		std::cout << "Homer's withdrawing: " << forLisa + forBart - cash << "\n";
		homer.GiveCash(bart, forBart);
		std::cout << "Homer's giving cash to Bart: " << forBart << "\n";
		homer.GiveCash(lisa, forLisa);
		std::cout << "Homer's giving cash to Lisa: " << forLisa << "\n";
	}
};

Action MARGE_ACTION = [](MembersMap& members, Bank& bank, int) {
	EconomyMember& marge = members.at(MemberKey::Marge);
	EconomyMember& apu = members.at(MemberKey::Apu);

	Money forApu = 15;

	auto id = marge.GetAccountId();
	auto apuId = apu.GetAccountId();
	if (!id.has_value() || !apuId.has_value())
	{
		throw CitySimulationError("Marge's or Apu's account doesn't exist. Check if initalization was done properly");
	}

	auto cash = marge.GetCash();
	auto balance = bank.GetAccountBalance(id.value());

	if (cash + balance < forApu)
	{
		std::cout << "Marge's skipping move\n";
		return;
	}

	if (balance < forApu)
	{
		auto deposit = forApu - balance;
		std::cout << "Marge's depositing " << deposit << "\n";
		marge.DepositCash(bank, deposit);
	}

	bank.SendMoney(id.value(), apuId.value(), forApu);
	std::cout << "Marge's doing transaction to Apu: " << forApu << "\n";
};

Action BART_ACTION = [](MembersMap& members, Bank&, int) {
	EconomyMember& bart = members.at(MemberKey::Bart);
	EconomyMember& apu = members.at(MemberKey::Apu);

	Money forApu = 15;

	auto cash = bart.GetCash();
	if (cash < forApu)
	{
		std::cout << "Bart's skipping move\n";
		return;
	}

	bart.GiveCash(apu, forApu);
	std::cout << "Bart's giving cash to Apu" << forApu << "\n";
};

Action LISA_ACTION = [](MembersMap& members, Bank&, int) {
	EconomyMember& lisa = members.at(MemberKey::Lisa);
	EconomyMember& apu = members.at(MemberKey::Apu);

	Money forApu = 15;

	auto cash = lisa.GetCash();
	if (cash < forApu)
	{
		std::cout << "Lisa's skipping move\n";
		return;
	}

	lisa.GiveCash(apu, forApu);
	std::cout << "Lisa's giving cash to Apu: " << forApu << "\n";
};

Action APU_ACTION = [](MembersMap& members, Bank& bank, int) {
	EconomyMember& apu = members.at(MemberKey::Apu);
	EconomyMember& burns = members.at(MemberKey::Burns);

	Money forBurns = 200;

	auto id = apu.GetAccountId();
	auto burnsId = burns.GetAccountId();

	if (!id.has_value() || !burnsId.has_value())
	{
		throw CitySimulationError("Burns' or Apu's account doesn't exist. Check if initalization was done properly");
	}

	auto cash = apu.GetCash();
	if (cash > 0)
	{
		std::cout << "Apu's deposting: " << cash << "\n";
		apu.DepositCash(bank, cash);
	}

	auto balance = bank.GetAccountBalance(id.value());

	if (balance < forBurns)
	{
		std::cout << "Apu's skipping move\n";
		return;
	}

	std::cout << "Apu's sending money to Burns: " << forBurns << "\n";
	bank.SendMoney(id.value(), burnsId.value(), forBurns);
};

Action BURNS_ACTION = [](MembersMap& members, Bank& bank, int) {
	EconomyMember& burns = members.at(MemberKey::Burns);
	EconomyMember& homer = members.at(MemberKey::Homer);
	EconomyMember& waylon = members.at(MemberKey::Waylon);

	static auto lastWaylonId = waylon.GetAccountId();

	Money forHomer = 50;
	Money forWaylon = 5;

	auto totalSpends = forHomer + forWaylon;

	auto id = burns.GetAccountId();
	auto homerId = homer.GetAccountId();

	if (!id.has_value() || !homerId.has_value() || !lastWaylonId.has_value())
	{
		throw CitySimulationError("Burns', Homer's or Waylon's account doesn't exist. Check if initalization was done properly");
	}

	auto cash = burns.GetCash();
	auto balance = bank.GetAccountBalance(id.value());

	if (cash + balance < totalSpends)
	{
		std::cout << "Burns's skipping move\n";
		return;
	}

	if (balance < totalSpends)
	{
		auto deposit = totalSpends - balance;
		std::cout << "Burns's deposting: " << deposit << "\n";
		burns.DepositCash(bank, deposit);
	}

	std::cout << "Burns' sending money to Homer: " << forHomer << "\n";
	bank.SendMoney(id.value(), homerId.value(), forHomer);
	try
	{
		std::cout << "Burns' trying to send money to Waylon: " << forWaylon << "\n";
		bank.SendMoney(id.value(), lastWaylonId.value(), forWaylon);
		std::cout << "Burn's transaction is succeed\n";
	}
	catch (BankOperationError)
	{
		std::cout << "Burns' transaction had failed. He's updating info about Waylon's id\n";
		lastWaylonId = waylon.GetAccountId();
	}
};