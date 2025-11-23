# 🌍 Simulateur d'Écosystème Intelligent

## Description

Simulation d'écosystème en C++ avec SDL3 mettant en oeuvre la POO (Programmation Orientée Objet) avancée


## Compilation

```bash
# Avec g++
mkdir -p bin
g++ -std=c++17 -O2 -Iinclude src/main.cpp src/Core/*.cpp src/Graphics/*.cpp -lSDL3 -o bin/ecosystem.exeGraphics/*.cpp -
# Avec clang++
mkdir -p bin
clang++ -std=c++17 -O2 -Iinclude src/main.cpp src/Core/*.cpp src/Graphics/*.cpp -lSDL3 -o bin/ecosystem.exe