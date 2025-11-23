// 📁 src/Core/GameEngine.cpp
#include "Core/GameEngine.h"
#include <iostream>
#include <sstream>
#include "Core/Logger.h"

#define SDL_RenderDrawLine SDL_RenderDrawLine_renamed_SDL_RenderLine


namespace Ecosystem {
    namespace Core {

        // 🏗 CONSTRUCTEUR
        GameEngine::GameEngine(const std::string& title, float width, float height)
            : mWindow(title, width, height),
            mEcosystem(width, height, 500),
            mIsRunning(false),
            mIsPaused(false),
            mShowDebug(true),
            mTimeScale(1.0f),
            mAccumulatedTime(0.0f) {}

        // ⚙️ INITIALISATION
        bool GameEngine::Initialize() {
            if (!mWindow.Initialize()) {
                return false;
            }
            mEcosystem.Initialize(20, 5, 30); // 20 herbivores, 5 carnivores, 30 plantes
            mIsRunning = true;
            mLastUpdateTime = std::chrono::high_resolution_clock::now();
            
            ::Ecosystem::Core::Log("✅ Moteur de jeu initialisé");
            return true;
        }

        // 🎮 BOUCLE PRINCIPALE
        void GameEngine::Run() {
            ::Ecosystem::Core::Log("🎯 Démarrage de la boucle de jeu...");
            
            while (mIsRunning) {
                auto currentTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> elapsed = currentTime - mLastUpdateTime;
                mLastUpdateTime = currentTime;
                
                float deltaTime = elapsed.count();
                
                HandleEvents();
                
                if (!mIsPaused) {
                    Update(deltaTime * mTimeScale);
                }

                Render();

                // Limitation à ~60 FPS
                SDL_Delay(16);
            }
        }

        // 🧹 FERMETURE
        void GameEngine::Shutdown() {
            mIsRunning = false;
            ::Ecosystem::Core::Log("🔄 Moteur de jeu arrêté");
        }

        // 🎮 GESTION DES ÉVÉNEMENTS
        void GameEngine::HandleEvents() {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_EVENT_QUIT:
                        mIsRunning = false;
                        break;

                    case SDL_EVENT_KEY_DOWN:
                        HandleInput(event.key.key);
                        break;
                }
            }
        }

        // ⌨️ GESTION DES TOUCHES
        void GameEngine::HandleInput(SDL_Keycode key) {
            switch (key) {
                case SDLK_ESCAPE:
                    mIsRunning = false;
                    break;

                case SDLK_SPACE:
                    mIsPaused = !mIsPaused;
                    ::Ecosystem::Core::Log(mIsPaused ? "⏸️ Simulation en pause" : "▶️ Simulation reprise");
                    break;

                case SDLK_R:
                    mEcosystem.Initialize(20, 5, 30);
                    ::Ecosystem::Core::Log("🔄 Simulation réinitialisée");
                    break;

                case SDLK_F:
                    mEcosystem.SpawnFood(10);
                    ::Ecosystem::Core::Log("🍎 Nourriture ajoutée");
                    break;

                case SDLK_UP:
                    mTimeScale *= 1.5f;
                    ::Ecosystem::Core::Log(std::string("⏩ Vitesse: ") + std::to_string(mTimeScale) + "x");
                    break;

                case SDLK_DOWN:
                    mTimeScale /= 1.5f;
                    ::Ecosystem::Core::Log(std::string("⏪ Vitesse: ") + std::to_string(mTimeScale) + "x");
                    break;

                case 'g':
                case 'G':
                    mShowDebug = !mShowDebug;
                    ::Ecosystem::Core::Log(std::string("🔧 Debug overlay: ") + (mShowDebug ? "ON" : "OFF"));
                    break;
            }
        }

        // 🔄 MISE À JOUR
        void GameEngine::Update(float deltaTime) {
            mEcosystem.Update(deltaTime);

            // Affichage occasionnel des statistiques
            static float statsTimer = 0.0f;
            statsTimer += deltaTime;
            if (statsTimer >= 2.0f) {
                auto stats = mEcosystem.GetStatistics();
                ::Ecosystem::Core::Log(std::string("📊 Stats - Herbivores: ") + std::to_string(stats.totalHerbivores)
                          + ", Carnivores: " + std::to_string(stats.totalCarnivores)
                          + ", Plantes: " + std::to_string(stats.totalPlants)
                          + ", Naissances: " + std::to_string(stats.birthsToday)
                          + ", Morts: " + std::to_string(stats.deathsToday));
                statsTimer = 0.0f;
            }
        }

        // 🎨 RENDU
        void GameEngine::Render() {
            mWindow.Clear();

            // Rendu de l'écosystème
            mEcosystem.Render(mWindow.GetRenderer());

            // Ici on ajouterait l'interface utilisateur
            RenderUI();

            mWindow.Present();
        }

        // 📊 INTERFACE UTILISATEUR
        void GameEngine::RenderUI() {
            if (!mShowDebug) return;

            SDL_Renderer* renderer = mWindow.GetRenderer();
            if (!renderer) return;

            // Grid parameters
            const int spacing = 50;
            int w = static_cast<int>(mWindow.GetWidth());
            int h = static_cast<int>(mWindow.GetHeight());

            // Grid color (semi-dark)
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 120);
            for (int x = 0; x <= w; x += spacing) {
                SDL_RenderDrawLine(renderer, x, 0, x, h);
            }
            for (int y = 0; y <= h; y += spacing) {
                SDL_RenderDrawLine(renderer, 0, y, w, y);
            }

            // Draw origin cross (top-left)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawLine(renderer, 0, 0, 10, 0);
            SDL_RenderDrawLine(renderer, 0, 0, 0, 10);

            // Draw center cross
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderDrawLine(renderer, w/2 - 10, h/2, w/2 + 10, h/2);
            SDL_RenderDrawLine(renderer, w/2, h/2 - 10, w/2, h/2 + 10);
        }

    } // namespace Core
} // namespace Ecosystem