#include <iostream>
#include <windows.h>
#include "Game.h"

int main() {
    // Cette ligne dit à Windows : "Oublie le vieux format, utilise l'UTF-8"
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // Optionnel : pour que l'entrée clavier soit aussi en UTF-8

    Game game;
    game.run();
    return 0;
}