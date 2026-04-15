#include <iostream>
#include <windows.h>
#include "Game.h"

int main() {
    // Permet oublier vieux format et utilise l'UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // Entree clavier soit aussi en UTF-8

    Game game;
    game.run();
    return 0;
}