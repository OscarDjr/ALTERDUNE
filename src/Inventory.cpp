#include "Item.h"
#include "Player.h"
#include <iostream>

void Inventory::addItem(const Item& item, int qty) {
    // Si l'item existe déjà, on augmente la quantité
    for (auto& entry : entries) {
        if (entry.item.name == item.name) {
            entry.qty += qty;
            return;
        }
    }
    entries.push_back({item, qty});
}

bool Inventory::useItem(int index, Player& player) {
    if (index < 0 || index >= (int)entries.size()) return false;
    auto& entry = entries[index];
    if (entry.qty <= 0) {
        std::cout << "Plus de " << entry.item.name << " disponible.\n";
        return false;
    }
    if (entry.item.type == ItemType::HEAL) {
        player.heal(entry.item.value);
        std::cout << "Vous utilisez " << entry.item.name
                  << " et recuperez " << entry.item.value << " HP.\n";
    }
    --entry.qty;
    if (entry.qty == 0)
        entries.erase(entries.begin() + index);
    return true;
}

void Inventory::display() const {
    if (entries.empty()) {
        std::cout << "  (inventaire vide)\n";
        return;
    }
    for (int i = 0; i < (int)entries.size(); ++i) {
        const auto& e = entries[i];
        std::cout << "  [" << i + 1 << "] " << e.item.name
                  << " x" << e.qty
                  << " | Soin : " << e.item.value << " HP\n";
    }
}

bool Inventory::isEmpty() const { return entries.empty(); }
int  Inventory::size()    const { return (int)entries.size(); }

const std::vector<InventoryEntry>& Inventory::getEntries() const {
    return entries;
}
