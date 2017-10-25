//
// Created by samueli on 2017-10-10.
//

#ifndef INS_SERVER_INS_DATA_STORE_HPP
#define INS_SERVER_INS_DATA_STORE_HPP

#include <spdlog/spdlog.h>
#include <sqlite3.h>
#include <iostream>
#include <vector>

#include "types.hpp"

namespace ins_service {

class DataStore {
   public:
    explicit DataStore() : database_(nullptr), console_(spdlog::stdout_logger_mt("DataStore")) {}

    void Init(const std::string &db_filename);

    void UpdateDeviceLocation(const std::string &device_id, Position pos);

    void InsertRSSIReadings(const std::string &device_id, std::vector<std::string> ssid_list,
                            std::vector<double> rssi_list);

    bool GetPosition(const std::string &device_id, QueryT queryby, Position &pos);

    void CreateDeviceTable(const std::string &device_id);

    void ClearDeviceTable(const std::string &device_id);

   private:
    void CreateLocationTable();

    void RunQuery(const std::string &sql);

    static int DbCallback(void *not_used, int argc, char **argv, char **azColName);  // Not used

    sqlite3 *database_;
    std::mutex database_lock_;
    std::shared_ptr<spdlog::logger> console_;
};

}  // namespace ins_service

#endif  // INS_SERVER_INS_DATA_STORE_HPP
