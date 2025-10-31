#include "retro_fortress/server/game_manager.hpp"
#include "retro_fortress/server/logging.hpp"

#include "retro_fortress/common/goldsrc_stub.hpp"

namespace rf2::server {

namespace {

void GameInit() {
    log_info("Retro Fortress 2 server initializing game manager");
    GameManager::instance().initialize_default_rules();
}

int OnFreeEntPrivateData(rf2::goldsrc::edict_t* entity) {
    (void)entity;
    log_info("Entity private data freed");
    return 1;
}

}  // namespace

}  // namespace rf2::server

extern "C" {

void GiveFnptrsToDll(rf2::goldsrc::enginefuncs_t* engineFunctions,
                     rf2::goldsrc::globalvars_t* globals) {
    rf2::goldsrc::g_engfuncs = engineFunctions ? *engineFunctions : rf2::goldsrc::enginefuncs_t{};
    rf2::goldsrc::gpGlobals = globals;
    rf2::server::log_info("Retro Fortress 2 server DLL attached to engine");
}

int GetEntityAPI2(rf2::goldsrc::DLL_FUNCTIONS* functionTable, int* interfaceVersion) {
    if (!functionTable) {
        return 0;
    }

    functionTable->pfnGameInit = &rf2::server::GameInit;

    if (interfaceVersion) {
        *interfaceVersion = 140;
    }

    return 1;
}

int GetNewDLLFunctions(rf2::goldsrc::NEW_DLL_FUNCTIONS* functionTable, int* interfaceVersion) {
    if (!functionTable) {
        return 0;
    }

    functionTable->pfnOnFreeEntPrivateData = &rf2::server::OnFreeEntPrivateData;

    if (interfaceVersion) {
        *interfaceVersion = 1;
    }

    return 1;
}

}

