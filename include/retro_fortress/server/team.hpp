#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "retro_fortress/server/player_class.hpp"

namespace rf2::server {

class Team {
public:
    Team(std::string identifier, std::string displayName, std::uint32_t colorHex);

    const std::string& identifier() const noexcept { return m_identifier; }
    const std::string& display_name() const noexcept { return m_displayName; }
    std::uint32_t color() const noexcept { return m_colorHex; }

    void add_class(PlayerClass playerClass);
    const std::vector<PlayerClass>& classes() const noexcept { return m_classes; }

    PlayerClass* find_class(const std::string& identifier) noexcept;
    const PlayerClass* find_class(const std::string& identifier) const noexcept;

private:
    std::string m_identifier;
    std::string m_displayName;
    std::uint32_t m_colorHex;
    std::vector<PlayerClass> m_classes;
};

}  // namespace rf2::server

