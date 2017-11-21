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
                                   std::vector<std::string> mac_addrs,
                                   std::vector<double>      rssis)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->InsertRSSIReadings(dev, mac_addrs, rssis);
}

bool DataStore::GetPosition(const std::string& dev, QueryT query, Position& pos)
{
    EXPECT_TRUE(g_mocked_data_store_ != nullptr);
    return g_mocked_data_store_->GetPosition(dev, query, pos);
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

} // ins_servvice