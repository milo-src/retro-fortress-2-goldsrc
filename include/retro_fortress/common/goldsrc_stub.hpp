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

struct client_data_t {
    float origin[3]{0.0F, 0.0F, 0.0F};
    float viewangles[3]{0.0F, 0.0F, 0.0F};
    float velocity[3]{0.0F, 0.0F, 0.0F};
    float punchangle[3]{0.0F, 0.0F, 0.0F};
    float fov{90.0F};
};

struct usercmd_s {
    std::uint16_t msec{0};
    float viewangles[3]{0.0F, 0.0F, 0.0F};
    float forwardmove{0.0F};
    float sidemove{0.0F};
    float upmove{0.0F};
    int buttons{0};
};

struct ref_params_t {
    float vieworg[3]{0.0F, 0.0F, 0.0F};
    float viewangles[3]{0.0F, 0.0F, 0.0F};
    float fov{90.0F};
};

struct cl_enginefunc_t {
    void (*pfnClientCmd)(const char*){nullptr};
    void (*pfnConsolePrint)(const char*){nullptr};
    void (*pfnAddCommand)(const char*, void (*)()){nullptr};
    float (*GetClientTime)(){nullptr};
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
void HUD_VidInit();
void HUD_Reset();
void HUD_Shutdown();
void HUD_Redraw(float time, int intermission);
int HUD_UpdateClientData(rf2::goldsrc::client_data_t* clientData, float flTime);
void HUD_Frame(double time);

void CL_CreateMove(float frameTime, rf2::goldsrc::usercmd_s* cmd, int active);
void V_CalcRefdef(rf2::goldsrc::ref_params_t* params);

}

