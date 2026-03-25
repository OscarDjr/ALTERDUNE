#pragma once
#include <string>

class Entity {
protected:
    std::string name;
    int hp;
    int hpMax;
public:
    Entity(const std::string& name, int hpMax);
    virtual ~Entity() = default;

    std::string getName() const;
    int getHp()    const;
    int getHpMax() const;
    bool isAlive() const;

    void takeDamage(int dmg);
    void heal(int amount);

    virtual void displayStats() const = 0;
};
