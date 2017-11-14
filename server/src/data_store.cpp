//
// Created by samueli on 2017-10-10.
//

#include <sstream>

#include "data_store.hpp"

namespace ins_service
{

void DataStore::Init(const std::string& db_filename)
{
    console_->debug("+ DataStore::Init");

    int result = sqlite3_open(db_filename.c_str(), &database_);
    if (result != 0)
    {
        console_->error("Can't open database: {0}", sqlite3_errmsg(database_));
        return;
    }
    console_->info("Opened database successfully");
    if (!CreateLocationTable())
    {
        console_->error("Cannot create location table");
    }

    console_->debug("- DataStore::Init");
    return;
}

void DataStore::Close(){
    sqlite3_close(database_);
    database_ = nullptr;
}


bool DataStore::CreateLocationTable()
{
    console_->debug("+ DataStore::CreateLocationTable");

    std::string sql = "CREATE TABLE IF NOT EXISTS location(device_id INTEGER PRIMARY KEY,"
                      "employee_id TEXT,"
                      "pos_x REAL,"
                      "pos_y REAL,"
                      "pos_z REAL,"
                      "timestamp datatime default current_timestamp);";
    console_->debug(sql);
    bool res = RunQuery(sql);

    console_->debug("- DataStore::CreateLocationTable");
    return res;
}

bool DataStore::CreateDeviceTable(const std::string& device_id)
{
    console_->debug("+ DataStore::CreateDeviceTable");

    std::string sql = "CREATE TABLE IF NOT EXISTS dev_" + device_id + "(id INTEGER PRIMARY KEY,"
                                                                      "mac_addr TEXT, rssi REAL,"
                                                                      "timestamp datatime default current_timestamp);";
    console_->debug(sql);
    bool res = RunQuery(sql);

    console_->debug("- DataStore::CreateDeviceTable");
    return res;
}

bool DataStore::ClearDeviceTable(const std::string& device_id)
{
    console_->debug("+ DataStore::ClearDeviceTable");

    std::string sql = "DELETE from dev_" + device_id;
    console_->info(sql);
    bool res = RunQuery(sql);

    console_->debug("- DataStore::ClearDeviceTable");
    return res;
}

bool DataStore::UpdateDeviceLocation(const std::string& device_id, Position pos)
{
    console_->debug("+ DataStore::UpdateDeviceLocation");

    std::string sql = "INSERT OR REPLACE INTO location (device_id, pos_x, pos_y, "
                      "pos_z) VALUES ('"
                      + device_id + "'," + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", "
                      + std::to_string(pos.z) + ");";
    console_->debug(sql);
    bool res = RunQuery(sql);

    console_->debug("- DataStore::UpdateDeviceLocation");
    return res;
}

bool DataStore::InsertRSSIReadings(const std::string&       device_id,
                                   std::vector<std::string> mac_address_list,
                                   std::vector<double>      rssi_list)
{
    console_->debug("+ DataStore::InsertRSSIReadings");

    if (mac_address_list.size() != rssi_list.size())
    {
        console_->error("Invalid mac address, rssi count");
        return false;
    }

    // Construct multi-record insert sql
    std::stringstream sql;
    sql << "INSERT INTO ";
    sql << "dev_" << device_id << " (";
    sql << "mac_addr, rssi) ";
    sql << "VALUES";
    for (int i = 0; i < mac_address_list.size(); ++i)
    {
        sql << "('" << mac_address_list[i] << "'," << rssi_list[i] << ")";
        if (i < (mac_address_list.size() - 1))
        {
            sql << ",";
        }
    }
    sql << ";";

    console_->debug(sql.str());
    bool res = RunQuery(sql.str());

    console_->debug("- DataStore::InsertRSSIReadings");
    return res;
}

bool DataStore::GetPosition(const std::string& id, QueryT query_by, Position& pos)
{
    console_->debug("+ DataStore::GetPosition");

    bool        result = false;
    std::string sql;

    if (query_by == QueryT::DEVICE)
        sql = "SELECT pos_x, pos_y, pos_z from location WHERE device_id=" + id;
    else
        sql = "SELECT pos_x, pos_y, pos_z from location WHERE employee_id=" + id;

    sqlite3_stmt* selectStmt;
    sqlite3_prepare(database_, sql.c_str(), static_cast<int>(sql.length() + 1), &selectStmt, NULL);
    while (1)
    {
        int state = sqlite3_step(selectStmt);
        if (state == SQLITE_ROW)
        {
            pos.x  = sqlite3_column_double(selectStmt, 0);
            pos.y  = sqlite3_column_double(selectStmt, 1);
            pos.z  = sqlite3_column_double(selectStmt, 2);
            result = true;
        }
        else if (state == SQLITE_DONE)
        {
            break;
        }
        else
        {
            console_->error("Failed to read from database");
            exit(1);
        }
    }
    sqlite3_finalize(selectStmt);

    console_->debug("- DataStore::GetPosition");
    return result;
}

bool DataStore::RunQuery(const std::string& sql)
{
    std::lock_guard<std::mutex> guard(database_lock_);
    char*                       error_msg;
    auto                        result = sqlite3_exec(database_, sql.c_str(), DbCallback, 0, &error_msg);
    if (result != SQLITE_OK)
    {
        console_->error("SQL error: {0}", error_msg);
        sqlite3_free(error_msg);
        return false;
    }
    else
    {
        console_->info("SQL executed successfully");
        return true;
    }
}

int DataStore::DbCallback(void* not_used, int argc, char** argv, char** azColName)
{
    std::shared_ptr<spdlog::logger> logger;
    logger->debug("+ DataStore::DbCallback");
    for (int i = 0; i < argc; i++)
    {
        logger->info("{0} = {1}", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    logger->debug("- DataStore::DbCallback");
    return 0;
}

} // namespace ins_service