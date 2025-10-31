#pragma once

#include <cstdint>
#include <cstdio>

namespace rf2::goldsrc {

using qboolean = int;

struct globalvars_t {
    float time{0.0F};
};

struct edict_t {
    void* pvPrivateData{nullptr};
};

struct entvars_t {
    float health{100.0F};
};

struct enginefuncs_t {
    void (*pfnServerPrint)(const char*){nullptr};
};

struct DLL_FUNCTIONS {
    void (*pfnGameInit)(){nullptr};
};

struct NEW_DLL_FUNCTIONS {
    int (*pfnOnFreeEntPrivateData)(edict_t*){nullptr};
};

struct cl_enginefunc_t {
    void (*pfnClientCmd)(const char*){nullptr};
};

extern enginefuncs_t g_engfuncs;
extern globalvars_t* gpGlobals;

inline void ServerPrint(const char* message) {
    if (g_engfuncs.pfnServerPrint) {
        g_engfuncs.pfnServerPrint(message);
    } else {
        std::printf("[rf2] %s", message);
    }
}

}  // namespace rf2::goldsrc

extern "C" {

void GiveFnptrsToDll(rf2::goldsrc::enginefuncs_t* engineFunctions,
                     rf2::goldsrc::globalvars_t* globals);
int GetEntityAPI2(rf2::goldsrc::DLL_FUNCTIONS* functionTable, int* interfaceVersion);
int GetNewDLLFunctions(rf2::goldsrc::NEW_DLL_FUNCTIONS* functionTable, int* interfaceVersion);

int Initialize(rf2::goldsrc::cl_enginefunc_t* engineFunctions, int interfaceVersion);
void HUD_Init();

}

