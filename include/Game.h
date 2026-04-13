#pragma once
#include "Player.h"
#include "Monster.h"
#include "ActAction.h"
#include <vector>
#include <memory>
#include <string>

// Entrée du bestiaire : monstre vaincu + résultat
struct BestiaryEntry {
    std::string name;
    std::string category;
    int hpMax;
    int atk;
    int def;
    bool killed; // true = tué, false = épargné
};

class Game {
public:
    enum class CombatResult { VICTORY_KILL, VICTORY_SPARE, DEFEAT };
private:
    Player player;
    ActionCatalog catalog;
    std::vector<std::unique_ptr<Monster>> monsterPool;
    std::vector<BestiaryEntry> bestiary;

    // ── Menus ──
    void showMainMenu();
    void showBestiary()    const;
    void showPlayerStats() const;
    void showItems();

    // ── Combat ──
    void startCombat();
    CombatResult runCombat(Monster& monster);

    // ── Actions combat ──
    void doFight(Monster& monster);
    void doAct(Monster& monster);
    void doItem();
    void doMercy(Monster& monster, bool& combatOver, bool& mercied);

    // ── Fin de partie ──
    void showEnding() const;

    // ── Utilitaires ──
    Monster* pickRandomMonster();
    int readInt(int min, int max) const;
public:
    Game();
    void run();
};
