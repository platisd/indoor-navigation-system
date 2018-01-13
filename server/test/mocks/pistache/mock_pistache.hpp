#ifndef MOCK_PISTACHE_H
#define MOCK_PISTACHE_H

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class PistacheMock
{
public:

};

// Methods for instantiating and deleting the mock
static PistacheMock* pistacheMock = NULL;
PistacheMock*        pistacheMockInstance()
{
    if (!pistacheMock)
    {
        pistacheMock = new PistacheMock();
    }
    return pistacheMock;
}

void releasePistacheMock()
{
    if (pistacheMock)
    {
        delete pistacheMock;
        pistacheMock = NULL;
    }
}

#endif
