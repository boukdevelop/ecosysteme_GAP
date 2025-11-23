// 📁 src/main.cpp
#include "Core/GameEngine.h"
#include <iostream>
#include "Core/Logger.h"

int main(int argc, char* argv[]) {

    // Possibilité de désactiver les logs avec l'argument -q
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-q" || a == "--quiet") Ecosystem::Core::gVerbose = false;
    }

    ::Ecosystem::Core::Log("🎮 Démarrage du Simulateur d'Écosystème");
    ::Ecosystem::Core::Log("=======================================");
    
    // 🏗 Création du moteur de jeu
    Ecosystem::Core::GameEngine engine("Simulateur d'Écosystème Intelligent", 1200.0f, 1400.0f);
    
        // ⚙️ Initialisation
    if (!engine.Initialize()) {
        std::cerr << "❌ Erreur: Impossible d'initialiser le moteur de jeu" << std::endl;
        return -1;
    }

    ::Ecosystem::Core::Log("✅ Moteur initialisé avec succès");
    ::Ecosystem::Core::Log("🎯 Lancement de la simulation...");
    ::Ecosystem::Core::Log("=== CONTRÔLES ===");
    ::Ecosystem::Core::Log("ESPACE: Pause/Reprise");
    ::Ecosystem::Core::Log("R: Reset simulation");
    ::Ecosystem::Core::Log("F: Ajouter nourriture");
    ::Ecosystem::Core::Log("FLÈCHES: Vitesse simulation");
    ::Ecosystem::Core::Log("ÉCHAP: Quitter");
    
    // 🎮 Boucle principale
    engine.Run();

    // 🛑 Arrêt propre
    engine.Shutdown();

    ::Ecosystem::Core::Log("👋 Simulation terminée. Au revoir !");
    return 0;
}