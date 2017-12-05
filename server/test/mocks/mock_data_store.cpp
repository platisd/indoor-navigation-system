#include "mock_data_store.hpp"

namespace ins_service
{

::testing::NiceMock<MockDataStore>* g_mocked_data_store_;

void DataStore::Init(const std::string& filename)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->Init(filename);
}

void DataStore::Close()
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->Close();
}

bool DataStore::UpdateDeviceLocation(const std::string& dev, Position pos)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->UpdateDeviceLocation(dev, pos);
}

bool DataStore::AssignDeviceToEmployee(const std::string& dev, const std::string& employee)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->AssignDeviceToEmployee(dev, employee);
}

bool DataStore::InsertRSSIReadings(const std::string&       dev,
                                   std::vector<AccessPointRssiPair> accesspoint_rssi_pair)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->InsertRSSIReadings(dev, accesspoint_rssi_pair);
}

bool DataStore::GetPosition(const std::string& id, QueryT query, Position& pos)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->GetPosition(id, query, pos);
}

bool DataStore::CreateDeviceTable(const std::string& dev)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->CreateDeviceTable(dev);
}

bool DataStore::ClearDeviceTable(const std::string& dev)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->ClearDeviceTable(dev);
}

std::vector<AccessPoint> DataStore::GetDistinctAccessPoints(const std::string& dev)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->GetDistinctAccessPoints(dev);
}

std::vector<int32_t> DataStore::GetRSSISeriesData(const std::string& device_id, AccessPoint access_point)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->GetRSSISeriesData(device_id, access_point);
}

std::vector<AccessPointRssiListPair> DataStore::GetRSSISeriesData(const std::string& device_id, std::vector<AccessPoint> access_points)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->GetRSSISeriesData(device_id, access_points);
}
} // ins_servvice