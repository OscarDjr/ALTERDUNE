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
    // 2 actions NEGATIVES
    actions["INSULT"]      = ActAction("INSULT",      "Tu l'insultes copieusement. Il te fixe, hors de lui.", -30);
    actions["MOCK"]        = ActAction("MOCK",        "Tu te moques de lui. C'etait une erreur. Une grosse erreur.", -20);
    actions["SING"]        = ActAction("SING",        "Tu entonnes une chanson fausse. Le monstre est touche.", +15);
    actions["APOLOGIZE"]   = ActAction("APOLOGIZE",   "Tu t'excuses pour tout et n'importe quoi. Ca desarme.", +20);
    actions["FLEX"]        = ActAction("FLEX",    "Tu fais des poses musculaires. Le monstre ricane.", +10);
    actions["STARE"]       = ActAction("STARE",   "Tu fixes le monstre sans ciller. Malaise total.", +10);
    actions["BOW"]         = ActAction("BOW",     "Tu t'inclines respectueusement. Il apprecie le geste.", +25);
    actions["THREATEN"]    = ActAction("THREATEN","Tu menaces le monstre betement. Il se vexe.", -25);
    actions["YELL"]        = ActAction("YELL",    "Tu cries sans raison. Le monstre est agace.", -15);
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
