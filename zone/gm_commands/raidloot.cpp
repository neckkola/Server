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

void command_packet(Client* c, const Seperator* sep)
{
	int arguments = sep->argnum;
	if (!arguments) {
		c->Message(Chat::White, "Usage: #packet - Send a Raid Update Packet to [playername, action, groupnumber, leader_name, player_name, isgroupleader]");
		return;
	}
	const char* playername;
	uint32 action;
	uint32 groupnumber;
	const char* leader_name;
	const char* player_name;
	uint8 isgroupleader;
	uint8 flag1 = 0;
	uint8 flag2 = 0;
	uint8 flag3 = 0;
	uint8 flag4 = 0;
	uint8 flag5 = 0;
	

		playername = sep->arg[1];
		Client* to = entity_list.GetClientByName(playername);
		action = std::stoi(sep->arg[2]);
		groupnumber = std::stoi(sep->arg[3]);
		leader_name = sep->arg[4];
		player_name = sep->arg[5];
		isgroupleader = std::stoi(sep->arg[6]);
		flag1 = std::stoi(sep->arg[7]);
		flag2 = std::stoi(sep->arg[8]);
		flag3 = std::stoi(sep->arg[9]);
		flag4 = std::stoi(sep->arg[10]);
		flag5 = std::stoi(sep->arg[10]);
	
	/*Raid* raid = entity_list.GetRaidByClient(c);
	if (raid) {
		std::vector<RaidMember> members = raid->GetMembers();

		for (const auto& m : members)
		{
			if (arguments == 2) {
				playername = sep->arg[1];
				action = std::stoi(sep->arg[2]);
				groupnumber = m.GroupNumber;
				leader_name = m.membername;
				player_name = m.membername;
				isgroupleader = m.IsGroupLeader;
			}

			if (strcmp(m.membername, playername) == 0)
			{
				auto outapp = new EQApplicationPacket(OP_RaidUpdate, sizeof(RaidAddMember_Struct));
				RaidAddMember_Struct* ram = (RaidAddMember_Struct*)outapp->pBuffer;
				ram->raidGen.action = action;
				ram->raidGen.parameter = groupnumber;
				strcpy(ram->raidGen.leader_name, leader_name);
				strcpy(ram->raidGen.player_name, player_name);
				ram->_class = m._class;
				ram->level = m.level;
				ram->isGroupLeader = isgroupleader;
				ram->flags[0] = flag1;
				ram->flags[1] = flag2;
				ram->flags[2] = flag3;
				ram->flags[3] = flag4;
				ram->flags[4] = flag5;
				raid->QueuePacket(outapp);
				safe_delete(outapp);
				return;

			}
		}
	}
	else {*/
		auto outapp = new EQApplicationPacket(OP_RaidUpdate, sizeof(RaidAddMember_Struct));
		RaidAddMember_Struct* ram = (RaidAddMember_Struct*)outapp->pBuffer;
		ram->raidGen.action = action;
		ram->raidGen.parameter = groupnumber;
		strcpy(ram->raidGen.leader_name, leader_name);
		strcpy(ram->raidGen.player_name, player_name);
		ram->_class = flag1;
		ram->level = flag2;
		ram->isGroupLeader = isgroupleader;
		ram->flags[0] = flag1;
		ram->flags[1] = flag2;
		ram->flags[2] = flag3;
		ram->flags[3] = flag4;
		ram->flags[4] = flag5;
		to->QueuePacket(outapp);
		safe_delete(outapp);
		return;
	//}
}
