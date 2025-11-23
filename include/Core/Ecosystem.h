// 📁 include/Core/Ecosystem.hpp
#pragma once

#include "Entity.h"
#include "Structs.h"
#include <vector>
#include <memory>
#include <random>

namespace Ecosystem {
    namespace Core {
        class Ecosystem {
            private:
                // 🔒 ÉTAT INTERNE
                std::vector<std::unique_ptr<Entity>> mEntities;
                std::vector<Food> mFoodSources;
                float mWorldWidth;
                float mWorldHeight;
                int mMaxEntities;
                int mDayCycle;

                // 🎲 Générateur aléatoire
                mutable std::mt19937 mRandomGenerator;

                // 📊 STATISTIQUES
                struct Statistics {
                    int totalHerbivores;
                    int totalCarnivores;
                    int totalPlants;
                    int totalFood;
                    int deathsToday;
                    int birthsToday;
                } mStats;

            public:
                // 🏗 CONSTRUCTEUR/DESTRUCTEUR
                Ecosystem(float width, float height, int maxEntities = 500);
                ~Ecosystem();

                // ⚙️ MÉTHODES PUBLIQUES
                void Initialize(int initialHerbivores, int initialCarnivores, int initialPlants);
                void Update(float deltaTime);
                void SpawnFood(int count);
                void RemoveDeadEntities();
                void HandleReproduction();
                void HandleEating();

                // 📊 GETTERS
                int GetEntityCount() const { return mEntities.size(); }
                int GetFoodCount() const { return mFoodSources.size(); }
                Statistics GetStatistics() const { return mStats; }
                float GetWorldWidth() const { return mWorldWidth; }
                float GetWorldHeight() const { return mWorldHeight; }
                
                // 🎯 MÉTHODES DE GESTION
                void AddEntity(std::unique_ptr<Entity> entity);
                void AddFood(Vector2D position, float energy = 25.0f);
                
                // 🎨 RENDU
                void Render(SDL_Renderer* renderer) const;

            private:
                // 🔐 MÉTHODES PRIVÉES
                void UpdateStatistics();
                void SpawnRandomEntity(EntityType type);
                Vector2D GetRandomPosition() const;
                void HandlePlantGrowth(float deltaTime);
        }; // class Ecosystem
        
    } // namespace Core
} // namespace Ecosystem