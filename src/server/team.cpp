#include "retro_fortress/server/team.hpp"

#include <algorithm>

namespace rf2::server {

Team::Team(std::string identifier, std::string displayName, std::uint32_t colorHex)
    : m_identifier(std::move(identifier)),
      m_displayName(std::move(displayName)),
      m_colorHex(colorHex) {}

void Team::add_class(PlayerClass playerClass) {
    m_classes.emplace_back(std::move(playerClass));
}

PlayerClass* Team::find_class(const std::string& identifier) noexcept {
    const auto it = std::find_if(m_classes.begin(), m_classes.end(), [&identifier](const PlayerClass& playerClass) {
        return playerClass.identifier() == identifier;
    });

    if (it == m_classes.end()) {
        return nullptr;
    }

    return &(*it);
}

const PlayerClass* Team::find_class(const std::string& identifier) const noexcept {
    const auto it = std::find_if(m_classes.begin(), m_classes.end(), [&identifier](const PlayerClass& playerClass) {
        return playerClass.identifier() == identifier;
    });

    if (it == m_classes.end()) {
        return nullptr;
    }

    return &(*it);
}

}  // namespace rf2::server

