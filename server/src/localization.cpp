//
// Created by samueli on 2017-10-10.
//

#include "localization.hpp"

namespace ins_service
{

Position Localization::ProcessRSSIDataSet(const std::string&       device_id,
                                          std::vector<std::string> ssid_list,
                                          std::vector<double>      rssi_list)
{
    console_->debug("+ Localization::ProcessRSSIDataSet");

    Position pos{ 3.2, 5, 11 };
    console_->debug("- Localization::ProcessRSSIDataSet");
    return pos;
}

} // namespace !ins_service
