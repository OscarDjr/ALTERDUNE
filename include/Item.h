#pragma once
#include <string>
#include <vector>

enum class ItemType { HEAL };

struct Item {
    std::string name;
    ItemType    type;
    int         value;

    Item() = default;
    Item(const std::string& name, ItemType type, int value)
        : name(name), type(type), value(value) {}
};

struct InventoryEntry {
    Item item;
    int  qty;
};

// Forward declaration
class Player;

class Inventory {
private:
    std::vector<InventoryEntry> entries;
public:
    void addItem(const Item& item, int qty);
    bool useItem(int index, Player& player); // true si succès
    void display() const;
    bool isEmpty() const;
    int  size()    const;
    const std::vector<InventoryEntry>& getEntries() const;
};
