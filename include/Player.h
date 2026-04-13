#pragma once
#include "Entity.h"
#include "Item.h"

class Player : public Entity {
private:
    static constexpr int BASE_ATK = 20;   // Attaque de base
    static constexpr int BASE_DEF = 0;    // 0% de réduction de base
    static constexpr int PLAYER_HP = 100;
    static constexpr float ATTACK_VARIANCE = 0.50f; // 50% de variance
    int kills;
    int spared;
    int victories;
    Inventory inventory;
public:
    Player(const std::string& name);
    int getKills()     const;
    int getSpared()    const;
    int getVictories() const;
    static float getVariance() { return ATTACK_VARIANCE; }
    Inventory& getInventory();

    void addKill();
    void addSpared();
    void addVictory();

    std::string checkEnding() const;
    void displayStats() const override;
};
