#include "../../../common/repositories/inventory_repository.h"
#include "../../zone_cli.h"
#include "fmt/chrono.h"

struct Inventory {
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

struct ParcelSend {
	uint32      npc_id;
	uint32      item_slot;
	uint32      quantity;
	uint32      money_flag;
	std::string send_to;
	std::string note;
};

struct ParcelInventoryDetails {
	uint32                         action; // 0 remove 1 update inventory, 2 add to inventory
	InventoryRepository::Inventory inventory;
};

struct ParcelSendResults {
	CharacterParcelsRepository::CharacterParcels db_parcels;
	ParcelInventoryDetails                       db_inventory;
};

struct ParcelTestCase {
	std::string       description;
	ParcelSend        parcel_send;
	ParcelSendResults parcel_send_results;
};

struct ParcelRetrieve {
	std::string                    description;
	ParcelRetrieve_Struct          parcel_retrieve;
	InventoryRepository::Inventory parcel_retrieve_results;
};

inline CharacterDataRepository::CharacterData SeedCharacterTable()
{
	const std::string charcter_name = "Parcel_Regression_Character";

	auto characters = CharacterDataRepository::GetWhere(database, fmt::format("name = '{}'", charcter_name));

	if (!characters.empty()) {
		return characters.front();
	}

	// seed the character table
	auto character_data             = CharacterDataRepository::NewEntity();
	character_data.id               = 0;
	character_data.name             = charcter_name;
	character_data.zone_id          = Zones::BAZAAR;
	character_data.zone_instance    = 0;
	character_data.x                = 1372.38;
	character_data.y                = 1041.5;
	character_data.z                = 34.4159;
	character_data.heading          = 233.25;
	character_data.race             = Race::HalfElf;
	character_data.class_           = Class::Druid;
	character_data.level            = 60;
	character_data.level2           = 60;
	character_data.deity            = Deity::Veeshan;
	character_data.face             = 3;
	character_data.hair_color       = 15;
	character_data.hair_style       = 1;
	character_data.beard            = 255;
	character_data.beard_color      = 255;
	character_data.eye_color_1      = 3;
	character_data.eye_color_2      = 3;
	character_data.exp_enabled      = 1;
	character_data.cur_hp           = 3000;
	character_data.mana             = 1500;
	character_data.endurance        = 1500;
	character_data.str              = 75;
	character_data.sta              = 75;
	character_data.cha              = 110;
	character_data.dex              = 95;
	character_data.int_             = 75;
	character_data.agi              = 90;
	character_data.wis              = 60;
	character_data.air_remaining    = 60;
	character_data.xtargets         = 5;

	auto character = CharacterDataRepository::InsertOne(database, character_data);

	return character;
}

inline void SeedCharacterInventoryTable(uint32 character_id)
{
	//auto inventory = InventoryRepository::GetWhere(database, fmt::format("`character_id` = {}", character_id));
	InventoryRepository::DeleteWhere(database, fmt::format("`character_id` = {}", character_id));

	std::vector<Inventory> inventory_data{
			{ EQ::invslot::GENERAL_BEGIN, 35034, 1, 0, 0, 0, 0, 0, 0 }, // Trader Satchel
			{ EQ::invslot::GENERAL_BEGIN + 1, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invslot::GENERAL_BEGIN + 2, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invslot::GENERAL_BEGIN + 3, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invslot::GENERAL_BEGIN + 4, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invslot::GENERAL_BEGIN + 5, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invslot::GENERAL_BEGIN + 6, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invslot::GENERAL_BEGIN + 7, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invslot::GENERAL_BEGIN + 8, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invslot::GENERAL_BEGIN + 9, 34079, 1, 0, 0, 0, 0, 0, 0 },        // Fabled Backpack
			{ EQ::invbag::GENERAL_BAGS_BEGIN, 4171, 1, 85490, 0, 0, 0, 0, 0 },     //Rubicite Greaves with an Augment
			{ EQ::invbag::GENERAL_BAGS_BEGIN + 1, 8517, 50, 0, 0, 0, 0, 0, 0 },    //Class 6 Arrows (stackable)
			{ EQ::invbag::GENERAL_BAGS_BEGIN + 2, 8517, 32767, 0, 0, 0, 0, 0, 0 }, //Class 6 Arrows (stackable)
			{ EQ::invbag::GENERAL_BAGS_BEGIN + 3, 70208, 1, 0, 0, 0, 0, 0, 0 },    //Shroud of the Fallen Defender with charges -1
			{ EQ::invbag::GENERAL_BAGS_BEGIN + 4, 14196, 3, 0, 0, 0, 0, 0, 0 },    //5 Dose Potion of Lesser Cohesion with 3 charges
			{ EQ::invbag::GENERAL_BAGS_BEGIN + 5, 8205, 0, 0, 0, 0, 0, 0, 0 },     //Glowing Org with 0 charges
			{ EQ::invbag::GENERAL_BAGS_BEGIN + 6, 5106, 1, 0, 0, 0, 0, 0, 0 },     //Shadowed Halberd (Giant)
			{ EQ::invbag::GENERAL_BAGS_BEGIN + 7, 34079, 1, 0, 0, 0, 0, 0, 0 },    //Fabled Backpack
		};

	std::vector<InventoryRepository::Inventory> queue{};
	for (auto const &i: inventory_data) {
		auto item          = InventoryRepository::NewEntity();
		item.character_id  = character_id;
		item.item_id       = i.item_id;
		item.slot_id       = i.slot_id;
		item.charges       = i.charges;
		item.augment_one   = i.augment_one;
		item.augment_two   = i.augment_two;
		item.augment_three = i.augment_three;
		item.augment_four  = i.augment_four;
		item.augment_five  = i.augment_five;
		item.augment_six   = i.augment_six;
		item.guid          = 0;

		queue.push_back(item);
	}

	InventoryRepository::InsertMany(database, queue);

}

inline void DeleteParcels(uint32 character_id)
{
	CharacterParcelsRepository::DeleteWhere(database, fmt::format("`char_id` = {}", character_id));
}

void RunParcelSendTests(Client *c, std::vector<ParcelTestCase> &test_cases)
{
		for (auto const& p : test_cases) {
		Parcel_Struct ps{};
		ps.quantity   = p.parcel_send.quantity;
		ps.item_slot  = p.parcel_send.item_slot;
		ps.money_flag = p.parcel_send.money_flag;
		ps.npc_id     = p.parcel_send.npc_id;
		strn0cpy(ps.send_to, p.parcel_send.send_to.c_str(), sizeof(ps.send_to));
		strn0cpy(ps.note, p.parcel_send.note.c_str(), sizeof(ps.note));

		c->SetParcelEnabled(true);
		c->DoParcelSend(&ps);

		auto result = CharacterParcelsRepository::GetWhere(
			database,
			fmt::format(
				"`char_id` = {} AND `slot_id` = {}",
				p.parcel_send_results.db_parcels.char_id,
				p.parcel_send_results.db_parcels.slot_id
			)
		);

		bool passed = true;

		if (result.empty()) {
			passed = false;
		}

		for (auto const &r : result) {
			if (r.item_id != p.parcel_send_results.db_parcels.item_id) {
				passed = false;
				RunTest("Parcel DB: Item ID mismatch", p.parcel_send_results.db_parcels.item_id, r.item_id);
				break;
			}
			if (r.quantity != p.parcel_send_results.db_parcels.quantity) {
				passed = false;
				RunTest("Parcel DB: Charges mismatch", p.parcel_send_results.db_parcels.quantity, r.quantity);
				break;
			}
			if (r.aug_slot_1 != p.parcel_send_results.db_parcels.aug_slot_1 ||
				r.aug_slot_2 != p.parcel_send_results.db_parcels.aug_slot_2 ||
				r.aug_slot_3 != p.parcel_send_results.db_parcels.aug_slot_3 ||
				r.aug_slot_4 != p.parcel_send_results.db_parcels.aug_slot_4 ||
				r.aug_slot_5 != p.parcel_send_results.db_parcels.aug_slot_5 ||
				r.aug_slot_6 != p.parcel_send_results.db_parcels.aug_slot_6) {
				passed = false;
				RunTest("Parcel DB: Augment mismatch", true, passed);
				break;
			}
			if (r.from_name != p.parcel_send_results.db_parcels.from_name) {
				passed = false;
				RunTest("Parcel DB: From name mismatch", p.parcel_send_results.db_parcels.from_name, r.from_name);
				break;
			}
			if (r.note != p.parcel_send_results.db_parcels.note) {
				passed = false;
				RunTest("Parcel DB: Parcel Notes mismatch", p.parcel_send_results.db_parcels.note, r.note);
				break;
			}
			//if (r.sent_date != p.parcel_send_results.db_parcels.sent_date) {
			//	passed = false;
			//	RunTest("Parcel DB: Sent date mismatch", p.parcel_send_results.db_parcels.sent_date, r.sent_date);
			//	break;
			//}
		}

		switch (p.parcel_send_results.db_inventory.action) {
			case 1: {
				auto inv = InventoryRepository::GetWhere(
					database,
					fmt::format(
						"`character_id` = {} AND `slot_id` = {}",
						p.parcel_send_results.db_inventory.inventory.character_id,
						p.parcel_send_results.db_inventory.inventory.slot_id
					)
				);

				if (inv.empty()) {
					passed = false;
				}

				for (auto const &r: inv) {
					if (r.item_id != p.parcel_send_results.db_inventory.inventory.item_id) {
						passed = false;
						RunTest(
							"Inventory: Item ID mismatch",
							p.parcel_send_results.db_inventory.inventory.item_id,
							r.item_id);
						break;
					}
					if (r.charges != p.parcel_send_results.db_inventory.inventory.charges) {
						passed = false;
						RunTest(
							"Inventory: Charges mismatch",
							p.parcel_send_results.db_inventory.inventory.charges,
							r.charges);
						break;
					}
					if (r.slot_id != p.parcel_send_results.db_inventory.inventory.slot_id) {
						passed = false;
						RunTest(
							"Inventory: Slot ID mismatch",
							p.parcel_send_results.db_inventory.inventory.slot_id,
							r.slot_id);
						break;
					}
					if (r.guid != p.parcel_send_results.db_inventory.inventory.guid) {
						passed = false;
						RunTest("Inventory: GUID mismatch", p.parcel_send_results.db_inventory.inventory.guid, r.guid);
						break;
					}
				}

				RunTest(p.description, true, passed);
				break;
			}
			case 0: {
				passed = false;
				auto inv = InventoryRepository::GetWhere(
					database,
					fmt::format(
						"`character_id` = {} AND `slot_id` = {}",
						p.parcel_send_results.db_inventory.inventory.character_id,
						p.parcel_send_results.db_inventory.inventory.slot_id
					)
				);

				if (inv.empty()) {
					passed = true;
				}

				RunTest(p.description, true, passed);
				break;
			}
			case 2: {
				passed = false;
				if (c->GetCarriedMoney() == 4444) {
					passed = true;
				}

				RunTest(p.description, true, passed);
				break;
			}
		}
	}

	std::cout << "\n===========================================\n";
	std::cout << "✅ All Parcels Send Tests Completed!\n";
	std::cout << "===========================================\n";
}

void RunParcelRetrieveTests(Client *c, std::vector<ParcelRetrieve> &test_cases)
{
	bool passed = true;
	for (auto const &r : test_cases) {
		c->DoParcelRetrieve(r.parcel_retrieve);

		if (r.parcel_retrieve_results.item_id == PARCEL_MONEY_ITEM_ID) {
			if (c->GetCarriedMoney() != 5555) {
				passed = false;
			}
			RunTest("Retrieve Parcel: 1p 1g 1s 1c", true, passed);
			break;
		}

		auto inv = InventoryRepository::GetWhere(
			database,
			fmt::format(
				"`character_id` = {} AND `slot_id` = {}",
				r.parcel_retrieve_results.character_id,
				r.parcel_retrieve_results.slot_id
			)
		);

		if (inv.empty()) {
			passed = false;
		}

		for (auto const &i : inv) {
			if (r.parcel_retrieve_results.item_id != i.item_id) {
				passed = false;
				RunTest("Inventory: Item ID mismatch", r.parcel_retrieve_results.item_id, i.item_id);
				break;
			}
			if (r.parcel_retrieve_results.charges != i.charges) {
				passed = false;
				RunTest("Inventory: Charges mismatch", r.parcel_retrieve_results.charges, i.charges);
				break;
			}
			if (r.parcel_retrieve_results.guid != i.guid) {
				passed = false;
				RunTest("Inventory: GUID mismatch", r.parcel_retrieve_results.guid, i.guid);
				break;
			}
			if (r.parcel_retrieve_results.augment_one != i.augment_one ||
				r.parcel_retrieve_results.augment_two != i.augment_two ||
				r.parcel_retrieve_results.augment_three != i.augment_three ||
				r.parcel_retrieve_results.augment_four != i.augment_four ||
				r.parcel_retrieve_results.augment_five != i.augment_five ||
				r.parcel_retrieve_results.augment_six != i.augment_six) {
				passed = false;
				RunTest("Inventory: Augments mismatch", true, false);
				break;
				}
		}
		RunTest(r.description, true, passed);
	}


	std::cout << "\n===========================================\n";
	std::cout << "✅ All Parcels Retrieve Tests Completed!\n";
	std::cout << "===========================================\n";
}
void ZoneCLI::TestParcels(int argc, char **argv, argh::parser &cmd, std::string &description)
{
	if (cmd[{"-h", "--help"}]) {
		return;
	}

	LogSys.SilenceConsoleLogging();

	Zone::Bootup(ZoneID("arena2"), 0, false);
	zone->StopShutdownTimer();

	auto   npc_type = content_db.LoadNPCTypesData(202129);
	if (npc_type) {
		auto npc = new NPC(
			npc_type,
			nullptr,
			glm::vec4(0, 0, 0, 0),
			GravityBehavior::Water
		);

		entity_list.AddNPC(npc);
	}

	entity_list.Process();
	entity_list.MobProcess();

	std::cout << "===========================================\n";
	std::cout << "⚙\uFE0F> Running Parcel Tests...\n";
	std::cout << "===========================================\n\n";

	auto    character = SeedCharacterTable();
	SeedCharacterInventoryTable(character.id);

	Client *c            = new Client();
	c->SetCharacterID(character.id);
	c->SetClientVersion(EQ::versions::ClientVersion::RoF2);
	c->SetClientVersionBit(EQ::versions::ConvertClientVersionToClientVersionBit(c->ClientVersion()));
	c->GetInv().SetInventoryVersion(EQ::versions::MobVersion::RoF2);
	c->SetName(character.name.c_str());
	c->AddMoneyToPP(4, 4, 4, 4);

	database.LoadCharacterData(c->CharacterID(), &c->GetPP(), &c->GetEPP());
	database.GetInventory(c);

	entity_list.AddClient(c);
	entity_list.Process();

	DeleteParcels(c->CharacterID());
	c->LoadParcels();

	auto merchant = entity_list.GetNPCByNPCTypeID(202129);

	auto date = time(nullptr);

	std::vector<ParcelTestCase> test_cases_send = {
		{
			.description = "Send Parcel: 23 - Class 6 Arrows from Inventory Slot 4011",
			.parcel_send = {
				.npc_id     = merchant->GetID(),
				.item_slot  = EQ::invbag::GENERAL_BAGS_BEGIN + 1,
				.quantity   = 23,
				.money_flag = 0,
				.send_to    = std::string(c->GetName()),
				.note       = "Send Parcel: 23 - Class 6 Arrows from Inventory Slot 4011"
			},
			.parcel_send_results = {
				.db_parcels  = {
					.char_id = c->CharacterID(),
					.item_id = 8517,
					.aug_slot_1 = 0,
					.aug_slot_2 = 0,
					.aug_slot_3 = 0,
					.aug_slot_4 = 0,
					.aug_slot_5 = 0,
					.aug_slot_6 = 0,
					.slot_id = 1,
					.quantity = 23,
					.from_name = std::string(c->GetName()),
					.note = std::string("Send Parcel: 23 - Class 6 Arrows from Inventory Slot 4011"),
					.sent_date = date
				},
				.db_inventory = {
					.action = 1,
					.inventory = {
						.character_id = c->CharacterID(),
						.slot_id = EQ::invbag::GENERAL_BAGS_BEGIN + 1,
						.item_id = 8517,
						.charges = 27,
						.augment_one = 0,
						.augment_two = 0,
						.augment_three = 0,
						.augment_four = 0,
						.augment_five = 0,
						.augment_six = 0,
						.guid = 14
					}
				}
			}
		},
			{
				.description         = "Send Parcel: 1 - Shroud of the Fallen Defender from Inventory Slot 4013",
				.parcel_send         = {
					.npc_id     = merchant->GetID(),
					.item_slot  = EQ::invbag::GENERAL_BAGS_BEGIN + 3,
					.quantity   = 1,
					.money_flag = 0,
					.send_to    = std::string(c->GetName()),
					.note       = "Send Parcel: 1 - Shroud of the Fallen Defender from Inventory Slot 4013" },
					.parcel_send_results = {
					.db_parcels   = {
						.char_id    = c->CharacterID(),
						.item_id    = 70208,
						.aug_slot_1 = 0,
						.aug_slot_2 = 0,
						.aug_slot_3 = 0,
						.aug_slot_4 = 0,
						.aug_slot_5 = 0,
						.aug_slot_6 = 0,
						.slot_id    = 2,
						.quantity   = 1,
						.from_name  = std::string(c->GetName()),
						.note       = std::string("Send Parcel: 1 - Shroud of the Fallen Defender from Inventory Slot 4013"),
						.sent_date = date
					},
					.db_inventory = {
						.action = 0,
						.inventory = {
							.character_id  = c->CharacterID(),
							.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 3,
							.item_id       = 70208,
							.charges       = 1,
							.augment_one   = 0,
							.augment_two   = 0,
							.augment_three = 0,
							.augment_four  = 0,
							.augment_five  = 0,
							.augment_six   = 0,
							.guid          = 16
						}
					}
				}
			},
				{
					.description         = "Send Parcel: 1 - 5 Dose Potion with 3 charges from Inventory Slot 4014",
					.parcel_send         = {
						.npc_id     = merchant->GetID(),
						.item_slot  = EQ::invbag::GENERAL_BAGS_BEGIN + 4,
						.quantity   = 1,
						.money_flag = 0,
						.send_to    = std::string(c->GetName()),
						.note       = "Send Parcel: 1 - 5 Dose Potion with 3 charges from Inventory Slot 4014"
					},
						.parcel_send_results = {
						.db_parcels   = {
							.char_id    = c->CharacterID(),
							.item_id    = 14196,
							.aug_slot_1 = 0,
							.aug_slot_2 = 0,
							.aug_slot_3 = 0,
							.aug_slot_4 = 0,
							.aug_slot_5 = 0,
							.aug_slot_6 = 0,
							.slot_id    = 3,
							.quantity   = 3,
							.from_name  = std::string(c->GetName()),
							.note       = std::string("Send Parcel: 1 - 5 Dose Potion with 3 charges from Inventory Slot 4014"),
							.sent_date = date
						},
						.db_inventory = {
							.action = 0,
							.inventory = {
								.character_id  = c->CharacterID(),
								.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 4,
								.item_id       = 14196,
								.charges       = 3,
								.augment_one   = 0,
								.augment_two   = 0,
								.augment_three = 0,
								.augment_four  = 0,
								.augment_five  = 0,
								.augment_six   = 0,
								.guid          = 17
							}
						}
						}
				},
			{
				.description         = "Send Parcel: 1 - Glowing Orb with 0 charges from Inventory Slot 4015",
				.parcel_send         = {
					.npc_id     = merchant->GetID(),
					.item_slot  = EQ::invbag::GENERAL_BAGS_BEGIN + 5,
					.quantity   = 1,
					.money_flag = 0,
					.send_to    = std::string(c->GetName()),
					.note       = "Send Parcel: 1 - Glowing Orb with 0 charges from Inventory Slot 4015"
				},
					.parcel_send_results = {
					.db_parcels   = {
						.char_id    = c->CharacterID(),
						.item_id    = 8205,
						.aug_slot_1 = 0,
						.aug_slot_2 = 0,
						.aug_slot_3 = 0,
						.aug_slot_4 = 0,
						.aug_slot_5 = 0,
						.aug_slot_6 = 0,
						.slot_id    = 4,
						.quantity   = 0,
						.from_name  = std::string(c->GetName()),
						.note       = std::string("Send Parcel: 1 - Glowing Orb with 0 charges from Inventory Slot 4015"),
						.sent_date = date
					},
					.db_inventory = {
						.action = 0,
						.inventory = {
							.character_id  = c->CharacterID(),
							.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 5,
							.item_id       = 8205,
							.charges       = 0,
							.augment_one   = 0,
							.augment_two   = 0,
							.augment_three = 0,
							.augment_four  = 0,
							.augment_five  = 0,
							.augment_six   = 0,
							.guid          = 18
						}
					}
					}
			},
			{
				.description         = "Send Parcel: 32767 - Class 6 Arrows from Inventory Slot 4012",
				.parcel_send         = {
					.npc_id     = merchant->GetID(),
					.item_slot  = EQ::invbag::GENERAL_BAGS_BEGIN + 2,
					.quantity   = 32767,
					.money_flag = 0,
					.send_to    = std::string(c->GetName()),
					.note       = "Send Parcel: 32767 - Class 6 Arrows from Inventory Slot 4012" },
					.parcel_send_results = {
					.db_parcels   = {
						.char_id    = c->CharacterID(),
						.item_id    = 8517,
						.aug_slot_1 = 0,
						.aug_slot_2 = 0,
						.aug_slot_3 = 0,
						.aug_slot_4 = 0,
						.aug_slot_5 = 0,
						.aug_slot_6 = 0,
						.slot_id    = 5,
						.quantity   = 32767,
						.from_name  = std::string(c->GetName()),
						.note       = std::string("Send Parcel: 32767 - Class 6 Arrows from Inventory Slot 4012"),
						.sent_date = date
					},
					.db_inventory = {
						.action = 0,
						.inventory = {
							.character_id  = c->CharacterID(),
							.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 2,
							.item_id       = 8517,
							.charges       = 32767,
							.augment_one   = 0,
							.augment_two   = 0,
							.augment_three = 0,
							.augment_four  = 0,
							.augment_five  = 0,
							.augment_six   = 0,
							.guid          = 15
						}
					}
					}
			},
			{
				.description         = "Send Parcel: 1 - Rubicite Greaves with an Augment of 85490 from Inventory Slot 4010",
				.parcel_send         = {
					.npc_id     = merchant->GetID(),
					.item_slot  = EQ::invbag::GENERAL_BAGS_BEGIN,
					.quantity   = 4171,
					.money_flag = 0,
					.send_to    = std::string(c->GetName()),
					.note       = "Send Parcel: 1 - Rubicite Greaves with an Augment of 85490 from Inventory Slot 4010" },
					.parcel_send_results = {
					.db_parcels   = {
						.char_id    = c->CharacterID(),
						.item_id    = 4171,
						.aug_slot_1 = 85490,
						.aug_slot_2 = 0,
						.aug_slot_3 = 0,
						.aug_slot_4 = 0,
						.aug_slot_5 = 0,
						.aug_slot_6 = 0,
						.slot_id    = 6,
						.quantity   = 1,
						.from_name  = std::string(c->GetName()),
						.note       = std::string("Send Parcel: 1 - Rubicite Greaves with an Augment of 85490 from Inventory Slot 4010"),
						.sent_date = date
					},
					.db_inventory = {
						.action = 0,
						.inventory = {
							.character_id  = c->CharacterID(),
							.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN ,
							.item_id       = 4171,
							.charges       = 1,
							.augment_one   = 85490,
							.augment_two   = 0,
							.augment_three = 0,
							.augment_four  = 0,
							.augment_five  = 0,
							.augment_six   = 0,
							.guid          = 12
						}
					}
					}
			},
			{
				.description         = "Send Parcel: 1p 1g 1s 1c",
				.parcel_send         = {
					.npc_id     = merchant->GetID(),
					.item_slot  = 0xFFFFFFFF,
					.quantity   = 1111,
					.money_flag = 1,
					.send_to    = std::string(c->GetName()),
					.note       = "Send Parcel: 1p 1g 1s 1c" },
					.parcel_send_results = {
					.db_parcels   = {
						.char_id    = c->CharacterID(),
						.item_id    = PARCEL_MONEY_ITEM_ID,
						.aug_slot_1 = 0,
						.aug_slot_2 = 0,
						.aug_slot_3 = 0,
						.aug_slot_4 = 0,
						.aug_slot_5 = 0,
						.aug_slot_6 = 0,
						.slot_id    = 7,
						.quantity   = 1111,
						.from_name  = std::string(c->GetName()),
						.note       = std::string("Send Parcel: 1p 1g 1s 1c"),
						.sent_date = date
					},
					.db_inventory = {
						.action = 2,
						.inventory = {
							.character_id  = c->CharacterID(),
							.slot_id       = 0,
							.item_id       = PARCEL_MONEY_ITEM_ID,
							.charges       = 1111,
							.augment_one   = 0,
							.augment_two   = 0,
							.augment_three = 0,
							.augment_four  = 0,
							.augment_five  = 0,
							.augment_six   = 0,
							.guid          = 0
						}
					}
					}
			}
	};

	std::vector<ParcelRetrieve> test_cases_retrieve = {
		{
			.description = "Retrieve Parcel: 1 Rubicite Greaves with augment 85490 placed in inventory slot 4010",
			.parcel_retrieve = {
				.merchant_entity_id = merchant->GetID(),
				.player_entity_id = c->GetID(),
				.parcel_slot_id = 6,
				.parcel_item_id = 4171
			},
			.parcel_retrieve_results = {
				.character_id  = c->CharacterID(),
				.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN,
				.item_id       = 4171,
				.charges       = 1,
				.augment_one   = 85490,
				.augment_two   = 0,
				.augment_three = 0,
				.augment_four  = 0,
				.augment_five  = 0,
				.augment_six   = 0,
				.guid          = 22
			}
		},
{
	.description = "Retrieve Parcel: 32767 - Class 6 Arrows placed in inventory slot 4012",
	.parcel_retrieve = {
		.merchant_entity_id = merchant->GetID(),
		.player_entity_id = c->GetID(),
		.parcel_slot_id = 5,
		.parcel_item_id = 8517
	},
	.parcel_retrieve_results = {
		.character_id  = c->CharacterID(),
		.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 2,
		.item_id       = 8517,
		.charges       = 32767,
		.augment_one   = 0,
		.augment_two   = 0,
		.augment_three = 0,
		.augment_four  = 0,
		.augment_five  = 0,
		.augment_six   = 0,
		.guid          = 24
	}
},
{
	.description = "Retrieve Parcel: 23 - Class 6 Arrows placed in inventory slot 4011",
	.parcel_retrieve = {
		.merchant_entity_id = merchant->GetID(),
		.player_entity_id = c->GetID(),
		.parcel_slot_id = 1,
		.parcel_item_id = 8517
	},
	.parcel_retrieve_results = {
		.character_id  = c->CharacterID(),
		.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 1,
		.item_id       = 8517,
		.charges       = 50,
		.augment_one   = 0,
		.augment_two   = 0,
		.augment_three = 0,
		.augment_four  = 0,
		.augment_five  = 0,
		.augment_six   = 0,
		.guid          = 14
	}
},
{
	.description = "Retrieve Parcel: 1 - Shroud of the Fallen Defender in inventory slot 4013",
	.parcel_retrieve = {
		.merchant_entity_id = merchant->GetID(),
		.player_entity_id = c->GetID(),
		.parcel_slot_id = 2,
		.parcel_item_id = 70208
	},
	.parcel_retrieve_results = {
		.character_id  = c->CharacterID(),
		.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 3,
		.item_id       = 70208,
		.charges       = 1,
		.augment_one   = 0,
		.augment_two   = 0,
		.augment_three = 0,
		.augment_four  = 0,
		.augment_five  = 0,
		.augment_six   = 0,
		.guid          = 26
	}
},
{
	.description = "Retrieve Parcel: 1 - 5 Dose Potion of Lesser Cohesion with 3 charges in inventory slot 4014",
	.parcel_retrieve = {
		.merchant_entity_id = merchant->GetID(),
		.player_entity_id = c->GetID(),
		.parcel_slot_id = 3,
		.parcel_item_id = 14196
	},
	.parcel_retrieve_results = {
		.character_id  = c->CharacterID(),
		.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 4,
		.item_id       = 14196,
		.charges       = 3,
		.augment_one   = 0,
		.augment_two   = 0,
		.augment_three = 0,
		.augment_four  = 0,
		.augment_five  = 0,
		.augment_six   = 0,
		.guid          = 27
	}
},
{
	.description = "Retrieve Parcel: 1 - Glowing Orb with 0 charges in inventory slot 4015",
	.parcel_retrieve = {
		.merchant_entity_id = merchant->GetID(),
		.player_entity_id = c->GetID(),
		.parcel_slot_id = 4,
		.parcel_item_id = 8205
	},
	.parcel_retrieve_results = {
		.character_id  = c->CharacterID(),
		.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN + 5,
		.item_id       = 8205,
		.charges       = 0,
		.augment_one   = 0,
		.augment_two   = 0,
		.augment_three = 0,
		.augment_four  = 0,
		.augment_five  = 0,
		.augment_six   = 0,
		.guid          = 28
	}
	},
{
	.description = "Retrieve Parcel: 1p 1g 1s 1c",
	.parcel_retrieve = {
		.merchant_entity_id = merchant->GetID(),
		.player_entity_id = c->GetID(),
		.parcel_slot_id = 7,
		.parcel_item_id = PARCEL_MONEY_ITEM_ID
	},
	.parcel_retrieve_results = {
		.character_id  = c->CharacterID(),
		.slot_id       = EQ::invbag::GENERAL_BAGS_BEGIN,
		.item_id       = PARCEL_MONEY_ITEM_ID,
		.charges       = 1111,
	}
}

	};

	RunParcelSendTests(c, test_cases_send);
	c->LoadParcels();
	RunParcelRetrieveTests(c, test_cases_retrieve);
}
