//
// Created by samueli on 2017-10-10.
//

#ifndef INS_SERVER_INS_LOCALIZATION_HPP
#define INS_SERVER_INS_LOCALIZATION_HPP

#include <spdlog/spdlog.h>
#include <string>
#include <vector>

#include "types.hpp"

namespace ins_service {

class Localization {
   public:
    explicit Localization() : console_(spdlog::stdout_logger_mt("Localization")) {}

    Position ProcessRSSIDataSet(const std::string &device_id, std::vector<std::string> ssid_list,
                     std::vector<double> rssi_list);

   private:
    std::shared_ptr<spdlog::logger> console_;
};
}

#endif  // INS_SERVER_INS_LOCALIZATION_HPP