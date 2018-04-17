//
// Created by samueli on 2017-10-02.
//

#ifndef INS_SERVER_INS_INCLUDE_SERVICE_HPP
#define INS_SERVER_INS_INCLUDE_SERVICE_HPP

#include <memory>
#include <mutex>
#include <spdlog/spdlog.h>

#include "lib_wrapper.hpp"
#include "data_store.hpp"
#include "localization.hpp"
#include "types.hpp"
extern "C"
{
#include <WifiNode.h>
#include <WifiAccessPointLocalConfig.h>
}

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

    explicit IndoorNavigationService()
        : http_end_point_(nullptr)
        , data_store_(nullptr)
        , localization_(nullptr)
        , console_(spdlog::get(LOGGER_NAME))
    {
        if (console_ == nullptr)
            console_ = spdlog::stdout_logger_mt(LOGGER_NAME);
    }

    int Init(Pistache::Address addr, int thread_count = 2);

    void Start();

    void Shutdown();

private:
    void SetupRoutes();

    void SetReceivedSignalStrengths(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void ResolveDevicePosition(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void ResetDevicePosition(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

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
