
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "data_store.hpp"
#include "stdio.h"

using namespace ::testing;

namespace ins_service
{

class DataStoreFixture : public Test
{
public:
    virtual void SetUp()
    {
        //spdlog::set_level(spdlog::level::debug);
    }

    sqlite3* GetDatabase(std::shared_ptr<DataStore> ds)
    {
        return ds->database_;
    }

    std::string GetExecutingSql()
    {
        return data_store_->executing_sql_;
    }

    bool CreateLocationTable()
    {
        return data_store_->CreateLocationTable();
    }

    bool RunQuery(std::string sql)
    {
        return data_store_->RunQuery(sql);
    }

    bool ReadDistinctMacAddrs(const std::string& device_id, std::vector<std::string>& mac_addrs)
    {
        return data_store_->ReadDistinctMacAddrs(device_id, mac_addrs);
    }

protected:
    std::shared_ptr<DataStore> data_store_ = std::make_shared<DataStore>();
};

/**
 * TEST: Init
 * EXPECT: Creates new db file.
 */
TEST_F(DataStoreFixture, Init_DbNotExisting_WillCreateNewDb)
{
    data_store_->Init("database_file");
    sqlite3* db = GetDatabase(data_store_);
    EXPECT_NE(db, nullptr);
    std::remove("database_file");
}

/*
 * TEST: Init
 * EXPECT: Loads existing db
 */
TEST_F(DataStoreFixture, Init_DbCreated_WillLoadExistingDb)
{
    sqlite3* db = GetDatabase(data_store_);
    EXPECT_EQ(db, nullptr);

    data_store_->Init("database_file");
    db = GetDatabase(data_store_);
    EXPECT_NE(db, nullptr);

    data_store_->Close();
    db = GetDatabase(data_store_);
    EXPECT_EQ(db, nullptr);

    std::shared_ptr<DataStore> data_store2_ = std::make_shared<DataStore>();
    data_store2_->Init("database_file");
    sqlite3* db2 = GetDatabase(data_store2_);
    EXPECT_NE(db2, nullptr);
    std::remove("database_file");
}

/**
 * TEST: Close
 * EXPECT: Close database after initialization
 */
TEST_F(DataStoreFixture, Close_DbInitialized_WillCloseDb)
{
    data_store_->Init("database_file");
    EXPECT_NE(GetDatabase(data_store_), nullptr);
    data_store_->Close();
    EXPECT_EQ(GetDatabase(data_store_), nullptr);
}

/**
 * TEST: CreateLocationTable
 * EXPECT: Builds SQL for creating location table
 */
TEST_F(DataStoreFixture, CreateLocationTable_WillFormSqlToCreateLocationTable)
{
    data_store_->Init("location_db");
    EXPECT_TRUE(CreateLocationTable());
    std::string expected_sql = "CREATE TABLE IF NOT EXISTS location(device_id INTEGER PRIMARY KEY,"
                               "employee_id TEXT,"
                               "pos_x REAL,"
                               "pos_y REAL,"
                               "pos_z REAL,"
                               "timestamp datatime default current_timestamp);";

    EXPECT_EQ(expected_sql, GetExecutingSql());
    data_store_->Close();
    std::remove("location_db");
}

/**
 * TEST: CreateDeviceTable
 * EXPECT: Builds SQL for creating device table
 */
TEST_F(DataStoreFixture, CreateDeviceTable_WillFormSqlToCreateDeviceTable)
{
    data_store_->Init("db");
    std::string device_id = "2435";
    EXPECT_TRUE(data_store_->CreateDeviceTable(device_id));
    std::string expected_sql = "CREATE TABLE IF NOT EXISTS dev_" + device_id
                               + "(id INTEGER PRIMARY KEY,"
                                 "mac_addr TEXT, rssi REAL,"
                                 "timestamp datatime default current_timestamp);";
    EXPECT_EQ(expected_sql, GetExecutingSql());
    data_store_->Close();
    std::remove("db");
}

/**l
 * TEST: ClearDeviceTable
 * EXPECT: Build SQL for clearing device table
 */
TEST_F(DataStoreFixture, ClearDeviceTable_WillFormSqlToClearTable)
{
    data_store_->Init("db");
    std::string device_id = "1000";
    data_store_->CreateDeviceTable(device_id);
    EXPECT_TRUE(data_store_->ClearDeviceTable(device_id));
    std::string expected_sql = "DELETE from dev_" + device_id;
    EXPECT_EQ(expected_sql, GetExecutingSql());
    data_store_->Close();
    std::remove("db");
}

/**
 * TEST: UpdateDeviceLocation
 * EXPECT: Builds SQL for updating device location
 */
TEST_F(DataStoreFixture, UpdateDeviceLocation_WillFormSqlToUpdateDeviceLocation)
{
    data_store_->Init("db");
    std::string device_id = "1000";
    Position    pos{ 2.0, 4.0, 6.0 };
    CreateLocationTable();
    EXPECT_TRUE(data_store_->UpdateDeviceLocation(device_id, pos));
    std::string expected_sql = "INSERT OR REPLACE INTO location (device_id, pos_x, pos_y, "
                               "pos_z, employee_id) VALUES ("
                               + device_id + "," + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", "
                               + std::to_string(pos.z) + ", (SELECT employee_id FROM location WHERE device_id="
                               + device_id + "));";
    EXPECT_EQ(expected_sql, GetExecutingSql());
    data_store_->Close();
    std::remove("db");
}

/**
 * TEST: AssignDeviceToEmployee
 * EXPECT: Builds SQL for linking a device to and employee
 */
TEST_F(DataStoreFixture, AssignDeviceToEmployee_WillFormSqlToLinkDeviceAndEmployee)
{
    data_store_->Init("db");
    std::string device_id   = "1000";
    std::string employee_id = "abcxyz";

    EXPECT_TRUE(data_store_->AssignDeviceToEmployee(device_id, employee_id));
    std::string expected_sql = "INSERT OR REPLACE INTO location (device_id, employee_id, pos_x, pos_y, pos_z) VALUES ("
                               + device_id + ",'" + employee_id + "',(SELECT pos_x FROM location WHERE device_id="
                               + device_id + "),(SELECT pos_y FROM location WHERE device_id=" + device_id
                               + "),(SELECT pos_z FROM location WHERE device_id=" + device_id + "));";
    EXPECT_EQ(expected_sql, GetExecutingSql());
    data_store_->Close();
    std::remove("db");
}

/**
 * TEST: InserRSSIReadings
 * EXPECT: Builds SQL for inserting rssi data
 */
TEST_F(DataStoreFixture, InsertRSSIReadings_WillFormSqlToInsertRSSIReadings)
{
    data_store_->Init("db");
    std::string              device_id        = "4004";
    std::vector<std::string> mac_address_list = { "ee:44:43:a5:ff:ef", "11:65:d4:fe:ee:ff", "01:23:dd:3e:4c:cc" };
    std::vector<double>      rssi_list        = { 23.66, 43.2, 99.9 };

    data_store_->CreateDeviceTable(device_id);
    EXPECT_TRUE(data_store_->InsertRSSIReadings(device_id, mac_address_list, rssi_list));
    std::string expected_sql = "INSERT INTO dev_" + device_id + " (mac_addr, rssi) VALUES"
                                                                "('ee:44:43:a5:ff:ef',23.66),('11:65:d4:fe:ee:ff',43.2)"
                                                                ",('01:23:dd:3e:4c:cc',99.9);";
    EXPECT_EQ(expected_sql, GetExecutingSql());
    data_store_->Close();
    std::remove("db");
}

/**
 * TEST: GetPosition
 * EXPECT: Gets position data for specified device
 */
TEST_F(DataStoreFixture, GetPosition_WillRunSqlToGetDevicePosition)
{
    data_store_->Init("location.db");
    CreateLocationTable();
    std::string device_id    = "1000";
    std::string device_id2   = "4003";
    std::string employee_id  = "abcxyz";
    std::string employee_id2 = "defuvw";
    Position    pos{ 2.0, 4.0, 6.0 };
    Position    pos2{ 25.5, 65.0, 43.0 };
    data_store_->UpdateDeviceLocation(device_id, pos);
    data_store_->UpdateDeviceLocation(device_id2, pos2);
    data_store_->AssignDeviceToEmployee(device_id, employee_id);
    data_store_->AssignDeviceToEmployee(device_id2, employee_id2);
    Position expected_position;
    Position expected_position2;
    data_store_->GetPosition(device_id, QueryT::DEVICE, expected_position);
    data_store_->GetPosition(device_id2, QueryT::DEVICE, expected_position2);
    EXPECT_EQ(expected_position, pos);
    EXPECT_EQ(expected_position2, pos2);
    data_store_->Close();
    std::remove("location.db");
}

/**
 * TEST: GetPosition
 * EXPECT: Get position data for specified employee
 */
TEST_F(DataStoreFixture, GetPosition_WillRunSqlToGetEmployeePosition)
{
    data_store_->Init("db");
    CreateLocationTable();
    std::string device_id    = "1000";
    std::string device_id2   = "4003";
    std::string employee_id  = "abcxyz";
    std::string employee_id2 = "defuvw";
    Position    pos{ 2.0, 4.0, 6.0 };
    Position    pos2{ 25.5, 65.0, 43.0 };
    data_store_->AssignDeviceToEmployee(device_id, employee_id);
    data_store_->AssignDeviceToEmployee(device_id2, employee_id2);
    data_store_->UpdateDeviceLocation(device_id, pos);
    data_store_->UpdateDeviceLocation(device_id2, pos2);

    Position expected_position;
    Position expected_position2;
    data_store_->GetPosition(employee_id, QueryT::EMPLOYEE, expected_position);
    data_store_->GetPosition(employee_id2, QueryT::EMPLOYEE, expected_position2);
    EXPECT_EQ(expected_position, pos);
    EXPECT_EQ(expected_position2, pos2);
    data_store_->Close();
    std::remove("db");
}

/**
 * TEST: Malformed SQL query
 * EXPECT: Function RunQuery returns false.
 */
TEST_F(DataStoreFixture, RunQuery_Fails)
{
    data_store_->Init("db");
    std::string query = "MALFORMED SQL.. IF NOT EXISTS test_table(device_id INTEGER PRIMARY KEY,employee_id TEXT);";
    EXPECT_FALSE(RunQuery(query));
    EXPECT_EQ(GetExecutingSql(), query);
    data_store_->Close();
    std::remove("db");
}

/**
 * TEST: RunQuery
 * EXPECT: Run SQL on database
 */
TEST_F(DataStoreFixture, RunQuery)
{
    data_store_->Init("db");
    std::string query = "CREATE TABLE IF NOT EXISTS test_table(col1 INTEGER PRIMARY KEY,col2 TEXT);";
    EXPECT_TRUE(RunQuery(query));
    EXPECT_EQ(GetExecutingSql(), query);
    query = "INSERT INTO test_table (col1, col2) VALUES (11,'test_value');";
    EXPECT_TRUE(RunQuery(query));
    EXPECT_EQ(GetExecutingSql(), query);
    data_store_->Close();
    std::remove("db");
}

/**
 * TEST: ReadDistinctMacAddrs
 * EXPECT: Unique mac addresses stored for a particular device
 */
TEST_F(DataStoreFixture, ReadDistinctMacAddrs_WillSetUniqueMacAddrsInDeviceTable)
{
    data_store_->Init("db");
    std::string              device_id = "4004";
    std::vector<std::string> mac_address_list
        = { "ee:44:43:a5:ff:ef", "11:65:d4:fe:ee:ff", "01:23:dd:3e:4c:cc", "ee:44:43:a5:ff:ef", "11:65:d4:fe:ee:ff",
            "01:23:dd:3e:4c:cc", "ee:44:43:a5:ff:ef", "11:65:d4:fe:ee:ff", "01:23:dd:3e:4c:cc", "ee:44:43:a5:ff:ef",
            "11:65:d4:fe:ee:ff", "01:23:dd:3e:4c:cc", "ee:44:43:a5:ff:ef", "11:65:d4:fe:ee:ff", "01:23:dd:3e:4c:cc" };
    std::vector<double> rssi_list
        = { 23.66, 43.2, 99.9, 23.66, 43.2, 99.9, 23.66, 43.2, 99.9, 23.66, 43.2, 99.9, 23.66, 43.2, 99.9 };

    data_store_->CreateDeviceTable(device_id);
    EXPECT_TRUE(data_store_->InsertRSSIReadings(device_id, mac_address_list, rssi_list));

    std::vector<std::string> unique_mac_addrs;
    std::vector<std::string> expected_unique_mac_addrs
        = { "ee:44:43:a5:ff:ef", "11:65:d4:fe:ee:ff", "01:23:dd:3e:4c:cc" };

    ReadDistinctMacAddrs(device_id, unique_mac_addrs);

    EXPECT_EQ(unique_mac_addrs.size(), expected_unique_mac_addrs.size());
    EXPECT_EQ(unique_mac_addrs, expected_unique_mac_addrs);

    data_store_->Close();
    std::remove("db");
}

} // namespace !ins_service