//
// Created by samueli on 2017-10-10.
//

#ifndef INS_SERVER_INS_INCLUDE_LOCALIZATION_HPP
#define INS_SERVER_INS_INCLUDE_LOCALIZATION_HPP

#include <spdlog/spdlog.h>
#include <string>
#include <vector>

#include "types.hpp"

namespace ins_service
{

#ifdef ENABLE_TESTS
class LocalizationFixture;
#endif // ENABLE_TESTS

class Localization
{
public:
#ifdef ENABLE_TESTS
    friend class LocalizationFixture;
#endif // ENABLE_TESTS

    explicit Localization()
        : console_(spdlog::get(LOGGER_NAME))
    {
        if (console_ == nullptr)
            console_ = spdlog::stdout_logger_mt(LOGGER_NAME);
    }

    Position
    ProcessRSSIDataSet(const std::string& device_id);

private:
    std::shared_ptr<spdlog::logger> console_;
};
}

#endif // INS_SERVER_INS_INCLUDE_LOCALIZATION_HPP