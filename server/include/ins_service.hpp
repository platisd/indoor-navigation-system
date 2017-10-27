//
// Created by samueli on 2017-10-02.
//

#ifndef INS_SERVER_INS_INCLUDE_SERVICE_HPP
#define INS_SERVER_INS_INCLUDE_SERVICE_HPP

#include <memory>
#include <mutex>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <spdlog/spdlog.h>

#include "data_store.hpp"
#include "localization.hpp"
#include "types.hpp"

namespace ins_service
{

#ifdef ENABLE_TESTS
class IndoorNavigationServiceFixture;
#endif // ENABLE_TESTS

class IndoorNavigationService
{
public:
#ifdef ENABLE_TESTS
    friend class IndoorNavigationServiceFixture;
#endif // ENABLE_TESTS

    explicit IndoorNavigationService(Pistache::Address addr)
        : http_end_point_(std::make_shared<Pistache::Http::Endpoint>(addr))
        , data_store_(nullptr)
        , localization_(nullptr)
        , console_(spdlog::get(LOGGER_NAME))
    {
        if (console_ == nullptr)
            console_ = spdlog::stdout_logger_mt(LOGGER_NAME);
    }

    int Init(int thread_count = 2);

    void Start();

    void Shutdown();

private:
    void SetupRoutes();

    void SetReceivedSignalStrengths(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void ResetDeviceLocation(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void GetDevicePosition(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void GetEmployeePosition(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void Auth(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void HandleReady(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void PrintCookies(const Pistache::Rest::Request& request);

    std::shared_ptr<Pistache::Http::Endpoint> http_end_point_;
    std::shared_ptr<DataStore>                data_store_;
    std::shared_ptr<Localization>             localization_;
    Pistache::Rest::Router                    router_;
    std::shared_ptr<spdlog::logger>           console_;
};

} // namespace ins_service

#endif // INS_SERVER_INS_INCLUDE_SERVICE_HPP
