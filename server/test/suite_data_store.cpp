
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "data_store.hpp"

namespace ins_service
{

class DataStoreFixture : public ::testing::Test
{
public:
    virtual void SetUp()
    {
    }

    sqlite3* GetDatabase()
    {
        return data_store_.database_;
    }

protected:
    DataStore data_store_;
};

TEST_F(DataStoreFixture, Init)
{
    data_store_.Init("database_file");
    sqlite3* db = GetDatabase();
    EXPECT_NE(db, nullptr);
}

} // namespace !ins_service