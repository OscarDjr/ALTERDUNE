#include "ActAction.h"
#include <iostream>
#include <stdexcept>

ActionCatalog::ActionCatalog() {
    actions["JOKE"]        = ActAction("JOKE",        "Tu racontes une blague sur les algorithmes. Le monstre rit... puis pleure.", +20);
    actions["COMPLIMENT"]  = ActAction("COMPLIMENT",  "Tu complimentes son pelage/ecailles. Il rougit visiblement.", +25);
    actions["DISCUSS"]     = ActAction("DISCUSS",     "Vous engagez une grande discussion philosophique. Le monstre semble apaise.", +15);
    actions["PET"]         = ActAction("PET",         "Tu tentes de le caresser. Contre toute attente, il apprecie.", +30);
    actions["DANCE"]       = ActAction("DANCE",       "Tu danses le macarena. Le monstre applaudit maladroitement.", +20);
    actions["OFFER_SNACK"] = ActAction("OFFER_SNACK", "Tu lui offres un snack mysterieux. Il hesite... puis accepte.", +35);
    actions["OBSERVE"]     = ActAction("OBSERVE",     "Tu l'observes en silence. Il se sent compris.", +10);
    actions["REASON"]      = ActAction("REASON",      "Tu lui expliques calmement que la violence ne resout rien.", +15);
    // 2 actions a impact NEGATIF (obligatoires)
    actions["INSULT"]      = ActAction("INSULT",      "Tu l'insultes copieusement. Il te fixe, hors de lui.", -30);
    actions["MOCK"]        = ActAction("MOCK",        "Tu te moques de lui. C'etait une erreur. Une grosse erreur.", -20);
}

bool ActionCatalog::hasAction(const std::string& id) const {
    return actions.count(id) > 0;
}

const ActAction& ActionCatalog::getAction(const std::string& id) const {
    auto it = actions.find(id);
    if (it == actions.end())
        throw std::runtime_error("Action inconnue : " + id);
    return it->second;
}

void ActionCatalog::displayAll() const {
    for (const auto& [id, act] : actions)
        std::cout << "  [" << id << "] " << act.text
                  << " (Mercy " << (act.mercyImpact >= 0 ? "+" : "") << act.mercyImpact << ")\n";
}
