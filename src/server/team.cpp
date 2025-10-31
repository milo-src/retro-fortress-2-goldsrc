#include "retro_fortress/server/team.hpp"

namespace rf2::server {

Team::Team(std::string identifier, std::string displayName, std::uint32_t colorHex)
    : m_identifier(std::move(identifier)),
      m_displayName(std::move(displayName)),
      m_colorHex(colorHex) {}

void Team::add_class(PlayerClass playerClass) {
    m_classes.emplace_back(std::move(playerClass));
}

}  // namespace rf2::server

