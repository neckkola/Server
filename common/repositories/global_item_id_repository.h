#ifndef EQEMU_GLOBAL_ITEM_ID_REPOSITORY_H
#define EQEMU_GLOBAL_ITEM_ID_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_global_item_id_repository.h"

class GlobalItemIdRepository: public BaseGlobalItemIdRepository {
public:

    /**
     * This file was auto generated and can be modified and extended upon
     *
     * Base repository methods are automatically
     * generated in the "base" version of this repository. The base repository
     * is immutable and to be left untouched, while methods in this class
     * are used as extension methods for more specific persistence-layer
     * accessors or mutators.
     *
     * Base Methods (Subject to be expanded upon in time)
     *
     * Note: Not all tables are designed appropriately to fit functionality with all base methods
     *
     * InsertOne
     * UpdateOne
     * DeleteOne
     * FindOne
     * GetWhere(std::string where_filter)
     * DeleteWhere(std::string where_filter)
     * InsertMany
     * All
     *
     * Example custom methods in a repository
     *
     * GlobalItemIdRepository::GetByZoneAndVersion(int zone_id, int zone_version)
     * GlobalItemIdRepository::GetWhereNeverExpires()
     * GlobalItemIdRepository::GetWhereXAndY()
     * GlobalItemIdRepository::DeleteWhereXAndY()
     *
     * Most of the above could be covered by base methods, but if you as a developer
     * find yourself re-using logic for other parts of the code, its best to just make a
     * method that can be re-used easily elsewhere especially if it can use a base repository
     * method and encapsulate filters there
     */

	// Custom extended repository methods here

	static void LockTable(Database& db)
	{
		auto query = fmt::format("LOCK TABLE `{}` WRITE WAIT 2;", TableName());
		db.QueryDatabase(query);
	}

	static void UnLockTable(Database& db)
	{
		auto query = "UNLOCK TABLES";
		db.QueryDatabase(query);
	}

	static uint64 GetNextSerialNumberRange(Database& db)
	{
		LockTable(db);

		auto x = All(db);
		if (x.empty()) {
			UnLockTable(db);
			return 0;
		}

		auto u = x.front();
		u.number += SERIAL_NUMBER_RANGE;
		ReplaceOne(db, u);

		UnLockTable(db);
		return u.number;
	}
};

#endif //EQEMU_GLOBAL_ITEM_ID_REPOSITORY_H
