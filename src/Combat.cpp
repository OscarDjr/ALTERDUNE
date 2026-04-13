#include "Combat.h"
#include <iostream>
#include <random>
#include <algorithm>

Combat::Combat(Player& p, Monster& m, const ActionCatalog& cat)
    : player(p), monster(m), catalog(cat) {}

Combat::Result Combat::start() {
    bool combatOver = false;

    while (!combatOver) {
        std::cout << "\n--- Tour de combat ---\n";
        std::cout << "[Joueur] HP: " << player.getHp() << "/" << player.getHpMax() << "\n";
        monster.displayStats();
        
        playerTurn(combatOver);

        if (!monster.isAlive() && !combatOver) {
            combatOver = true;
            mercied = false;
        }

        if (!combatOver && monster.isAlive()) {
            monster.attack(player);
            if (!player.isAlive()) return Result::DEFEAT;
        }
    }

    return mercied ? Result::VICTORY_SPARE : Result::VICTORY_KILL;
}

void Combat::playerTurn(bool& combatOver) {
    std::cout << "\n  1. FIGHT  2. ACT  3. ITEM  4. MERCY\n> ";
    int choice = readInt(1, 4);

    switch (choice) {
        case 1: doFight(); break;
        case 2: doAct();   break;
        case 3:
            if (!doItem()) playerTurn(combatOver);  // tour rejoué
            break;
        case 4: doMercy(combatOver); break;
    }
}

void Combat::doFight() {
    static std::mt19937 rng(std::random_device{}());
    float var = Player::getVariance();

    int minAtk = static_cast<int>(player.getAtk() * (1.0f - var));
    int maxAtk = static_cast<int>(player.getAtk() * (1.0f + var));

    std::uniform_int_distribution<int> dist(minAtk, maxAtk);
    int rawDmg = dist(rng);

    float reduction = monster.getDef() / 100.0f;
    int blocked = static_cast<int>(rawDmg * reduction);
    int finalDmg = std::max(1, rawDmg - blocked);

    monster.takeDamage(finalDmg);
    std::cout << "\n> Tu frappes : " << rawDmg << " ATK\n"
              << "> " << monster.getName() << " bloque : -" << blocked << " (Bouclier " << monster.getDef() << "%)\n"
              << ">> Degats infliges : " << finalDmg << " HP !\n";
}

void Combat::doAct() {
    const auto& actIds = monster.getActIds();
    int count = std::min((int)actIds.size(), monster.getActCount());

    std::cout << "\n=== Actions ACT ===\n";
    for (int i = 0; i < count; ++i)
        std::cout << "  " << i + 1 << ". " << actIds[i] << "\n";
    
    int choice = readInt(1, count);
    const ActAction& act = catalog.getAction(actIds[choice - 1]);
    
    std::cout << "\n>> " << act.text << "\n";
    monster.applyMercyImpact(act.mercyImpact);
    std::cout << "Mercy : " << monster.getMercy() << " / " << monster.getMercyGoal() << "\n";
}

bool Combat::doItem() {
    if (player.getInventory().isEmpty()) {
        std::cout << "Inventaire vide !\n";
        return false;
    }
    player.getInventory().display();
    std::cout << "(0 = annuler) > ";
    int choice = readInt(0, player.getInventory().size());
    if (choice == 0) return false;
    player.getInventory().useItem(choice - 1, player);
    return true;
}

void Combat::doMercy(bool& combatOver) {
    if (monster.canBeMercied()) {
        combatOver = true;
        mercied = true;
    } else {
        std::cout << "Le monstre n'est pas pret a t'ecouter...\n";
    }
}

int Combat::readInt(int min, int max) const {
    int val;
    while (!(std::cin >> val) || val < min || val > max) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Entree invalide (" << min << "-" << max << ") : ";
    }
    std::cin.ignore(10000, '\n');
    return val;
}