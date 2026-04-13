#pragma once
#include <string>

class Entity {
protected:
    std::string name;
    int hp;
    int hpMax;
    int atk; // Force de base du joueur ou du monstre
    int def; // Score de défense (0 à 100)
public:
    Entity(const std::string& name, int hpMax, int atk, int def);
    virtual ~Entity() = default;
    std::string getName() const;
    int getHp()    const;
    int getHpMax() const;
    int getAtk() const { return atk; }
    int getDef() const { return def; }
    bool isAlive() const;

    void takeDamage(int dmg);
    void heal(int amount);
    void addAtk(int amount) { atk += amount; }
    void addDef(int amount) { def += amount; }

    virtual void displayStats() const = 0;
};
