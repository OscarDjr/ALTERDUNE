#include "Entity.h"
#include <iostream>
#include <algorithm>

Entity::Entity(const std::string& name, int hpMax)
    : name(name), hp(hpMax), hpMax(hpMax) {}

std::string Entity::getName() const { return name; }
int Entity::getHp()    const { return hp; }
int Entity::getHpMax() const { return hpMax; }
bool Entity::isAlive() const { return hp > 0; }

void Entity::takeDamage(int dmg) {
    hp = std::max(0, hp - dmg);
}

void Entity::heal(int amount) {
    hp = std::min(hpMax, hp + amount);
}
