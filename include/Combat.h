#pragma once
#include "Player.h"
#include "Monster.h"
#include "ActAction.h"

class Combat {
public:
    enum class Result { VICTORY_KILL, VICTORY_SPARE, DEFEAT };

    Combat(Player& p, Monster& m, const ActionCatalog& cat);

    Result start();

private:
    Player& player;
    Monster& monster;
    const ActionCatalog& catalog;
    bool mercied = false;

    void playerTurn(bool& combatOver);
    void doFight();
    void doAct();
    bool doItem();
    void doMercy(bool& combatOver);

    int readInt(int min, int max) const;
};