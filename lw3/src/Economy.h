#pragma once

#include <functional>
#include <string>

class EconomyMember;
class Bank;

enum class MemberKey
{
	Homer,
	Marge,
	Bart,
	Lisa,
	Apu,
	Burns,
	Nelson,
	Turley,
	Waylon
};

using AccountId = unsigned long long;
using Money = long long;
using MembersMap = std::unordered_map<MemberKey, EconomyMember>;
using Action = std::function<void(MembersMap&, Bank&, int currentDay)>;