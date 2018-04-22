//
// Created by samueli on 2017-10-10.
//

#include "localization.hpp"
#include "data_store.hpp"

extern insNode_t * insNoderoot;

namespace ins_service {

wifiParams_t * Localization::FillNodeDataPoints(wifiParams_t * wifiNodeBlock,
		AccessPointRssiListPair mac_rssi_) {
	initKalmanParams(wifiNodeBlock);

	wifiNodeBlock->macAddress =
			const_cast<char *>(mac_rssi_.first.mac_addr.c_str());

	wifiNodeBlock->noSampleData = mac_rssi_.second.size();

	for (int i = 0; i < mac_rssi_.second.size(); ++i) {
		wifiNodeBlock->rssisampledata[(i % NUMBER_SAMPLES)] =
				(float) mac_rssi_.second[i];
	}

	loadLCFGParams(wifiNodeBlock);

	return wifiNodeBlock;
}

insNode_t * Localization::FillNodesDataPoints(const char * device_id,
		std::vector<AccessPointRssiListPair> mac_rssi_list) {
	insNode_t * insNode;

	if ((insNode = findWifiNode(insNoderoot, device_id)) == NULL) {
		insNode = createInsNodeListDevice(device_id); // check to make sure nodeblock exits!!
	}

	std::string msg = "Number of distinct mac addresses received from device: "
			+ std::string(device_id) + " is: "
			+ std::to_string(mac_rssi_list.size()) + " with: "
			+ std::to_string(mac_rssi_list[0].second.size()) + " data points.";
	console_->debug(msg);

	for (int i = 0; i < mac_rssi_list.size(); ++i) {
		FillNodeDataPoints(&insNode->wifiAccessPointNode[i], mac_rssi_list[i]);
	}
	return insNode;
}

Position Localization::ProcessRSSIDataSet(const std::string& device_id) {
	float * posit;
	const char * buff = device_id.c_str();
	Position pos;

	data_store_ = std::make_shared<DataStore>();

#ifdef ENABLE_TESTS
	data_store_->Init("db");
#else
	data_store_->Init("../ins.db");
#endif // ENABLE_TESTS
	console_->debug("+ Localization::ProcessRSSIDataSet");

	std::vector<AccessPoint> distn = data_store_->GetDistinctAccessPoints(
			device_id);

	if (distn.size() >= TRILATERAT_NUMBER_NODES) {
		std::vector<AccessPointRssiListPair> mac_rssi_list =
				data_store_->GetRSSISeriesData(device_id, distn);

		posit = GetCartesianPosition(FillNodesDataPoints(buff, mac_rssi_list));

		pos = {posit[0],posit[1],posit[2]};
	}
	else
	{
		console_->error("Not enough Access Points to process positioning!!");

		pos = {0.0f,0.0f,0.0f};
	}

	data_store_->Close();

	console_->debug("- Localization::ProcessRSSIDataSet");

	return pos;
}

} // namespace !ins_service

