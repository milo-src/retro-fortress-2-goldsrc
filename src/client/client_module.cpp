#include "retro_fortress/common/goldsrc_stub.hpp"

#include <string>

namespace rf2::client {

namespace detail {

inline rf2::goldsrc::cl_enginefunc_t* g_engineFunctions = nullptr;

void announce(const std::string& message) {
    if (g_engineFunctions && g_engineFunctions->pfnClientCmd) {
        g_engineFunctions->pfnClientCmd(message.c_str());
    } else {
        rf2::goldsrc::ServerPrint(("[client] " + message + "\n").c_str());
    }
}

}  // namespace detail

void bind_engine(rf2::goldsrc::cl_enginefunc_t* engineFunctions) {
    detail::g_engineFunctions = engineFunctions;
}

void on_initialize() {
    detail::announce("Retro Fortress 2 client initialized");
}

void on_hud_init() {
    detail::announce("Retro Fortress 2 HUD bootstrapped");
}

}  // namespace rf2::client

extern "C" {

int Initialize(rf2::goldsrc::cl_enginefunc_t* engineFunctions, int interfaceVersion) {
    (void)interfaceVersion;

    rf2::client::bind_engine(engineFunctions);
    rf2::client::on_initialize();
    return 1;
}

void HUD_Init() {
    rf2::client::on_hud_init();
}

}

