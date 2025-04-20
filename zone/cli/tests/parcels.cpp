#include "../../zone_cli.h"

struct inventory {
	uint32 slot_id;
	uint32 item_id;
	int16  charges;
	uint32 augment_one;
	uint32 augment_two;
	uint32 augment_three;
	uint32 augment_four;
	uint32 augment_five;
	uint32 augment_six;
};

void ZoneCLI::TestParcels(int argc, char **argv, argh::parser &cmd, std::string &description)
{
	if (cmd[{"-h", "--help"}]) {
		return;
	}

	LogSys.SilenceConsoleLogging();

	Zone::Bootup(ZoneID("qrg"), 0, false);
	zone->StopShutdownTimer();

	entity_list.Process();
	entity_list.MobProcess();

	std::cout << "===========================================\n";
	std::cout << "⚙\uFE0F> Running Parcel Tests...\n";
	std::cout << "===========================================\n\n";

	Client *c       = new Client();
	std::vector<inventory> test_inventory{
		{ EQ::invslot::GENERAL_BEGIN, 35034, 1, 0, 0, 0, 0, 0, 0 }, // Trader Satchel
		{ EQ::invbag::GENERAL_BAGS_BEGIN, 4171, 1, 85490, 0, 0, 0, 0, 0 },
		{ EQ::invbag::GENERAL_BAGS_BEGIN + 1, 8517, 50, 0, 0, 0, 0, 0, 0 },
		{ EQ::invbag::GENERAL_BAGS_BEGIN + 2, 8517, 32767, 0, 0, 0, 0, 0, 0 },
		{ EQ::invbag::GENERAL_BAGS_BEGIN + 3, 70208, 1, 0, 0, 0, 0, 0, 0 },
		{ EQ::invbag::GENERAL_BAGS_BEGIN + 4, 14196, 5, 0, 0, 0, 0, 0, 0 },
		{ EQ::invbag::GENERAL_BAGS_BEGIN + 5, 14196, 2, 0, 0, 0, 0, 0, 0 },
		{ EQ::invbag::GENERAL_BAGS_BEGIN + 6, 8205, 0, 0, 0, 0, 0, 0, 0 },
		{ EQ::invbag::GENERAL_BAGS_BEGIN + 7, 5106, 1, 0, 0, 0, 0, 0, 0 },
		{ EQ::invbag::GENERAL_BAGS_BEGIN + 8, 34079, 1, 0, 0, 0, 0, 0, 0 },
	};

	c->SetCharacterID(std::numeric_limits<uint32>::max());
	c->SetName("TestParcelCharacter");

	for (auto const &i:test_inventory) {
		auto inst = database.CreateItem(i.item_id, i.charges, i.augment_one, i.augment_two, i.augment_three, i.augment_four, i.augment_five, i.augment_six);
		c->PutItemInInventory(i.slot_id, *inst);
	}

	Parcel_Struct ps{};
	ps.quantity   = 1;
	ps.item_slot  = c->FindNextFreeParcelSlotUsingMemory();
	ps.money_flag = 0;
	ps.npc_id     = 0;
	strn0cpy(ps.send_to, "TestParcelCharacter", sizeof(ps.send_to));
	strn0cpy(ps.note, "This is a test note.", sizeof(ps.note));

	c->DoParcelSend(&ps);

	std::cout << "\n===========================================\n";
	std::cout << "✅ All NPC Hand-in Tests Completed!\n";
	std::cout << "===========================================\n";
}