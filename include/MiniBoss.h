#pragma once
#include "Monster.h"

class MiniBoss : public Monster {
public:
    MiniBoss(const std::string& name, int hp, int atk, int def,
             int mercyGoal, const std::vector<std::string>& actIds);
    int getActCount() const override; // retourne 3
};
