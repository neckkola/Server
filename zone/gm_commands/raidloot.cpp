#include "../client.h"
#include "../groups.h"
#include "../raids.h"
#include "../raids.h"

void command_raidloot(Client *c, const Seperator *sep)
{
	int arguments = sep->argnum;
	if (!arguments) {
		c->Message(Chat::White, "Usage: #raidloot [All|GroupLeader|RaidLeader|Selected]");
		return;
	}

	auto client_raid = c->GetRaid();
	if (!client_raid) {
		c->Message(Chat::White, "You must be in a Raid to use this command.");
		return;
	}

	if (!client_raid->IsLeader(c)) {
		c->Message(Chat::White, "You must be the Raid Leader to use this command.");
		return;
	}

	std::string raid_loot_type  = Strings::ToLower(sep->arg[1]);
	bool        is_all          = raid_loot_type.find("all") != std::string::npos;
	bool        is_group_leader = raid_loot_type.find("groupleader") != std::string::npos;
	bool        is_raid_leader  = raid_loot_type.find("raidleader") != std::string::npos;
	bool        is_selected     = raid_loot_type.find("selected") != std::string::npos;
	if (
		!is_all &&
		!is_group_leader &&
		!is_raid_leader &&
		!is_selected
		) {
		c->Message(Chat::White, "Usage: #raidloot [All|GroupLeader|RaidLeader|Selected]");
		return;
	}

	std::map<uint32, std::string> loot_types = {
		{RaidLootTypes::All,         "All"},
		{RaidLootTypes::GroupLeader, "GroupLeader"},
		{RaidLootTypes::RaidLeader,  "RaidLeader"},
		{RaidLootTypes::Selected,    "Selected"}
	};

	uint32 loot_type;
	if (is_all) {
		loot_type = RaidLootTypes::All;
	}
	else if (is_group_leader) {
		loot_type = RaidLootTypes::GroupLeader;
	}
	else if (is_raid_leader) {
		loot_type = RaidLootTypes::RaidLeader;
	}
	else if (is_selected) {
		loot_type = RaidLootTypes::Selected;
	}

	c->Message(
		Chat::White,
		fmt::format(
			"Loot type changed to {} ({}).",
			loot_types[loot_type],
			loot_type
		).c_str()
	);
}
	void command_test(Client* c, const Seperator* sep)
	{

		struct RaidNotes_Struct
		{
			uint32	action;				//000
			char	LeaderName[64];		//004 - Rola
			uint32	unk3;
			char	PlayerName[64];		//072 - Rola
			uint32	unk1;
			char	Notes[64];			//140 - Notes
			uint32	unk2;

		};
		int arguments = sep->argnum;
		if (!arguments) {
			c->Message(Chat::White, "Usage: #test 1 LeaderName Notes");
			return;
		}

		auto raid = c->GetRaid();
		if (!raid) {
			c->Message(Chat::White, "You must be in a Raid to use this command.");
			return;
		}

		uint32 item = atoi(sep->arg[1]);

		switch (item) {
		case 1: {
			char LeaderName[64];
			char PlayerName[64];
			char Notes[64];
			memset(LeaderName, 0, 64);
			memset(PlayerName, 0, 64);
			memset(Notes, 0, 64);

			auto outapp = new EQApplicationPacket(OP_RaidTest, sizeof(RaidNotes_Struct));
			RaidNotes_Struct* rg = (RaidNotes_Struct*)outapp->pBuffer;
			rg->action = raidSetNote;
			strcpy(rg->LeaderName, sep->arg[2]);
			strcpy(rg->PlayerName, sep->arg[2]);
			rg->unk1 = 0xFFFFFFFF;
			strcpy(rg->Notes, sep->arg[3]);
			rg->unk2 = 0xAAAAAAAA;
			rg->unk3 = 0xbbbbbbbb;

	//		c->QueuePacket(outapp, false);
			c->FastQueuePacket(&outapp);
			safe_delete(outapp);
			break;
		}
		case 2: {
			char leadername[64];
			memset(leadername, 0, 64);
			strncpy(leadername, sep->arg[2], sizeof(sep->arg[2]));
			uint32 gid = atoi(sep->arg[3]);
			RaidLeadershipAA_Struct raid_aa;
			GroupLeadershipAA_Struct group_aa[MAX_RAID_GROUPS];
			Client* rl = raid->GetLeader();
			rl->GetRaidAAs(&raid_aa);
			Client* gl = raid->GetGroupLeader(0);
			gl->GetGroupAAs(&group_aa[0]);


			auto outapp = new EQApplicationPacket(OP_RaidUpdate, sizeof(RaidLeadershipUpdate_Struct));
			RaidLeadershipUpdate_Struct* rlaa = (RaidLeadershipUpdate_Struct*)outapp->pBuffer;
			rlaa->action = raidSetLeaderAbilities;
			strn0cpy(rlaa->leader_name, c->GetName(), 64);
			strn0cpy(rlaa->player_name, c->GetName(), 64);
			if (gid != RAID_GROUPLESS)
				memcpy(&rlaa->group, &group_aa[gid], sizeof(GroupLeadershipAA_Struct));
			memcpy(&rlaa->raid, &raid_aa, sizeof(RaidLeadershipAA_Struct));
			c->QueuePacket(outapp);
			safe_delete(outapp);
			break;
		}
		case 3: {

			auto outapp = new EQApplicationPacket(OP_RaidUpdate, sizeof(RaidGeneral_Struct));
			RaidGeneral_Struct* rg = (RaidGeneral_Struct*)outapp->pBuffer;
			rg->action = raidLock;
			strn0cpy(rg->leader_name, "Rola", 64);
			strn0cpy(rg->player_name, "Rola", 64);
			c->QueuePacket(outapp);
			safe_delete(outapp);
			break;

		}
		}

	}
