#pragma once

#include "Economy.h"
#include <set>

struct MemberData
{
    MemberKey key;
    std::string name;
    Action action;
};

extern std::vector<MemberData> MEMBERS_DATA;
extern std::set<MemberKey> MEMBERS_KEYS_WITHOUT_ACCOUNT;