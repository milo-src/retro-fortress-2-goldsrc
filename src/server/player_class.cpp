#include "retro_fortress/server/player_class.hpp"

namespace rf2::server {

PlayerClass::PlayerClass(std::string identifier, std::string displayName, std::string role)
    : m_identifier(std::move(identifier)), m_displayName(std::move(displayName)), m_role(std::move(role)) {}

void PlayerClass::add_loadout_item(std::string item) {
    m_loadout.emplace_back(std::move(item));
}

}  // namespace rf2::server

