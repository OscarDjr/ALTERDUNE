#pragma once
#include "Entity.h"
#include "ActAction.h"
#include <vector>
#include <string>

enum class CategoryType { NORMAL, MINIBOSS, BOSS };

class Monster : public Entity {
protected:
    int atk;
    int def;
    int mercy;
    int mercyGoal;
    std::vector<std::string> actIds;
    CategoryType category;
public:
    Monster(const std::string& name, int hp, int atk, int def,
            int mercyGoal, const std::vector<std::string>& actIds, CategoryType cat);
    virtual ~Monster() = default;

    CategoryType getCategory() const;
    int getMercy()     const;
    int getMercyGoal() const;
    bool canBeMercied() const;
    const std::vector<std::string>& getActIds() const;

    void applyMercyImpact(int impact);

    virtual int getActCount() const = 0;

    void displayStats() const override;
    void attack(Entity& target) const;

    static std::string categoryToString(CategoryType cat);
};
