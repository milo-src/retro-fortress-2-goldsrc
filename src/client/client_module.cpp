#include "retro_fortress/common/goldsrc_stub.hpp"
#include "retro_fortress/common/class_profiles.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace rf2::client {

namespace detail {

struct PlayerScore {
    std::string name;
    std::string className;
    int teamIndex{0};
    int score{0};
    int deaths{0};
    int ping{0};
    bool isLocal{false};
};

struct TeamInfo {
    std::string shortName;
    std::string fullName;
    std::uint32_t colorHex{0};
    int totalScore{0};
    int playerCount{0};
};

struct ClientContext {
    rf2::goldsrc::cl_enginefunc_t* engine{nullptr};
    std::array<TeamInfo, 2> teams{TeamInfo{"RED", "Reliable Excavation Demolition", 0xB8383B},
                                  TeamInfo{"BLU", "Builders League United", 0x5885A2}};
    std::vector<PlayerScore> scoreboard{};
    std::vector<rf2::common::ClassProfile> classProfiles{};
    std::array<float, 3> viewAngles{0.0F, 0.0F, 0.0F};
    std::array<float, 3> viewOrigin{0.0F, 0.0F, 64.0F};
    std::array<float, 3> viewVelocity{0.0F, 0.0F, 0.0F};
    float lastRedrawTime{0.0F};
    float lastScoreboardPrintTime{0.0F};
    float fov{90.0F};
    bool hudReady{false};
    bool commandsRegistered{false};
    std::size_t localClassIndex{0};
    std::vector<std::string> classRotation{};
};

ClientContext& context() {
    static ClientContext ctx;
    return ctx;
}

const rf2::common::ClassProfile* profile_for_index(std::size_t index) {
    auto& ctx = context();
    if (ctx.classProfiles.empty() || index >= ctx.classProfiles.size()) {
        return nullptr;
    }
    return &ctx.classProfiles[index];
}

const rf2::common::ClassProfile* current_profile() {
    return profile_for_index(context().localClassIndex);
}

void print_line(std::string_view line);

void print_class_profile_details(const rf2::common::ClassProfile& profile) {
    std::ostringstream oss;
    oss << "Class: " << profile.display_name << " [" << profile.identifier << "] | Role: "
        << profile.role;
    print_line(oss.str());

    if (profile.loadout.empty()) {
        print_line("  Loadout: (empty)");
        return;
    }

    print_line("  Loadout:");
    for (const auto& item : profile.loadout) {
        print_line("    - " + item);
    }
}

void print_line(std::string_view line) {
    auto& ctx = context();
    std::string buffer(line);
    buffer.push_back('\n');

    if (ctx.engine && ctx.engine->pfnConsolePrint) {
        ctx.engine->pfnConsolePrint(buffer.c_str());
    } else {
        rf2::goldsrc::ServerPrint(buffer.c_str());
    }
}

void announce(std::string_view message) {
    print_line(std::string{"[client] "} + std::string{message});
}

void recalculate_team_totals() {
    auto& ctx = context();
    for (auto& team : ctx.teams) {
        team.totalScore = 0;
        team.playerCount = 0;
    }

    for (const auto& player : ctx.scoreboard) {
        if (player.teamIndex >= 0 && static_cast<std::size_t>(player.teamIndex) < ctx.teams.size()) {
            auto& team = ctx.teams[static_cast<std::size_t>(player.teamIndex)];
            team.totalScore += player.score;
            team.playerCount += 1;
        }
    }
}

std::string format_team_summary(const TeamInfo& team) {
    std::ostringstream oss;
    oss << team.shortName << " | " << team.fullName << " | score " << team.totalScore
        << " | players " << team.playerCount;
    return oss.str();
}

std::string format_player_entry(const PlayerScore& player) {
    std::ostringstream oss;
    oss << std::left << std::setw(18) << player.name << std::setw(10) << player.className
        << std::right << std::setw(5) << player.score << std::setw(7) << player.deaths
        << std::setw(7) << player.ping;
    if (player.isLocal) {
        oss << "  <- you";
    }
    return oss.str();
}

void print_scoreboard() {
    auto& ctx = context();
    recalculate_team_totals();

    print_line("=== Retro Fortress 2 Scoreboard ===");
    for (const auto& team : ctx.teams) {
        print_line(format_team_summary(team));
    }
    print_line("-------------------------------");
    print_line("Player             Class         Score Deaths   Ping");

    auto sortedPlayers = ctx.scoreboard;
    std::stable_sort(sortedPlayers.begin(), sortedPlayers.end(), [](const PlayerScore& lhs, const PlayerScore& rhs) {
        if (lhs.teamIndex != rhs.teamIndex) {
            return lhs.teamIndex < rhs.teamIndex;
        }
        if (lhs.score != rhs.score) {
            return lhs.score > rhs.score;
        }
        return lhs.deaths < rhs.deaths;
    });

    for (const auto& player : sortedPlayers) {
        print_line(format_player_entry(player));
    }
}

void ensure_sample_scoreboard() {
    auto& ctx = context();
    ctx.scoreboard.clear();
    const auto rotationSize = ctx.classRotation.size();
    const auto classNameForIndex = [&](std::size_t index) -> std::string {
        if (rotationSize == 0) {
            return "Unknown";
        }
        return ctx.classRotation[index % rotationSize];
    };

    ctx.scoreboard.emplace_back(
        PlayerScore{"Mercenary", classNameForIndex(ctx.localClassIndex), 0, 5, 2, 45, true});
    ctx.scoreboard.emplace_back(PlayerScore{"BlastMaster", classNameForIndex(1), 0, 3, 4, 80, false});
    ctx.scoreboard.emplace_back(PlayerScore{"PyroMania", classNameForIndex(2), 0, 2, 3, 60, false});
    ctx.scoreboard.emplace_back(PlayerScore{"StickyJim", classNameForIndex(3), 1, 7, 5, 65, false});
    ctx.scoreboard.emplace_back(PlayerScore{"SteelWall", classNameForIndex(4), 1, 6, 4, 90, false});
    ctx.scoreboard.emplace_back(PlayerScore{"Gearhead", classNameForIndex(5), 1, 4, 6, 55, false});
    recalculate_team_totals();
}

void update_local_class_rotation() {
    auto& ctx = context();
    auto& rotation = ctx.classRotation;
    if (rotation.empty()) {
        return;
    }

    ctx.localClassIndex = (ctx.localClassIndex + 1) % rotation.size();
    const auto& nextClass = rotation[ctx.localClassIndex];
    announce("Selected class: " + nextClass);
    if (const auto* profile = current_profile()) {
        print_class_profile_details(*profile);
    }

    for (auto& player : ctx.scoreboard) {
        if (player.isLocal) {
            player.className = nextClass;
            return;
        }
    }
}

void command_print_scoreboard() {
    print_scoreboard();
}

void command_cycle_class() {
    update_local_class_rotation();
}

void command_show_class_info() {
    const auto* profile = current_profile();
    if (!profile) {
        announce("No class data available");
        return;
    }

    print_class_profile_details(*profile);
}

void register_commands() {
    auto& ctx = context();
    if (ctx.commandsRegistered || !ctx.engine || !ctx.engine->pfnAddCommand) {
        return;
    }

    ctx.engine->pfnAddCommand("rf2_scoreboard", &command_print_scoreboard);
    ctx.engine->pfnAddCommand("rf2_cycle_class", &command_cycle_class);
    ctx.engine->pfnAddCommand("rf2_class_info", &command_show_class_info);
    ctx.commandsRegistered = true;
}

}  // namespace detail

void bind_engine(rf2::goldsrc::cl_enginefunc_t* engineFunctions) {
    detail::context().engine = engineFunctions;
}

void on_initialize() {
    auto& ctx = detail::context();
    const auto& profiles = rf2::common::default_class_profiles();
    ctx.classProfiles.assign(profiles.begin(), profiles.end());
    ctx.classRotation.clear();
    ctx.classRotation.reserve(ctx.classProfiles.size());
    for (const auto& profile : ctx.classProfiles) {
        ctx.classRotation.push_back(profile.display_name);
    }
    ctx.localClassIndex = 0;
    ctx.fov = 90.0F;
    ctx.viewAngles = {0.0F, 0.0F, 0.0F};
    ctx.viewVelocity = {0.0F, 0.0F, 0.0F};
    ctx.viewOrigin = {0.0F, 0.0F, 64.0F};

    detail::announce("Retro Fortress 2 client initialized");
    if (const auto* profile = detail::current_profile()) {
        detail::print_class_profile_details(*profile);
    }
    detail::register_commands();
}

void on_hud_init() {
    auto& ctx = detail::context();
    ctx.hudReady = true;
    ctx.lastRedrawTime = 0.0F;
    detail::register_commands();
    detail::announce("Retro Fortress 2 HUD bootstrapped");
}

void on_vid_init() {
    detail::ensure_sample_scoreboard();
    detail::announce("HUD video resources configured");
}

void on_hud_redraw(float time, bool intermission) {
    auto& ctx = detail::context();
    ctx.lastRedrawTime = time;

    if (!ctx.hudReady) {
        return;
    }

    if (!intermission && time - ctx.lastScoreboardPrintTime > 5.0F) {
        detail::print_scoreboard();
        ctx.lastScoreboardPrintTime = time;
    }
}

int on_update_client_data(rf2::goldsrc::client_data_t* clientData, float frameTime) {
    auto& ctx = detail::context();
    if (!clientData) {
        return 0;
    }

    ctx.fov = std::clamp(ctx.fov, 70.0F, 110.0F);

    for (std::size_t i = 0; i < ctx.viewAngles.size(); ++i) {
        clientData->viewangles[i] = ctx.viewAngles[i];
        clientData->velocity[i] = ctx.viewVelocity[i];
        clientData->origin[i] = ctx.viewOrigin[i];
        clientData->punchangle[i] = 0.0F;
    }
    clientData->fov = ctx.fov;

    ctx.viewOrigin[0] += ctx.viewVelocity[0] * frameTime;
    ctx.viewOrigin[1] += ctx.viewVelocity[1] * frameTime;
    ctx.viewOrigin[2] += ctx.viewVelocity[2] * frameTime;

    return 1;
}

void on_reset() {
    auto& ctx = detail::context();
    ctx.hudReady = false;
    ctx.lastRedrawTime = 0.0F;
    ctx.lastScoreboardPrintTime = 0.0F;
    detail::announce("HUD reset");
}

void on_shutdown() {
    detail::announce("Retro Fortress 2 client shutting down");
}

void on_frame(double hostTime) {
    auto& ctx = detail::context();
    if (ctx.engine && ctx.engine->GetClientTime) {
        const float engineTime = ctx.engine->GetClientTime();
        ctx.viewAngles[1] = std::fmod(engineTime * 10.0F, 360.0F);
    }

    if (!ctx.scoreboard.empty()) {
        auto& localPlayer = ctx.scoreboard.front();
        if (localPlayer.isLocal) {
            localPlayer.ping = static_cast<int>(hostTime * 10.0) % 200;
        }
    }
}

void on_create_move(float frameTime, rf2::goldsrc::usercmd_s* cmd, bool isActive) {
    auto& ctx = detail::context();
    if (!cmd || !isActive) {
        return;
    }

    cmd->msec = static_cast<std::uint16_t>(frameTime * 1000.0F);
    for (std::size_t i = 0; i < ctx.viewAngles.size(); ++i) {
        cmd->viewangles[i] = ctx.viewAngles[i];
    }
    cmd->forwardmove = 0.0F;
    cmd->sidemove = 0.0F;
    cmd->upmove = 0.0F;
    cmd->buttons = 0;
}

void on_calc_refdef(rf2::goldsrc::ref_params_t* params) {
    auto& ctx = detail::context();
    if (!params) {
        return;
    }

    for (std::size_t i = 0; i < ctx.viewAngles.size(); ++i) {
        params->viewangles[i] = ctx.viewAngles[i];
        params->vieworg[i] = ctx.viewOrigin[i];
    }
    params->fov = ctx.fov;
}

}  // namespace rf2::client

extern "C" {

// The exported entry points follow the Half-Life SDK client DLL interface
// described in cl_dll/cdll_int.cpp. They allow the GoldSrc engine to drive
// the higher level Retro Fortress 2 client systems.

int Initialize(rf2::goldsrc::cl_enginefunc_t* engineFunctions, int interfaceVersion) {
    (void)interfaceVersion;

    rf2::client::bind_engine(engineFunctions);
    rf2::client::on_initialize();
    return 1;
}

void HUD_Init() {
    rf2::client::on_hud_init();
}

void HUD_VidInit() {
    rf2::client::on_vid_init();
}

void HUD_Reset() {
    rf2::client::on_reset();
}

void HUD_Shutdown() {
    rf2::client::on_shutdown();
}

void HUD_Redraw(float time, int intermission) {
    rf2::client::on_hud_redraw(time, intermission != 0);
}

int HUD_UpdateClientData(rf2::goldsrc::client_data_t* clientData, float flTime) {
    return rf2::client::on_update_client_data(clientData, flTime);
}

void HUD_Frame(double time) {
    rf2::client::on_frame(time);
}

void CL_CreateMove(float frameTime, rf2::goldsrc::usercmd_s* cmd, int active) {
    rf2::client::on_create_move(frameTime, cmd, active != 0);
}

void V_CalcRefdef(rf2::goldsrc::ref_params_t* params) {
    rf2::client::on_calc_refdef(params);
}

}

