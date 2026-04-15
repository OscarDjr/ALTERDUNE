#pragma once
#include "Monster.h"

class NormalMonster : public Monster {
public:
    NormalMonster(const std::string& name, int hp, int atk, int def,
                  int mercyGoal, const std::vector<std::string>& actIds);
    int getActCount() const override; // retourne 2
    std::unique_ptr<Monster> clone() const override {
        return std::make_unique<NormalMonster>(*this);
    }
};
