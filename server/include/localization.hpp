//
// Created by samueli on 2017-10-10.
//

#ifndef INS_SERVER_INS_INCLUDE_LOCALIZATION_HPP
#define INS_SERVER_INS_INCLUDE_LOCALIZATION_HPP

#include <spdlog/spdlog.h>

#include "types.hpp"
#include "data_store.hpp"

extern "C"
{
#include <WifiNode.h>
#include <WifiAccessPointLocalConfig.h>
}

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
	, data_store_(0)
	{
		if (console_ == nullptr)
			console_ = spdlog::stdout_logger_mt(LOGGER_NAME);
	}

	Position
	ProcessRSSIDataSet(const std::string& device_id);
	insNode_t * FillNodesDataPoints(const char * device_id, std::vector<AccessPointRssiListPair> mac_rssi_list);
	wifiParams_t * FillNodeDataPoints(wifiParams_t *  wifiNodeBlock, AccessPointRssiListPair mac_rssi_);

private:
	std::shared_ptr<spdlog::logger> console_;
	std::shared_ptr<DataStore> data_store_;
};
}

#endif // INS_SERVER_INS_INCLUDE_LOCALIZATION_HPP
