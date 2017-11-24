#include "mock_localization.hpp"

namespace ins_service
{

::testing::NiceMock<MockLocalization>* g_mocked_localization_;

Position Localization::ProcessRSSIDataSet(const std::string& device_id)
{
    EXPECT_TRUE(g_mocked_localization_ != nullptr);
    return g_mocked_localization_->ProcessRSSIDataSet(device_id);
}

} // ins_service