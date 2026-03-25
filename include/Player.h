#pragma once
#include "Entity.h"
#include "Item.h"

class Player : public Entity {
private:
    int kills;
    int spared;
    int victories;
    Inventory inventory;
public:
    Player(const std::string& name, int hpMax = 100);

    int getKills()     const;
    int getSpared()    const;
    int getVictories() const;
    Inventory& getInventory();

    void addKill();
    void addSpared();
    void addVictory();

    std::string checkEnding() const;
    void displayStats() const override;
};
