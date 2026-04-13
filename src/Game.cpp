#include "Game.h"
#include "FileLoader.h"
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>

Game::Game() : player("", 100) {
    std::cout << "╔══════════════════════════════╗\n";
    std::cout << "║      A L T E R D U N E       ║\n";
    std::cout << "╚══════════════════════════════╝\n\n";
    std::cout << "Entrez le nom de votre personnage : ";
    std::string name;
    std::getline(std::cin, name);
    if (name.empty()) name = "Joueur";
    player = Player(name, 100);

    try {
        FileLoader::loadItems("data/items.csv", player.getInventory());
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n"; std::exit(1);
    }
    try {
        monsterPool = FileLoader::loadMonsters("data/monsters.csv", catalog);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n"; std::exit(1);
    }

    std::cout << "\n--- Bienvenue, " << player.getName() << " ! ---\n";
    std::cout << "HP : " << player.getHp() << " / " << player.getHpMax() << "\n";
    std::cout << "Inventaire de depart :\n";
    player.getInventory().display();
    std::cout << "\n";
}

void Game::run() {
    while (player.getVictories() < 10)
        showMainMenu();
    showEnding();
}

void Game::showMainMenu() {
    std::cout << "\n=== MENU PRINCIPAL ===\n";
    std::cout << "Victoires : " << player.getVictories() << " / 10\n\n";
    std::cout << "1. Bestiaire\n2. Demarrer un combat\n3. Statistiques\n4. Items\n5. Quitter\n> ";

    int choice = readInt(1, 5);
    switch (choice) {
        case 1: showBestiary();    break;
        case 2: startCombat();     break;
        case 3: showPlayerStats(); break;
        case 4: showItems();       break;
        case 5:
            std::cout << "Au revoir, " << player.getName() << " !\n";
            std::exit(0);
    }
}

void Game::showBestiary() const {
    std::cout << "\n=== BESTIAIRE ===\n";
    if (bestiary.empty()) { std::cout << "  (Aucun monstre vaincu)\n"; return; }
    for (const auto& e : bestiary)
        std::cout << "  [" << e.category << "] " << e.name
                  << " | HP max: " << e.hpMax << " | ATK: " << e.atk
                  << " | DEF: " << e.def
                  << " | " << (e.killed ? "Tue" : "Epargne") << "\n";
}

void Game::showPlayerStats() const {
    std::cout << "\n";
    player.displayStats();
}

void Game::showItems() {
    std::cout << "\n=== INVENTAIRE ===\n";
    player.getInventory().display();
    if (player.getInventory().isEmpty()) return;
    std::cout << "Utiliser un item ? (0 = non) : ";
    int choice = readInt(0, player.getInventory().size());
    if (choice > 0)
        player.getInventory().useItem(choice - 1, player);
}

void Game::startCombat() {
    Monster* base = pickRandomMonster();
    if (!base) { std::cout << "Aucun monstre disponible !\n"; return; }

    auto monster = base->clone();
    std::cout << "\n~~~ Un " << Monster::categoryToString(monster->getCategory())
              << " apparait : " << monster->getName() << " ! ~~~\n";
    monster->displayStats();

    CombatResult result = runCombat(*monster);  // ← CombatResult, plus bool

    if (result == CombatResult::DEFEAT) {
        std::cout << "\n*** Vous avez ete vaincu. Game Over. ***\n";
        std::exit(0);
    }
}

Game::CombatResult Game::runCombat(Monster& monster) {  // ← Game::CombatResult
    bool combatOver = false;
    bool mercied    = false;

    while (!combatOver) {
        std::cout << "\n--- Tour de combat ---\n";
        std::cout << "[Joueur] HP: " << player.getHp() << "/" << player.getHpMax() << "\n";
        monster.displayStats();
        std::cout << "\n  1. FIGHT  2. ACT  3. ITEM  4. MERCY\n> ";

        switch (readInt(1, 4)) {
            case 1: doFight(monster); break;
            case 2: doAct(monster);   break;
            case 3: doItem();         break;
            case 4: doMercy(monster, combatOver, mercied); break;
        }

        if (!monster.isAlive() && !combatOver) {
            combatOver = true;
            mercied    = false;
        }

        if (!combatOver && monster.isAlive()) {
            monster.attack(player);
            if (!player.isAlive()) return CombatResult::DEFEAT;
        }
    }

    player.addVictory();
    if (mercied) {
        player.addSpared();
        std::cout << "\n*** " << monster.getName() << " a ete epargne ! ***\n";
    } else {
        player.addKill();
        std::cout << "\n*** " << monster.getName() << " a ete vaincu ! ***\n";
    }

    bestiary.push_back({
        monster.getName(),
        Monster::categoryToString(monster.getCategory()),
        monster.getHpMax(),
        monster.getAtk(),
        monster.getDef(),
        !mercied
    });

    std::cout << "Victoires : " << player.getVictories() << " / 10\n";
    return mercied ? CombatResult::VICTORY_SPARE : CombatResult::VICTORY_KILL;
}

void Game::doFight(Monster& monster) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, monster.getHpMax());
    int dmg = dist(rng);
    monster.takeDamage(dmg);
    std::cout << "Vous attaquez " << monster.getName() << " ! ";
    if (dmg == 0) std::cout << "Coup rate !\n";
    else          std::cout << dmg << " degats infliges.\n";
}

void Game::doAct(Monster& monster) {
    const auto& actIds = monster.getActIds();
    int count = std::min((int)actIds.size(), monster.getActCount());

    std::cout << "\n=== Actions ACT ===\n";
    for (int i = 0; i < count; ++i)
        std::cout << "  " << i + 1 << ". " << actIds[i] << "\n";
    std::cout << "> ";

    const ActAction& act = catalog.getAction(actIds[readInt(1, count) - 1]);
    std::cout << "\n>> " << act.text << "\n";
    monster.applyMercyImpact(act.mercyImpact);
    std::cout << "Mercy : " << monster.getMercy() << " / " << monster.getMercyGoal() << "\n";
}

void Game::doItem() {
    if (player.getInventory().isEmpty()) {
        std::cout << "Votre inventaire est vide !\n"; return;
    }
    std::cout << "\n=== Inventaire ===\n";
    player.getInventory().display();
    std::cout << "(0 = annuler) > ";
    int choice = readInt(0, player.getInventory().size());
    if (choice > 0)
        player.getInventory().useItem(choice - 1, player);
}

void Game::doMercy(Monster& monster, bool& combatOver, bool& mercied) {
    if (!monster.canBeMercied()) {
        std::cout << "Le monstre n'est pas pret. (Mercy : "
                  << monster.getMercy() << " / " << monster.getMercyGoal() << ")\n";
        return;
    }
    combatOver = true;
    mercied    = true;
}

void Game::showEnding() const {
    std::string ending = player.checkEnding();
    std::cout << "\n╔══════════════════════════════╗\n";
    std::cout << "║         FIN DE PARTIE         ║\n";
    std::cout << "╚══════════════════════════════╝\n";
    std::cout << "Joueur : " << player.getName() << "\n" << ending << "\n";
    if      (ending == "Fin Genocidaire") std::cout << "Vous avez elimine tout le monde.\n";
    else if (ending == "Fin Pacifiste")   std::cout << "Vous avez epargne chaque ame.\n";
    else                                  std::cout << "Ni tout noir ni tout blanc.\n";
}

Monster* Game::pickRandomMonster() {
    if (monsterPool.empty()) return nullptr;
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, (int)monsterPool.size() - 1);
    return monsterPool[dist(rng)].get();
}

int Game::readInt(int min, int max) const {
    int val;
    while (true) {
        std::cin >> val;
        if (std::cin.fail() || val < min || val > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entre " << min << " et " << max << " : ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
    }
}