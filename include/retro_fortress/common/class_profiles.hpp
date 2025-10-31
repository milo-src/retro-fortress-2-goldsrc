#pragma once

#include <string>
#include <vector>

namespace rf2::common {

struct ClassProfile {
    std::string identifier;
    std::string display_name;
    std::string role;
    std::vector<std::string> loadout;
};

const std::vector<ClassProfile>& default_class_profiles();

}  // namespace rf2::common

