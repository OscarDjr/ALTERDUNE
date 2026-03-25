cmake_minimum_required(VERSION 3.28)
project(ALTERDUNE)

set(CMAKE_CXX_STANDARD 20)

# On dit à CLion d'aller chercher les .h dans le dossier include
include_directories(include)

# On liste tous les .cpp avec leur chemin src/
add_executable(ALTERDUNE
        src/main.cpp
        src/Entity.cpp
        src/Player.cpp
        src/Monster.cpp
        src/ActAction.cpp
        src/Inventory.cpp
        src/FileLoader.cpp
        src/Game.cpp
)