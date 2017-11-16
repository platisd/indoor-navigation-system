#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "data_store.hpp"

namespace ins_service
{

class MockDataStore;
extern ::testing::NiceMock<MockDataStore>* g_mocked_data_store_;

class MockDataStore
{
public:
    MOCK_METHOD1(Init, void(std::string&));

    MOCK_METHOD0(Close, void());

    MOCK_METHOD2(UpdateDeviceLocation, bool(std::string&, Position));

    MOCK_METHOD2(AssignDeviceToEmployee, bool(const std::string&, const std::string&));

    MOCK_METHOD3(InsertRSSIReadings, bool(const std::string&, std::vector<std::string>, std::vector<double>));

    MOCK_METHOD3(GetPosition, bool(const std::string&, QueryT, Position&));

    MOCK_METHOD1(CreateDeviceTable, bool(const std::string&));

    MOCK_METHOD1(ClearDeviceTable, bool(const std::string&));

    ~MockDataStore()
    {
        g_mocked_data_store_ = nullptr;
    }
};
}