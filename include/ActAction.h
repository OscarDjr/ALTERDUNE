#pragma once
#include <string>
#include <map>

struct ActAction {
    std::string id;
    std::string text;
    int mercyImpact;

    ActAction() = default;
    ActAction(const std::string& id, const std::string& text, int impact)
        : id(id), text(text), mercyImpact(impact) {}
};

class ActionCatalog {
private:
    std::map<std::string, ActAction> actions;
public:
    ActionCatalog();
    bool hasAction(const std::string& id) const;
    const ActAction& getAction(const std::string& id) const;
    void displayAll() const;
};
