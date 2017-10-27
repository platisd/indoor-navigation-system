//
// Created by samueli on 2017-10-02.
//

#include "ins_service.hpp"

namespace ins_service
{

int IndoorNavigationService::Init(int thread_count)
{
    console_->debug("+ IndoorNavigationService::Init");

    data_store_ = std::make_shared<DataStore>();
    data_store_->Init("../ins.db");

    localization_ = std::make_shared<Localization>();

    auto opts
        = Pistache::Http::Endpoint::options().threads(thread_count).flags(Pistache::Tcp::Options::InstallSignalHandler);
    http_end_point_->init(opts);
    SetupRoutes();

    console_->debug("- IndoorNavigationService::Init");
    return 0;
}

void IndoorNavigationService::Start()
{
    console_->debug("+ IndoorNavigationService::Start");

    http_end_point_->setHandler(router_.handler());
    console_->info("Indoor Navigation Service now running ...");
    http_end_point_->serve();

    console_->debug("- IndoorNavigationService::Start");
}

void IndoorNavigationService::Shutdown()
{
    console_->debug("+ IndoorNavigationService::Shutdown");

    console_->info("Indoor Navigation Service is shutting down ...");
    http_end_point_->shutdown();

    console_->debug("- IndoorNavigationService::Shutdown");
}

void IndoorNavigationService::SetupRoutes()
{
    console_->debug("+ IndoorNavigationService::SetupRoutes");

    Pistache::Rest::Routes::Post(
        router_,
        "/set_rss/:device_id/:ssid1/:rssi1/:ssid2/:rssi2/:ssid3/:rssi3/:ssid4/:rssi4/:end?",
        Pistache::Rest::Routes::bind(&IndoorNavigationService::SetReceivedSignalStrengths, this));

    Pistache::Rest::Routes::Post(router_,
                                 "/reset_device_location/:device_id",
                                 Pistache::Rest::Routes::bind(&IndoorNavigationService::ResetDeviceLocation, this));

    Pistache::Rest::Routes::Get(router_,
                                "/get_device_position/:device_id",
                                Pistache::Rest::Routes::bind(&IndoorNavigationService::GetDevicePosition, this));

    Pistache::Rest::Routes::Get(router_,
                                "/get_employee_position/:employee_id",
                                Pistache::Rest::Routes::bind(&IndoorNavigationService::GetEmployeePosition, this));

    Pistache::Rest::Routes::Get(
        router_, "/ready", Pistache::Rest::Routes::bind(&IndoorNavigationService::HandleReady, this));

    Pistache::Rest::Routes::Get(router_, "/auth", Pistache::Rest::Routes::bind(&IndoorNavigationService::Auth, this));

    console_->debug("- IndoorNavigationService::SetupRoutes");
}

void IndoorNavigationService::SetReceivedSignalStrengths(const Pistache::Rest::Request& request,
                                                         Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::SetReceivedSignalStrengths");

    std::string device_id = request.param(":device_id").as<std::string>();

    std::vector<std::string> ssid_list = { request.param(":ssid1").as<std::string>(),
                                           request.param(":ssid2").as<std::string>(),
                                           request.param(":ssid3").as<std::string>(),
                                           request.param(":ssid4").as<std::string>() };

    std::vector<double> rssi_list = { request.param(":rssi1").as<double>(),
                                      request.param(":rssi1").as<double>(),
                                      request.param(":rssi1").as<double>(),
                                      request.param(":rssi1").as<double>() };

    console_->debug("ssids: {0}, {1}, {2}, {3}", ssid_list[0], ssid_list[1], ssid_list[2], ssid_list[3]);
    console_->debug(
        "rssis: {:03.3}, {:03.3}, {:03.3}, {:03.3}", rssi_list[0], rssi_list[1], rssi_list[2], rssi_list[3]);

    // @TODO Keep class aware of created device tables to avoid always calling this function.
    data_store_->CreateDeviceTable(device_id);
    data_store_->InsertRSSIReadings(device_id, ssid_list, rssi_list);

    if (request.hasParam(":end") && request.param(":end").as<std::string>() == "end")
    {
        Position pos = localization_->ProcessRSSIDataSet(device_id, ssid_list, rssi_list);
        data_store_->UpdateDeviceLocation(device_id, pos);
    }
    response.send(Pistache::Http::Code::Ok, "{result:success}");

    console_->debug("- IndoorNavigationService::SetReceivedSignalStrengths");
}

void IndoorNavigationService::ResetDeviceLocation(const Pistache::Rest::Request& request,
                                                  Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::ResetDeviceLocation");

    std::string device_id = request.param(":device_id").as<std::string>();
    data_store_->ClearDeviceTable(device_id);
    response.send(Pistache::Http::Code::Ok, "{result:success}");

    console_->debug("- IndoorNavigationService::ResetDeviceLocation");
}

void IndoorNavigationService::GetDevicePosition(const Pistache::Rest::Request& request,
                                                Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::GetDevicePosition");

    std::string device_id = request.param(":device_id").as<std::string>();

    Position pos;
    if (data_store_->GetPosition(device_id, QueryT::DEVICE, pos))
    {
        response.send(Pistache::Http::Code::Ok,
                      "device_id:" + device_id + " {loc_x:" + std::to_string(pos.x) + ",loc_y:" + std::to_string(pos.y)
                          + ",loc_z:"
                          + std::to_string(pos.z)
                          + "}");
        console_->info("X-{:03.3}, Y-{:03.3}, Z-{:03.3}, ", pos.x, pos.y, pos.z);
    }
    else
    {
        response.send(Pistache::Http::Code::Ok, "{error: device_id not found}");
    }

    console_->debug("- IndoorNavigationService::GetDevicePosition");
}

void IndoorNavigationService::GetEmployeePosition(const Pistache::Rest::Request& request,
                                                  Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::GetEmployeePosition");

    std::string employee_id = request.param(":employee_id").as<std::string>();

    Position pos;
    if (data_store_->GetPosition(employee_id, QueryT::EMPLOYEE, pos))
    {
        response.send(Pistache::Http::Code::Ok,
                      "employee_id:" + employee_id + " {loc_x:" + std::to_string(pos.x) + ",loc_y:"
                          + std::to_string(pos.y)
                          + ",loc_z:"
                          + std::to_string(pos.z)
                          + "}");
        console_->info("X-{:03.3}, Y-{:03.3}, Z-{:03.3}, ", pos.x, pos.y, pos.z);
    }
    else
    {
        response.send(Pistache::Http::Code::Ok, "{error: employee_id not found}");
    }

    console_->debug("- IndoorNavigationService::GetEmployeePosition");
}

void IndoorNavigationService::HandleReady(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::HandleReady");
    response.send(Pistache::Http::Code::Ok, "1");
    console_->debug("- IndoorNavigationService::HandleReady");
}

void IndoorNavigationService::Auth(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::Auth");
    // TODO
    PrintCookies(request);
    response.cookies().add(Pistache::Http::Cookie("lang", "en-US"));
    response.send(Pistache::Http::Code::Ok);
    console_->debug("- IndoorNavigationService::Auth");
}

void IndoorNavigationService::PrintCookies(const Pistache::Rest::Request& request)
{
    console_->debug("- IndoorNavigationService::PrintCookies");
    auto cookies = request.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto& c : cookies)
    {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
    console_->debug("- IndoorNavigationService::PrintCookies");
}

} // namespace ins_service
