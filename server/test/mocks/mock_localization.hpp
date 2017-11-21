#ifndef INS_SERVER_TEST_MOCKS_LOCALIZATION_HPP
#define INS_SERVER_TEST_MOCKS_LOCALIZATION_HPP

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "localization.hpp"

namespace ins_service
{
class MockLocalization;

extern ::testing::NiceMock<MockLocalization>* g_mocked_localization_;

class MockLocalization : public Localization
{
public:
    MOCK_METHOD1(ProcessRSSIDataSet, Position(const std::string&));

    ~MockLocalization()
    {
        g_mocked_localization_ = nullptr;
    }
};
}

#endif // !INS_SERVER_TEST_MOCKS_LOCALIZATION_HPP