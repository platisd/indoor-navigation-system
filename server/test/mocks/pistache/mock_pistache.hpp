#ifndef MOCK_PISTACHE_H
#define MOCK_PISTACHE_H

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <functional>
#include <string>
#include <vector>

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
    Port(uint16_t){}
};

class Ipv4
{
    Ipv4(uint8_t, uint8_t, uint8_t, uint8_t){}
    static Ipv4 any();
};

class Address
{
    Address(Ipv4, Port){}
};

namespace Tcp
{
enum class Options
{
    InstallSignalHandler = 0,
};
} // Tcp

namespace Http
{

enum class Code
{
    Ok = 0,
    Internal_Server_Error = 1,
};

class Cookie
{
public:
    Cookie(std::string n, std::string v) : name(n), value(v) {}
    std::string name, value;
};

class CookieJar
{
public:
    void add(Pistache::Http::Cookie);
};

class ResponseWriter
{
public:
    ResponseWriter(){}
    void send(Pistache::Http::Code);
    void send(Pistache::Http::Code,  std::string);
    CookieJar cookies() const
    {
        return CookieJar();
    }

};

class Endpoint
{
public:
    Endpoint(Pistache::Address){}
    class Options
    {
    public:
        Options& threads(int);
        Options& flags(Tcp::Options);
    };
    static Options options();
};
} // Http

namespace Rest
{

namespace details {
    template<typename T> struct LexicalCast {
        static T cast(const std::string& value) {
            std::istringstream iss(value);
            T out;
            if (!(iss >> out))
                throw std::runtime_error("Bad lexical cast");
            return out;
        }
    };

    template<>
    struct LexicalCast<std::string> {
        static std::string cast(const std::string& value) {
            return value;
        }
    };
} // details


class TypedParam
{
public:
    TypedParam(std::string p = "") : parameter_(p) {}
    template<typename T> T as() const
    {
        return details::LexicalCast<T>::cast(parameter_);
    }
private:
    std::string parameter_;
};

class Request
{
public:
    Request() {}
    bool hasParam(std::string name) const;
    TypedParam param(std::string p) const
    {
        return TypedParam(p);
    }

    std::vector<Pistache::Http::Cookie> cookies() const
    {
        return {Pistache::Http::Cookie("", "")};
    }
};

class Router
{
public:
    Router(){}
};

namespace Routes
{
namespace Route
{
class Handler
{
public:
    Handler(){}
};


enum class Result
{
    Ok = 0,
};

} // Route
    void Get(Router& router, std::string resource, Route::Handler handler);
    void Post(Router& router, std::string resource, Route::Handler handler);

    template<typename Result, typename Cls, typename... Args, typename Obj>
    Route::Handler bind(Result (Cls::*func)(Args...), Obj obj) {
        #define CALL_MEMBER_FN(obj, pmf)  ((obj)->*(pmf))

        return [=](const Rest::Request& request, Http::ResponseWriter response) {
            CALL_MEMBER_FN(obj, func)(request, std::move(response));
            // Create mock method RestCallback(std::function<void(Rest::Request&, Http::ResponseWriter)>)
            // And call it here
            return Route::Result::Ok;
        };

        #undef CALL_MEMBER_FN
    }

} // Routes
} // Rest
} // Pistache

#endif
