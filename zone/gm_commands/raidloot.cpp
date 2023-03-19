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

		auto outapp = new EQApplicationPacket(OP_RaidUpdate, sizeof(RaidNotes_Struct) + 64);
		RaidNotes_Struct* rg = (RaidNotes_Struct*)outapp->pBuffer;
		rg->action = raidSetNote;
		strcpy(rg->LeaderName, sep->arg[2]);
		strcpy(rg->PlayerName, sep->arg[2]);
		rg->unk1 = 0xFFFFFFFF;
		strcpy(rg->Notes, sep->arg[3]);

		c->QueuePacket(outapp, false);
		safe_delete(outapp);
		break;
	}
	case 2: {
	
		struct raidtesting {
			uint32 b1;
			uint32 b2;
			uint32 b3;
			uint32 b4;
			uint32 b5;
			uint32 b6;
			uint32 b7;
			uint32 b8;
			uint32 b9;
			uint32 b10;
			uint32 b11;
			uint32 b12;
			uint32 b13;
			uint32 b14;
			uint32 b15;
			uint32 b16;
			uint32 b17;
			uint32 b18;
			uint32 b19;
			uint32 b20;
			uint32 b21;
			uint32 b22;
			uint32 b23;
		};
		auto outapp = new EQApplicationPacket(OP_RaidAMW, sizeof(raidtesting));
		raidtesting* rg = (raidtesting*)outapp->pBuffer;
		rg->b1 = atoi(sep->arg[2]);		rg->b2 = 5;		rg->b3 = 1;  rg->b4 = 0;
		rg->b5 = atoi(sep->arg[6]);		rg->b6 = 0;		rg->b7 = 0;  rg->b8 = 0;
		rg->b9 = atoi(sep->arg[10]);		rg->b10 = 0;	rg->b11 = 0; rg->b12 = 0;
		rg->b13 = atoi(sep->arg[14]);	rg->b14 = 0;	rg->b15 = 0; rg->b16 = 0;
		rg->b17 = atoi(sep->arg[18]);	rg->b18 = 0;	rg->b19 = 0; rg->b20 = 0;
		rg->b21 = atoi(sep->arg[22]);	rg->b22 = 0;	rg->b23 = 0;

		c->QueuePacket(outapp, false);
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
