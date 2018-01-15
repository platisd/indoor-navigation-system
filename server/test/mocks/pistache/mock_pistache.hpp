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

namespace Pistache
{
int hardware_concurrency()
{
    return 0;
}

class Port
{
public:
    Port(uint16_t) = default;
};

class Ipv4
{
    Ipv4(uint8_t, uint8_t, uint8_t, uint8_t) = default;
    static Ipv4 any();
};

class Address
{
    Address(Ipv4, Port) = default;
};

namespace Tcp
{
enum class Options
{
    InstallSignalHandler = 0
};
} // Tcp

namespace Rest
{
class Request
{
public:
    Request() = default;
};

class Router
{
public:
    Router() = default;
};

namespace Routes
{
namespace Route
{
class Handler
{
public:
    Handler() = default;
};
} // Route
    void Get(Router& router, std::string resource, Route::Handler handler);
    void Post(Router& router, std::string resource, Route::Handler handler);
    // template<typename Result, typename... Args>
    // Route::Handler bind(Result (*func)(Args...)) {
    // }
} // Routes
} // Rest

namespace Http
{
class ResponseWriter
{
public:
    ResponseWriter() = default;
};

class Endpoint
{
public:
    Endpoint() = default;
    class Options
    {
        Options& threads(int);
        Options& flags(Tcp::Options);
    };
    static Options options();
};

enum class Code
{
    Ok = 0
    Internal_Server_Error = 1
};

class Cookie
{
    Cookie(std::string, std::string) = default;
};
} // Http

} // Pistache

#endif
