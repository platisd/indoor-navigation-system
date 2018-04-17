
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "data_store.hpp"
#include "localization.hpp"
#include "ins_service.hpp"
#include <stdio.h>

namespace ins_service
{

class LocalizationFixture : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        //spdlog::set_level(spdlog::level::debug);
    }

protected:
	Localization localization_;
    std::shared_ptr<DataStore> data_store_ = std::make_shared<DataStore>();
};

/*
 * TEST: Localize
 * EXPECT: <x,y,z> of dummy node.
*/
TEST_F(LocalizationFixture, RoundTripLocalizationTest)
{
	data_store_->Init("db");

    lcfg_initialize("../mocks/mock_WifiNodeLCFG.xml");

	double resolution = 1.0f;

	//FILL LCFG TUNING PARAMS

	std::string dev_name = "0501";
	Position dev_position{3.0,4.0,1.0};

	char wifi_node_1_ID[32] = "ff:01:ff:00:ff:ee";
	//float wifi_node_1_position[3] = {0.0,0.0,1.0};

	char wifi_node_2_ID[32] = "ff:02:ff:00:ff:ee";
	//float wifi_node_2_position[3] = {0.0,4.0,1.0};

	char wifi_node_3_ID[32] = "ff:03:ff:00:ff:ee";
	//float wifi_node_3_position[3] = {3.0,0.0,1.0};

	//wifi_1_ref_power = -10;
	//wifi_2_ref_power = -12;
	//wifi_3_ref_power = -15;

	//use simplified pathLoss equation with n = 1.5
	//wifi_1_2meter_receive_power = -14.515;
	//wifi_2_2meter_receive_power = -16.515;
	//wifi_3_2meter_receive_power = -19.515;

	bool ret = lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock1/macAddress",wifi_node_1_ID);
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock2/macAddress",wifi_node_2_ID);
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock3/macAddress",wifi_node_3_ID);

	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock1/_3DPosition/x", "0");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock2/_3DPosition/x", "0");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock3/_3DPosition/x", "3");

	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock1/_3DPosition/y", "0");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock2/_3DPosition/y", "4");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock3/_3DPosition/y", "0");

	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock1/_3DPosition/z", "1");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock2/_3DPosition/z", "1");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock3/_3DPosition/z", "1");

	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock1/powerAtArbitraryDistance","-14.515");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock2/powerAtArbitraryDistance","-16.515");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock3/powerAtArbitraryDistance","-19.515");

	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock1/arbitraryDistance","2.0");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock2/arbitraryDistance","2.0");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock3/arbitraryDistance","2.0");

	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock1/powerTransmit","-10");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock2/powerTransmit","-12");
	ret += lcfg_setStringParameter("/WifiNodes/wifiFloor1/wifiNodeBlock3/powerTransmit","-15");

	EXPECT_EQ(ret , false);

	//TRANSMIT RSSI AT CURRENT POSITION -> SERVER -> DBASE CALCULATED PL EQN
	float nodeReceivedPower_wifi1 = -20.485;
	float nodeReceivedPower_wifi2 = -19.157;
	float nodeReceivedPower_wifi3 = -24.031;

	//CHANNEL/INSNODE/PRACTICAL NOISE
	float noise_power = 0.01;

	EXPECT_EQ(data_store_->CreateDeviceTable(dev_name),1);

    for (uint32_t i = 0; i < 10; i++)
    {
    	std::string sql = "INSERT INTO dev_" + dev_name + " (mac_addr, rssi) VALUES"
    			"('"+ std::string(wifi_node_1_ID) +"',"+ std::to_string(nodeReceivedPower_wifi1 + (float)(noise_power*rand()/(float)RAND_MAX))+"),('"+ std::string(wifi_node_2_ID) +"',"+ std::to_string(nodeReceivedPower_wifi2 + (float)(noise_power*rand()/(float)RAND_MAX))+")"
    			",('"+ std::string(wifi_node_3_ID) +"',"+ std::to_string(nodeReceivedPower_wifi3 + (float)(noise_power*rand()/(float)RAND_MAX))+");";

    	data_store_->RunQuery(sql);
    	//    auto result = sqlite3_exec(database_, sql.c_str(), DbCallback, 0, &error_msg);
    }

    Position pos = localization_.ProcessRSSIDataSet(dev_name);
    data_store_->UpdateDeviceLocation(dev_name, pos);

    data_store_->GetPosition(dev_name, QueryT::DEVICE, pos);

    EXPECT_NEAR(pos.x, dev_position.x, resolution);
    EXPECT_NEAR(pos.y, dev_position.y, resolution);
    EXPECT_NEAR(pos.z, dev_position.z, resolution);

    data_store_->Close();
    std::remove("db");
}


} // namespace !ins_service

