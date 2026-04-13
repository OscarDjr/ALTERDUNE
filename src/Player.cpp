#include "Player.h"
#include <iostream>

Player::Player(const std::string& name)
    : Entity(name, PLAYER_HP, BASE_ATK, BASE_DEF), kills(0), spared(0), victories(0) {}

int Player::getKills()     const { return kills; }
int Player::getSpared()    const { return spared; }
int Player::getVictories() const { return victories; }
Inventory& Player::getInventory() { return inventory; }

void Player::addKill()    { ++kills; }
void Player::addSpared()  { ++spared; }
void Player::addVictory() { ++victories; }

std::string Player::checkEnding() const {
    if (spared == 0) return "Fin Genocidaire";
    if (kills  == 0) return "Fin Pacifiste";
    return "Fin Neutre";
}

void Player::displayStats() const {
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║             PROFIL DE L'AME                  ║\n";
    std::cout << "╠══════════════════════════════════════════════╝\n";
    std::cout << "║  Nom       : " << name << "\n";
    std::cout << "║  Sante     : " << hp << " / " << hpMax << " HP\n";
    std::cout << "║\n";
    std::cout << "║  FORCE (ATK) : " << atk << " (Variation: +/- 50%)\n";
    std::cout << "║  PLAGE DEGATS: [" << static_cast<int>(atk * 0.5) << " - " << static_cast<int>(atk * 1.5) << "]\n";
    std::cout << "║  ARMURE(DEF) : " << def << "% de reduction\n";
    std::cout << "║\n";
    std::cout << "║  PALMARES :\n";
    std::cout << "║  - Monstres vaincus  : " << victories << "\n";
    std::cout << "║  - Morts causees     : " << kills << "\n";
    std::cout << "║  - Ames epargnees    : " << spared << "\n";
    std::cout << "╚══════════════════════════════════════════════╝\n";
}
