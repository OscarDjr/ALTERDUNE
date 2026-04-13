#include "Monster.h"
#include "NormalMonster.h"
#include "MiniBoss.h"
#include "Boss.h"
#include <iostream>
#include <algorithm>
#include <random>

Monster::Monster(const std::string& name, int hp, int atk, int def,
                 int mercyGoal, const std::vector<std::string>& actIds, CategoryType cat)
    : Entity(name, hp, atk, def), mercy(0), mercyGoal(mercyGoal),
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

    // 1. RÉCUPÉRATION DE LA VARIANCE 
    float var = Monster::getVariance();

    // 2. CALCUL DES BORNES (Utilise l'atk héritée d'Entity)
    int minAtk = static_cast<int>(this->atk * (1.0f - var));
    int maxAtk = static_cast<int>(this->atk * (1.0f + var));

    // 3. JET DE DÉS
    std::uniform_int_distribution<int> dist(minAtk, maxAtk);
    int rawDmg = dist(rng);

    // 4. RÉDUCTION PAR LA DÉFENSE DU JOUEUR
    float reductionFactor = target.getDef() / 100.0f;
    int blocked = static_cast<int>(rawDmg * reductionFactor);
    int finalDmg = rawDmg - blocked;

    if (finalDmg < 1) finalDmg = 1;

    // 5. APPLICATION ET AFFICHAGE
    target.takeDamage(finalDmg);

    std::cout << "\n> " << name << " attaque : " << rawDmg << " ATK\n";
    std::cout << "> Tu bloques : -" << blocked
              << " (Bouclier " << target.getDef() << "%)\n";
    std::cout << ">> Tu perds : " << finalDmg << " HP\n";
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

// ── Sous-classes ──

NormalMonster::NormalMonster(const std::string& name, int hp, int atk, int def,
                             int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::NORMAL) {}
int NormalMonster::getActCount() const { return 2; }

MiniBoss::MiniBoss(const std::string& name, int hp, int atk, int def,
                   int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::MINIBOSS) {}
int MiniBoss::getActCount() const { return 3; }

Boss::Boss(const std::string& name, int hp, int atk, int def,
           int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::BOSS) {}
int Boss::getActCount() const { return 4; }
