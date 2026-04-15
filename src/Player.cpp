#include "Player.h"
#include <iostream>
#include <iomanip>

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
    float var = Player::getVariance();
    std::string bar = "";
    int filled = (hpMax > 0) ? (hp * 20 / hpMax) : 0;
    for (int i = 0; i < 20; ++i) bar += (i < filled ? "#" : "-");

    std::cout << "\n"
              << "  ======================================\n"
              << "        PROFIL DE L'AVENTURIER\n"
              << "  ======================================\n"
              << "  Nom        : " << name << "\n"
              << "  --------------------------------------\n"
              << "  HP         : " << hp << " / " << hpMax << "\n"
              << "  Vie        : [" << bar << "]\n"
              << "  Attaque    : " << atk << "  (Var : +/-" << (int)(var * 100) << "%)\n"
              << "  Defense    : " << def << "%\n"
              << "  --------------------------------------\n"
              << "  Victoires  : " << victories << " / 10\n"
              << "  Tues       : " << kills << "   |   Epargnes : " << spared << "\n"
              << "  ======================================\n\n";
}