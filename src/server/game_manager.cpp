#include "retro_fortress/server/game_manager.hpp"

#include <sstream>

#include "retro_fortress/common/class_profiles.hpp"
#include "retro_fortress/server/logging.hpp"

namespace rf2::server {

GameManager& GameManager::instance() {
    static GameManager manager;
    return manager;
}

void GameManager::initialize_default_rules() {
    m_teams.clear();

    Team red("red", "Reliable Excavation Demolition", 0xB8383B);
    Team blu("blu", "Builders League United", 0x5885A2);

    const auto& classProfiles = rf2::common::default_class_profiles();

    const auto populateTeam = [&classProfiles](Team& team) {
        for (const auto& profile : classProfiles) {
            PlayerClass playerClass(profile.identifier, profile.display_name, profile.role);
            for (const auto& loadoutItem : profile.loadout) {
                playerClass.add_loadout_item(loadoutItem);
            }
            team.add_class(playerClass);
        }
    };

    populateTeam(red);
    populateTeam(blu);

    m_teams.push_back(red);
    m_teams.push_back(blu);

    std::ostringstream summary;
    summary << "Retro Fortress 2 default rules initialized (" << m_teams.size() << " teams, "
            << classProfiles.size() << " classes)";
    log_info(summary.str());
}

}  // namespace rf2::server

