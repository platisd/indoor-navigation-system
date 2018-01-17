//
// Created by samueli on 2017-10-02.
//

#include "ins_service.hpp"
#include <csignal>

volatile sig_atomic_t is_server_running = 1;

void kill_server(int sig)
{
    is_server_running = 0;
}

int main(int argc, char* argv[])
{
    spdlog::set_level(spdlog::level::debug);

    Pistache::Port port(9080);
    int            thread_count = 2;

    if (argc >= 2)
    {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if (argc == 3)
            thread_count = static_cast<int>(std::stol(argv[2]));
    }

    Pistache::Address addr(Pistache::Ipv4::any(), port);
    auto              console = spdlog::stdout_logger_mt("main_console");

    console->info("Indoor Navigation System service setting up on {1}:{0:d}", addr.port(), addr.host());
    console->info("Cores = {0}", Pistache::hardware_concurrency());
    console->info("Using {0} threads", thread_count);

    ins_service::IndoorNavigationService ins;
    ins.Init(addr, thread_count);

    // Register abort & terminate signals respectively
    std::signal(SIGINT, kill_server);
    std::signal(SIGTERM, kill_server);

    // start server
    ins.Start();

    while (is_server_running)
        sleep(1);

    // shutdown server
    ins.Shutdown();
}