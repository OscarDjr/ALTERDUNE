#include "Entity.h"
#include <iostream>
#include <algorithm>

// Constructeur : initialise les stats de base
Entity::Entity(const std::string& name, int hpMax, int atk, int def)
    : name(name), hp(hpMax), hpMax(hpMax), atk(atk), def(def) {}

std::string Entity::getName() const { return name; }
int Entity::getHp()    const { return hp; }
int Entity::getHpMax() const { return hpMax; }
bool Entity::isAlive() const { return hp > 0; }

void Entity::takeDamage(int dmg) {
    // Calcul degats et securite pour ne pas descendre sous 0
    hp = std::max(0, hp - dmg);
}

void Entity::heal(int amount) {
    // Soin avec limite au PV Max (pour pas depasser)
    hp = std::min(hpMax, hp + amount);
}
