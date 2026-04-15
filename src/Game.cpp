#include "Game.h"
#include "Combat.h"
#include "FileLoader.h"
#include <iostream>
#include <random>

Game::Game() : player("Joueur") {
    std::cout << "╔══════════════════════════════╗\n"
              << "║      A L T E R D U N E       ║\n"
              << "╚══════════════════════════════╝\n\n";

    std::cout << "Nom de votre personnage : ";
    std::string name;
    std::getline(std::cin, name);
    if (!name.empty()) player = Player(name);

    try {
        FileLoader::loadItems("data/items.csv", player.getInventory());
        monsterPool = FileLoader::loadMonsters("data/monsters.csv", catalog);
    } catch (const std::exception& e) {
        std::cerr << "Erreur critique : " << e.what() << "\n";
        std::exit(1);
    }
}

void Game::run() {
    while (player.getVictories() < 10) {
        showMainMenu();
    }
    showEnding();
}

void Game::showMainMenu() {
    std::cout << "\n=== MENU PRINCIPAL (Victoires: " << player.getVictories() << "/10) ===\n"
              << "1. Bestiaire\n2. Demarrer un combat\n3. Statistiques\n4. Items\n5. Quitter\n> ";

    switch (readInt(1, 5)) {
        case 1: showBestiary();    break;
        case 2: startCombat();     break;
        case 3: showPlayerStats(); break;
        case 4: showItems();       break;
        case 5: std::exit(0);
    }
}

void Game::startCombat() {
    // Spawn d'un monstre aleatoire
    Monster* base = pickRandomMonster();
    if (!base) return;
    auto monster = base->clone();

    std::cout << "\n~~~ Un " << Monster::categoryToString(monster->getCategory())
              << " apparait : " << monster->getName() << " ! ~~~\n"
              << "[INFO] " << player.getName() << " (ATK: " << player.getAtk() << " | DEF: " << player.getDef() << "%)\n"
              << "[INFO] " << monster->getName() << " (ATK: " << monster->getAtk() << " | DEF: " << monster->getDef() << "%)\n";
    // Lancement du moteur de combat
    Combat encounter(player, *monster, catalog);
    Combat::Result res = encounter.start();

    //fin de partie si mort
    if (res == Combat::Result::DEFEAT) {
        std::cout << "\n*** GAME OVER : " << player.getName() << " a peri... ***\n";
        std::cin.get();
        std::exit(0);
    }

    // Stats de victoire et enregistrement nom
    player.addVictory();
    defeatedMonsters.insert(monster->getName());
    bool killed = (res == Combat::Result::VICTORY_KILL);

    // Check si tue ou epargne pour l'ending
    if (killed) {
        player.addKill();
        std::cout << "\n*** " << monster->getName() << " a ete vaincu ! ***\n";
    } else {
        player.addSpared();
        std::cout << "\n*** " << monster->getName() << " a ete epargne ! ***\n";
    }

    player.addAtk(2);
    player.addDef(5);
    std::cout << "  [+] ATK : " << player.getAtk() << " (+2)\n"
              << "  [+] DEF : " << player.getDef() << "% (+5)\n";

    bestiary.push_back({
        monster->getName(),
        Monster::categoryToString(monster->getCategory()),
        monster->getHpMax(),
        monster->getAtk(),
        monster->getDef(),
        killed
    });
}

void Game::showBestiary() const {
    std::cout << "\n=== BESTIAIRE ===\n";
    if (bestiary.empty()) std::cout << "  (Vide)\n";
    for (const auto& e : bestiary)
        std::cout << "  [" << e.category << "] " << e.name << " | " << (e.killed ? "Tue" : "Epargne") << "\n";
}

void Game::showPlayerStats() const { player.displayStats(); }

void Game::showItems() {
    std::cout << "\n=== INVENTAIRE ===\n";
    player.getInventory().display();
    std::cout << "(Impossible d'utiliser un item hors combat.)\n";
}

void Game::showEnding() const {
    std::cout << "\n--- FIN : " << player.checkEnding() << " ---\n";
}

Monster* Game::pickRandomMonster() {
    // Filtrage : on ne veut pas retomber sur un monstre deja vaincu
    std::vector<Monster*> available;
    for (auto& m : monsterPool)
        if (defeatedMonsters.find(m->getName()) == defeatedMonsters.end())
            available.push_back(m.get());

    // Tirage au sort avec random_device
    if (available.empty()) return nullptr;
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, (int)available.size() - 1);
    return available[dist(rng)];
}

int Game::readInt(int min, int max) const {
    int val;
    // Boucle de securite contre les entrees de texte (cin.fail)
    while (!(std::cin >> val) || val < min || val > max) {
        std::cin.clear(); std::cin.ignore(10000, '\n');
        std::cout << "Choix invalide : ";
    }
    std::cin.ignore(10000, '\n');
    return val;
}