#include "Monster.h"
#include "NormalMonster.h"
#include "MiniBoss.h"
#include "Boss.h"
#include <iostream>
#include <algorithm>
#include <random>

// ─────── Monster (base) ───────

Monster::Monster(const std::string& name, int hp, int atk, int def,
                 int mercyGoal, const std::vector<std::string>& actIds, CategoryType cat)
    : Entity(name, hp), atk(atk), def(def), mercy(0), mercyGoal(mercyGoal),
      actIds(actIds), category(cat) {}

CategoryType Monster::getCategory() const { return category; }
int Monster::getMercy()     const { return mercy; }
int Monster::getMercyGoal() const { return mercyGoal; }
bool Monster::canBeMercied() const { return mercy >= mercyGoal; }
const std::vector<std::string>& Monster::getActIds() const { return actIds; }

void Monster::applyMercyImpact(int impact) {
    mercy = std::clamp(mercy + impact, 0, mercyGoal);
}

void Monster::attack(Entity& target) const {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, target.getHpMax());
    int dmg = dist(rng);
    target.takeDamage(dmg);
    std::cout << name << " attaque ! ";
    if (dmg == 0) std::cout << "Coup rate !\n";
    else          std::cout << dmg << " degats infliges au joueur.\n";
}

void Monster::displayStats() const {
    std::cout << "[" << categoryToString(category) << "] " << name
              << " | HP: " << hp << "/" << hpMax
              << " | Mercy: " << mercy << "/" << mercyGoal << "\n";
}

std::string Monster::categoryToString(CategoryType cat) {
    switch (cat) {
        case CategoryType::NORMAL:   return "NORMAL";
        case CategoryType::MINIBOSS: return "MINIBOSS";
        case CategoryType::BOSS:     return "BOSS";
    }
    return "?";
}

// ─────── NormalMonster ───────

NormalMonster::NormalMonster(const std::string& name, int hp, int atk, int def,
                             int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::NORMAL) {}

int NormalMonster::getActCount() const { return 2; }

// ─────── MiniBoss ───────

MiniBoss::MiniBoss(const std::string& name, int hp, int atk, int def,
                   int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::MINIBOSS) {}

int MiniBoss::getActCount() const { return 3; }

// ─────── Boss ───────

Boss::Boss(const std::string& name, int hp, int atk, int def,
           int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::BOSS) {}

int Boss::getActCount() const { return 4; }
