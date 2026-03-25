#include "Player.h"
#include <iostream>

Player::Player(const std::string& name, int hpMax)
    : Entity(name, hpMax), kills(0), spared(0), victories(0) {}

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
    std::cout << "=== " << name << " ===\n"
              << "HP        : " << hp << " / " << hpMax << "\n"
              << "Victoires : " << victories << " / 10\n"
              << "Tues      : " << kills     << "\n"
              << "Epargnes  : " << spared    << "\n";
}
