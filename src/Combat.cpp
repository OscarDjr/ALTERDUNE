#include "Combat.h"
#include <iostream>
#include <random>
#include <algorithm>

Combat::Combat(Player& p, Monster& m, const ActionCatalog& cat)
    : player(p), monster(m), catalog(cat) {}

// Boucle principale du combat
Combat::Result Combat::start() {
    bool combatOver = false;

    while (!combatOver) {
        std::cout << "\n--- Tour de combat ---\n";
        std::cout << "[Joueur] HP: " << player.getHp() << "/" << player.getHpMax() << "\n";
        monster.displayStats();
        
        playerTurn(combatOver);

        // Check si le monstre est mort apres le tour du joueur
        if (!monster.isAlive() && !combatOver) {
            combatOver = true;
            mercied = false;
        }

        // Riposte du monstre si le combat continue
        if (!combatOver && monster.isAlive()) {
            monster.attack(player);
            if (!player.isAlive()) return Result::DEFEAT;
        }
    }

    // Renvoie le type de victoire pour le score final
    return mercied ? Result::VICTORY_SPARE : Result::VICTORY_KILL;
}

// Menu de choix du joueur
void Combat::playerTurn(bool& combatOver) {
    std::cout << "\n  1. FIGHT  2. ACT  3. ITEM  4. MERCY\n> ";
    int choice = readInt(1, 4);

    switch (choice) {
        case 1: doFight(); break;
        case 2: doAct();   break;
        case 3:
            //si vide -->tour rejoué
            if (!doItem()) playerTurn(combatOver);  // tour rejoué
            break;
        case 4: doMercy(combatOver); break;
    }
}

void Combat::doFight() {
    static std::mt19937 rng(std::random_device{}());
    float var = Player::getVariance();

    // Calcul des degats avec une part d'aleatoire (variance)
    int minAtk = static_cast<int>(player.getAtk() * (1.0f - var));
    int maxAtk = static_cast<int>(player.getAtk() * (1.0f + var));

    std::uniform_int_distribution<int> dist(minAtk, maxAtk);
    int rawDmg = dist(rng);

    // Calcul de la reduction par la defense du monstre
    float reduction = monster.getDef() / 100.0f;
    int blocked = static_cast<int>(rawDmg * reduction);
    int finalDmg = std::max(1, rawDmg - blocked);

    monster.takeDamage(finalDmg);
    std::cout << "\n> Tu frappes : " << rawDmg << " ATK\n"
              << "> " << monster.getName() << " bloque : -" << blocked << " (Bouclier " << monster.getDef() << "%)\n"
              << ">> Degats infliges : " << finalDmg << " HP !\n";
}

void Combat::doAct() {
    // Liste les actions speciales du monstre actuel
    const auto& actIds = monster.getActIds();
    int count = std::min((int)actIds.size(), monster.getActCount());

    std::cout << "\n=== Actions ACT ===\n";
    for (int i = 0; i < count; ++i)
        std::cout << "  " << i + 1 << ". " << actIds[i] << "\n";

    int choice = readInt(1, count);
    const ActAction& act = catalog.getAction(actIds[choice - 1]);

    int before = monster.getMercy();
    // Update la jauge de Mercy du monstre
    monster.applyMercyImpact(act.mercyImpact);
    int after = monster.getMercy();
    int delta = after - before;

    std::cout << "\n>> " << act.text << "\n";
    std::cout << "Mercy : " << after << "%"
              << "  (" << (delta >= 0 ? "+" : "") << delta << "%"")\n";
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
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);

    int roll = dist(rng);
    int mercy = monster.getMercy();

    // Tentative d'epargne basee sur le % de la jauge Mercy
    if (roll <= mercy) {
        std::cout << "\nLe monstre accepte d'etre epargne. (chance que ça soit arrivé : " << mercy << "%)\n";
        combatOver = true;
        mercied = true;
    } else {
        std::cout << "\nLe monstre refuse... (chance que le monstre refuse : " << 100-mercy << "%)\n";
    }
}
// Securise la saisie utilisateur
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