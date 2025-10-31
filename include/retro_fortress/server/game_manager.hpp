#pragma once

#include <memory>
#include <vector>

#include "retro_fortress/server/team.hpp"

namespace rf2::server {

class GameManager {
public:
    static GameManager& instance();

    void initialize_default_rules();
    const std::vector<Team>& teams() const noexcept { return m_teams; }

    Team* find_team(const std::string& identifier) noexcept;
    const Team* find_team(const std::string& identifier) const noexcept;

private:
    GameManager() = default;

    std::vector<Team> m_teams;
};

}  // namespace rf2::server

