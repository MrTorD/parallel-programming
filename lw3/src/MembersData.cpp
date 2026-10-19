#include "MembersData.h"
#include "ActionsData.h"

std::vector<MemberData> MEMBERS_DATA = {
	{ MemberKey::Homer, "Homer Simpson", HOMER_ACTION },
	{ MemberKey::Marge, "Marge Simpson", MARGE_ACTION },
	{ MemberKey::Bart, "Bart Simpson", BART_ACTION },
	{ MemberKey::Lisa, "Lisa Simpson", LISA_ACTION },
	{ MemberKey::Apu, "Apu Nahasapeemapetilon", APU_ACTION },
	{ MemberKey::Burns, "Charles Burns", BURNS_ACTION },
};

std::set<MemberKey> MEMBERS_KEYS_WITHOUT_ACCOUNT = { MemberKey::Bart, MemberKey::Lisa };
