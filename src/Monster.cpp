#include "Monster.h"
#include "NormalMonster.h"
#include "MiniBoss.h"
#include "Boss.h"
#include <iostream>
#include <algorithm>
#include <random>

// Constructeur : initialise les stats du monstre et sa jauge de Mercy
Monster::Monster(const std::string& name, int hp, int atk, int def,
                 int mercyGoal, const std::vector<std::string>& actIds, CategoryType cat)
    : Entity(name, hp, atk, def), mercy(0), mercyGoal(mercyGoal),
      actIds(actIds), category(cat) {}

// Getters pour la gestion du combat et de l'epargne
CategoryType Monster::getCategory() const { return category; }
int Monster::getMercy()     const { return mercy; }
int Monster::getMercyGoal() const { return mercyGoal; }
bool Monster::canBeMercied() const { return mercy >= mercyGoal; }
const std::vector<std::string>& Monster::getActIds() const { return actIds; }

void Monster::applyMercyImpact(int impact) {
    // Utilise clamp pour rester entre 0 et le max de la jauge
    mercy = std::clamp(mercy + impact, 0, mercyGoal);
}

// Logique d'attaque du monstre (similaire au joueur)
void Monster::attack(Entity& target) const {
    static std::mt19937 rng(std::random_device{}());

    // recupere la variance
    float var = Monster::getVariance();

    // calcule les bornes (Utilise l'atk héritée d'Entity)
    int minAtk = static_cast<int>(this->atk * (1.0f - var));
    int maxAtk = static_cast<int>(this->atk * (1.0f + var));

    // jet de des
    std::uniform_int_distribution<int> dist(minAtk, maxAtk);
    int rawDmg = dist(rng);

    // reduction par la defense du joeur
    float reductionFactor = target.getDef() / 100.0f;
    int blocked = static_cast<int>(rawDmg * reductionFactor);
    int finalDmg = rawDmg - blocked;

    if (finalDmg < 1) finalDmg = 1;

    // application & affichage
    target.takeDamage(finalDmg);

    std::cout << "\n> " << name << " attaque : " << rawDmg << " ATK\n";
    std::cout << "> Tu bloques : -" << blocked
              << " (Bouclier " << target.getDef() << "%)\n";
    std::cout << ">> Tu perds : " << finalDmg << " HP\n";
}

// Affiche l'etat du monstre (PV et jauge Mercy)
void Monster::displayStats() const {
    std::cout << "[" << categoryToString(category) << "] " << name
              << " | HP: " << hp << "/" << hpMax
              << " | Mercy: " << mercy << "/" << mercyGoal << "\n";
}

// Helper pour l'affichage textuel de l'enum
std::string Monster::categoryToString(CategoryType cat) {
    switch (cat) {
        case CategoryType::NORMAL:   return "NORMAL";
        case CategoryType::MINIBOSS: return "MINIBOSS";
        case CategoryType::BOSS:     return "BOSS";
    }
    return "?";
}

// sous classes
// Chaque type de monstre a un nombre d'actions (ACT) different
NormalMonster::NormalMonster(const std::string& name, int hp, int atk, int def,
                             int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::NORMAL) {}
int NormalMonster::getActCount() const { return 2; } // 2 actions

MiniBoss::MiniBoss(const std::string& name, int hp, int atk, int def,
                   int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::MINIBOSS) {}
int MiniBoss::getActCount() const { return 3; } // 3actions

Boss::Boss(const std::string& name, int hp, int atk, int def,
           int mercyGoal, const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, mercyGoal, actIds, CategoryType::BOSS) {}
int Boss::getActCount() const { return 4; } //4actions
