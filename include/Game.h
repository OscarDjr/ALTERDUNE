#pragma once
#include "Player.h"
#include "Monster.h"
#include "ActAction.h"
#include <vector>
#include <memory>
#include <set>

struct BestiaryEntry {
    std::string name;
    std::string category;
    int hpMax;
    int atk;
    int def;
    bool killed;
};

class Game {
private:
    Player player;
    std::vector<std::unique_ptr<Monster>> monsterPool;
    std::vector<BestiaryEntry> bestiary;
    std::set<std::string> defeatedMonsters;
    ActionCatalog catalog;

    void showMainMenu();
    void startCombat();
    void showBestiary() const;
    void showPlayerStats() const;
    void showItems();
    void showEnding() const;

    Monster* pickRandomMonster();
    int readInt(int min, int max) const;

public:
    Game();
    void run();
};