#pragma once

#include <string>
#include <vector>

namespace rf2::server {

class PlayerClass {
public:
    PlayerClass(std::string identifier, std::string displayName, std::string role);

    const std::string& identifier() const noexcept { return m_identifier; }
    const std::string& display_name() const noexcept { return m_displayName; }
    const std::string& role() const noexcept { return m_role; }

    void add_loadout_item(std::string item);
    const std::vector<std::string>& loadout() const noexcept { return m_loadout; }

private:
    std::string m_identifier;
    std::string m_displayName;
    std::string m_role;
    std::vector<std::string> m_loadout;
};

}  // namespace rf2::server

