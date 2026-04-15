#pragma once
#include "Entity.h"
#include <vector>
#include <string>
#include <memory>

enum class CategoryType { NORMAL, MINIBOSS, BOSS };

class Monster : public Entity {
protected:
    int mercy;
    int mercyGoal;
    std::vector<std::string> actIds;
    CategoryType category;
    static constexpr float MONSTER_VARIANCE = 0.25f; // 25% de variance
public:
    Monster(const std::string& name, int hp, int atk, int def,
            int mercyGoal, const std::vector<std::string>& actIds, CategoryType cat);
    static float getVariance() { return MONSTER_VARIANCE; }
    virtual ~Monster() = default;

    // Accesseurs
    CategoryType getCategory() const;
    int getMercy()     const;
    int getMercyGoal() const;
    bool canBeMercied() const;
    const std::vector<std::string>& getActIds() const;

    void applyMercyImpact(int impact);

    virtual int getActCount() const = 0;
    virtual std::unique_ptr<Monster> clone() const = 0;
    void displayStats() const override;
    void attack(Entity& target) const;

    static std::string categoryToString(CategoryType cat);
};
