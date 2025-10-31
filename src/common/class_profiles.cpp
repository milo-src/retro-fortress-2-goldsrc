#include "retro_fortress/common/class_profiles.hpp"

namespace rf2::common {

namespace {

const std::vector<ClassProfile> kDefaultClassProfiles = {
    {"scout", "Scout", "Offense", {"scattergun", "pistol", "bat"}},
    {"soldier", "Soldier", "Offense", {"rocket_launcher", "shotgun", "shovel"}},
    {"pyro", "Pyro", "Offense", {"flamethrower", "shotgun", "axe"}},
    {"demoman", "Demoman", "Defense", {"grenade_launcher", "sticky_launcher", "bottle"}},
    {"heavy", "Heavy", "Defense", {"minigun", "shotgun", "fists"}},
    {"engineer", "Engineer", "Defense", {"shotgun", "pistol", "wrench"}},
    {"medic", "Medic", "Support", {"syringe_gun", "medi_gun", "bone_saw"}},
    {"sniper", "Sniper", "Support", {"sniper_rifle", "smg", "kukri"}},
    {"spy", "Spy", "Support", {"revolver", "sap", "knife"}},
};

}  // namespace

const std::vector<ClassProfile>& default_class_profiles() {
    return kDefaultClassProfiles;
}

}  // namespace rf2::common

