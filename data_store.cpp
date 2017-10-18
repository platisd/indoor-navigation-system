//
// Created by samueli on 2017-10-10.
//

#include "data_store.hpp"

namespace ins_service {

void DataStore::Init(const std::string &db_filename) {
    console_->debug("+ DataStore::Init");

    int result = sqlite3_open(db_filename.c_str(), &database_);
    if (result != 0) {
        console_->error("Can't open database: {0}", sqlite3_errmsg(database_));
        return;
    }
    console_->info("Opened database successfully");
    CreateLocationTable();

    console_->debug("- DataStore::Init");
    return;
}

void DataStore::CreateLocationTable() {
    console_->debug("+ DataStore::CreateLocationTable");

    std::string sql =
        "CREATE TABLE IF NOT EXISTS location(device_id INTEGER PRIMARY KEY,"
        "employee_id TEXT,"
        "loc_x REAL,"
        "loc_y REAL,"
        "loc_z REAL,"
        "timestamp datatime default current_timestamp);";
    console_->debug(sql);
    RunQuery(sql);

    console_->debug("- DataStore::CreateLocationTable");
}

void DataStore::CreateDeviceTable(const std::string &device_id) {
    console_->debug("+ DataStore::CreateDeviceTable");

    std::string sql = "CREATE TABLE IF NOT EXISTS dev_" + device_id +
                      "(id INTEGER PRIMARY KEY,"
                      "ssid1 TEXT, rss1 REAL,"
                      "ssid2 TEXT, rss2 REAL,"
                      "ssid3 TEXT, rss3 REAL,"
                      "ssid4 TEXT, rss4 REAL,"
                      "ssid5 TEXT, rss5 REAL,"
                      "timestamp datatime default current_timestamp);";
    console_->debug(sql);
    RunQuery(sql);

    console_->debug("- DataStore::CreateDeviceTable");
}

void DataStore::ClearDeviceTable(const std::string &device_id) {
    console_->debug("+ DataStore::ClearDeviceTable");

    std::string sql = "DELETE from dev_" + device_id;
    console_->info(sql);
    RunQuery(sql);

    console_->debug("- DataStore::ClearDeviceTable");
}

void DataStore::UpdateDeviceLocation(const std::string &device_id, Position pos) {
    console_->debug("+ DataStore::UpdateDeviceLocation");

    std::string sql =
        "INSERT OR REPLACE INTO location (device_id, employee_id, loc_x, loc_y, "
        "loc_z) VALUES ('" +
        device_id + "', 'employee_id', " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " +
        std::to_string(pos.z) + ");";
    console_->debug(sql);
    RunQuery(sql);

    console_->debug("- DataStore::UpdateDeviceLocation");
}

void DataStore::InsertRSSIReadings(const std::string &device_id, std::vector<std::string> ssid_list,
                                   std::vector<double> rssi_list) {
    console_->debug("+ DataStore::InsertRSSIReadings");

    std::string sql = "INSERT INTO dev_" + device_id +
                      " (ssid1, ssid2, ssid3, ssid4, rss1, rss2, rss3, rss4) "
                      "VALUES ('" +
                      ssid_list[0] + "','" + ssid_list[1] + "','" + ssid_list[2] + "','" + ssid_list[3] + "'," +
                      std::to_string(rssi_list[0]) + "," + std::to_string(rssi_list[1]) + "," +
                      std::to_string(rssi_list[2]) + "," + std::to_string(rssi_list[3]) + ");";
    console_->debug(sql);
    RunQuery(sql);

    console_->debug("- DataStore::InsertRSSIReadings");
}

Position DataStore::GetPosition(const std::string &id, QueryT query_by) {
    console_->debug("+ DataStore::GetPosition");

    std::string sql;

    if (query_by == QueryT::DEVICE)
        sql = "SELECT loc_x, loc_y, loc_z from location WHERE device_id=" + id;
    else
        sql = "SELECT loc_x, loc_y, loc_z from location WHERE employee_id=" + id;

    Position pos;
    sqlite3_stmt *selectStmt;
    sqlite3_prepare(database_, sql.c_str(), static_cast<int>(sql.length() + 1), &selectStmt, NULL);
    while (1) {
        int state = sqlite3_step(selectStmt);
        if (state == SQLITE_ROW) {
            pos.x = sqlite3_column_double(selectStmt, 0);
            pos.y = sqlite3_column_double(selectStmt, 1);
            pos.z = sqlite3_column_double(selectStmt, 2);
        } else if (state == SQLITE_DONE) {
            break;
        } else {
            console_->error("Failed to read from database");
            exit(1);
        }
    }
    sqlite3_finalize(selectStmt);

    console_->debug("- DataStore::GetPosition");
    return pos;
}

void DataStore::RunQuery(const std::string &sql) {
    std::lock_guard<std::mutex> guard(database_lock_);
    char *error_msg;
    auto result = sqlite3_exec(database_, sql.c_str(), DbCallback, 0, &error_msg);
    if (result != SQLITE_OK) {
        console_->error("SQL error: {0}", error_msg);
        sqlite3_free(error_msg);
    } else {
        console_->info("SQL executed successfully");
    }
}

int DataStore::DbCallback(void *not_used, int argc, char **argv, char **azColName) {
    std::shared_ptr<spdlog::logger> logger;
    logger->debug("+ DataStore::DbCallback");
    for (int i = 0; i < argc; i++) {
        logger->info("{0} = {1}", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    logger->debug("- DataStore::DbCallback");
    return 0;
}
}  // namespace ins_service