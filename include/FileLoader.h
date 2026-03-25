#pragma once
#include "Item.h"
#include "Monster.h"
#include "ActAction.h"
#include <vector>
#include <memory>
#include <string>

class FileLoader {
public:
    // Charge items.csv -> remplit l'inventaire du joueur
    static void loadItems(const std::string& filename, Inventory& inventory);

    // Charge monsters.csv -> retourne un vecteur de pointeurs polymorphes
    static std::vector<std::unique_ptr<Monster>> loadMonsters(
        const std::string& filename,
        const ActionCatalog& catalog);

private:
    // Découpe une ligne CSV selon le séparateur ';'
    static std::vector<std::string> split(const std::string& line, char sep = ';');
    static std::string trim(const std::string& s);
};
