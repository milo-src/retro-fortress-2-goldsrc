#include "retro_fortress/server/game_manager.hpp"

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

    PlayerClass scout("scout", "Scout", "Offense");
    scout.add_loadout_item("scattergun");
    scout.add_loadout_item("pistol");
    scout.add_loadout_item("bat");

    PlayerClass soldier("soldier", "Soldier", "Offense");
    soldier.add_loadout_item("rocket_launcher");
    soldier.add_loadout_item("shotgun");
    soldier.add_loadout_item("shovel");

    PlayerClass pyro("pyro", "Pyro", "Offense");
    pyro.add_loadout_item("flamethrower");
    pyro.add_loadout_item("shotgun");
    pyro.add_loadout_item("axe");

    PlayerClass demoman("demoman", "Demoman", "Defense");
    demoman.add_loadout_item("grenade_launcher");
    demoman.add_loadout_item("sticky_launcher");
    demoman.add_loadout_item("bottle");

    PlayerClass heavy("heavy", "Heavy", "Defense");
    heavy.add_loadout_item("minigun");
    heavy.add_loadout_item("shotgun");
    heavy.add_loadout_item("fists");

    PlayerClass engineer("engineer", "Engineer", "Defense");
    engineer.add_loadout_item("shotgun");
    engineer.add_loadout_item("pistol");
    engineer.add_loadout_item("wrench");

    PlayerClass medic("medic", "Medic", "Support");
    medic.add_loadout_item("syringe_gun");
    medic.add_loadout_item("medi_gun");
    medic.add_loadout_item("bone_saw");

    PlayerClass sniper("sniper", "Sniper", "Support");
    sniper.add_loadout_item("sniper_rifle");
    sniper.add_loadout_item("smg");
    sniper.add_loadout_item("kukri");

    PlayerClass spy("spy", "Spy", "Support");
    spy.add_loadout_item("revolver");
    spy.add_loadout_item("sap");
    spy.add_loadout_item("knife");

    red.add_class(scout);
    red.add_class(soldier);
    red.add_class(pyro);
    red.add_class(demoman);
    red.add_class(heavy);
    red.add_class(engineer);
    red.add_class(medic);
    red.add_class(sniper);
    red.add_class(spy);

    blu.add_class(scout);
    blu.add_class(soldier);
    blu.add_class(pyro);
    blu.add_class(demoman);
    blu.add_class(heavy);
    blu.add_class(engineer);
    blu.add_class(medic);
    blu.add_class(sniper);
    blu.add_class(spy);

    m_teams.push_back(red);
    m_teams.push_back(blu);

    log_info("Retro Fortress 2 default rules initialized (2 teams, 9 classes)");
}

}  // namespace rf2::server

