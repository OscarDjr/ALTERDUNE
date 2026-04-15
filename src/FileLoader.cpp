#include "FileLoader.h"
#include "NormalMonster.h"
#include "MiniBoss.h"
#include "Boss.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// Utilitaires

// Nettoyage des espaces et retours chariots en debut/fin de chaine
std::string FileLoader::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Decoupage de la ligne selon le separateur (souvent ';')
std::vector<std::string> FileLoader::split(const std::string& line, char sep) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, sep))
        tokens.push_back(trim(token));
    return tokens;
}

// Chargement items.csv

// Lecture du fichier items.csv pour remplir l'inventaire
void FileLoader::loadItems(const std::string& filename, Inventory& inventory) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERREUR] Fichier introuvable : " << filename << "\n";
        throw std::runtime_error("Fichier introuvable : " + filename);
    }

    std::string line;
    std::getline(file, line); // ignorer l'en-tête

    int lineNum = 1;
    while (std::getline(file, line)) {
        ++lineNum;
        if (trim(line).empty()) continue;

        auto tokens = split(line);
        if (tokens.size() < 4) {
            std::cerr << "[AVERTISSEMENT] Ligne " << lineNum
                      << " mal formee dans " << filename << " (ignoree)\n";
            continue;
        }

        std::string nom   = tokens[0];
        std::string type  = tokens[1];
        int valeur        = 0;
        int quantite      = 0;

        try {
            valeur   = std::stoi(tokens[2]);
            quantite = std::stoi(tokens[3]);
        } catch (...) {
            std::cerr << "[AVERTISSEMENT] Ligne " << lineNum
                      << " : valeur/quantite invalide (ignoree)\n";
            continue;
        }

        ItemType itemType = ItemType::HEAL; // seul type supporté pour l'instant
        if (type != "HEAL") {
            std::cerr << "[AVERTISSEMENT] Type inconnu '" << type
                      << "' ligne " << lineNum << " (traite comme HEAL)\n";
        }

        inventory.addItem(Item(nom, itemType, valeur), quantite);
    }
}

// Chargement monsters.csv

std::vector<std::unique_ptr<Monster>> FileLoader::loadMonsters(
    const std::string& filename,
    const ActionCatalog& catalog)
{
    std::vector<std::unique_ptr<Monster>> monsters;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERREUR] Fichier introuvable : " << filename << "\n";
        throw std::runtime_error("Fichier introuvable : " + filename);
    }

    std::string line;
    std::getline(file, line); // ignorer l'en-tête

    int lineNum = 1;
    while (std::getline(file, line)) {
        ++lineNum;
        if (trim(line).empty()) continue;

        auto tokens = split(line);
        // Format : categorie;nom;hp;atk;def;mercyGoal;act1;act2;act3;act4
        if (tokens.size() < 10) {
            std::cerr << "[AVERTISSEMENT] Ligne " << lineNum
                      << " mal formee dans " << filename << " (ignoree)\n";
            continue;
        }

        std::string cat  = tokens[0];
        std::string nom  = tokens[1];
        int hp = 0, atk = 0, def = 0, mercyGoal = 0;

        try {
            hp        = std::stoi(tokens[2]);
            atk       = std::stoi(tokens[3]);
            def       = std::stoi(tokens[4]);
            mercyGoal = std::stoi(tokens[5]);
        } catch (...) {
            std::cerr << "[AVERTISSEMENT] Ligne " << lineNum
                      << " : statistiques invalides (ignoree)\n";
            continue;
        }

        // Collecte les identifiants d'actions (tokens[6] à tokens[9])
        std::vector<std::string> actIds;
        for (int i = 6; i <= 9; ++i) {
            if (tokens[i] != "-") {
                if (!catalog.hasAction(tokens[i])) {
                    std::cerr << "[AVERTISSEMENT] Action inconnue '"
                              << tokens[i] << "' (ignoree)\n";
                } else {
                    actIds.push_back(tokens[i]);
                }
            }
        }

        // Création polymorphe selon la catégorie
        if (cat == "NORMAL") {
            monsters.push_back(std::make_unique<NormalMonster>(
                nom, hp, atk, def, mercyGoal, actIds));
        } else if (cat == "MINIBOSS") {
            monsters.push_back(std::make_unique<MiniBoss>(
                nom, hp, atk, def, mercyGoal, actIds));
        } else if (cat == "BOSS") {
            monsters.push_back(std::make_unique<Boss>(
                nom, hp, atk, def, mercyGoal, actIds));
        } else {
            std::cerr << "[AVERTISSEMENT] Categorie inconnue '"
                      << cat << "' ligne " << lineNum << " (ignoree)\n";
        }
    }

    return monsters;
}
