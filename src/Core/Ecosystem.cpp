#include "Core/Ecosystem.h"
#include "Core/Logger.h"
#include <algorithm>
#include <iostream>

namespace Ecosystem {
    namespace Core {

        // 🏗 CONSTRUCTEUR
        Ecosystem::Ecosystem(float width, float height, int maxEntities)
        : mWorldWidth(width), mWorldHeight(height), mMaxEntities(maxEntities),
        mDayCycle(0), mRandomGenerator(std::random_device{}())
        {
            // Initialisation des statistiques
            mStats = {0, 0, 0, 0, 0, 0};
                ::Ecosystem::Core::Log(std::string("🌍 Écosystème créé: ") + std::to_string(width) + "x" + std::to_string(height));
        }

        // 🗑 DESTRUCTEUR
        Ecosystem::~Ecosystem() {
              ::Ecosystem::Core::Log(std::string("🌍 Écosystème détruit (") + std::to_string(mEntities.size()) + " entités nettoyé)");
        }

        // ⚙️ INITIALISATION
        void Ecosystem::Initialize(int initialHerbivores, int initialCarnivores, int initialPlants) {
            mEntities.clear();
            mFoodSources.clear();

            // Création des entités initiales
            for (int i = 0; i < initialHerbivores; ++i) {
                SpawnRandomEntity(EntityType::HERBIVORE);
            }

            for (int i = 0; i < initialCarnivores; ++i) {
                SpawnRandomEntity(EntityType::CARNIVORE);
            }

            for (int i = 0; i < initialPlants; ++i) {
                SpawnRandomEntity(EntityType::PLANT);
            }

            // Nourriture initiale
            SpawnFood(20);
                        ::Ecosystem::Core::Log(std::string("🌱 Écosystème initialisé avec ") + std::to_string(mEntities.size()) + " entités");
        }

        // 🔄 MISE À JOUR
        void Ecosystem::Update(float deltaTime) {

            // Mise à jour de toutes les entités
            for (auto& entity : mEntities) {
                entity->Update(deltaTime);
            }

            // Gestion des comportements
            HandleEating();
            HandleReproduction();
            RemoveDeadEntities();
            HandlePlantGrowth(deltaTime);
            
            // Mise à jour des statistiques
            UpdateStatistics();
            mDayCycle++;
        }

        // 🍎 GÉNÉRATION DE NOURRITURE
        void Ecosystem::SpawnFood(int count) {
            for (int i = 0; i < count; ++i) {
                if (mFoodSources.size() < 100) { // Limite maximale de nourriture
                    Vector2D position = GetRandomPosition();
                    mFoodSources.emplace_back(position, 25.0f);
                }
            }
        }

        // 💀 SUPPRESSION DES ENTITÉS MORTES
        void Ecosystem::RemoveDeadEntities() {
            int initialCount = mEntities.size();
            mEntities.erase(
                std::remove_if(mEntities.begin(), mEntities.end(),
                [](const std::unique_ptr<Entity>& entity) {
                    return !entity->IsAlive();
                }),
                mEntities.end()
            );
            int removedCount = initialCount - mEntities.size();
            if (removedCount > 0) {
                mStats.deathsToday += removedCount;
            }
        }

        // 👶 GESTION DE LA REPRODUCTION
        void Ecosystem::HandleReproduction() {
            std::vector<std::unique_ptr<Entity>> newEntities;
            for (auto& entity : mEntities) {
                if (entity->CanReproduce() && mEntities.size() < mMaxEntities) {
                    auto baby = entity->Reproduce();
                    if (baby) {
                        newEntities.push_back(std::move(baby));
                        mStats.birthsToday++;
                    }
                }
            }

            // Ajout des nouveaux entités
            for (auto& newEntity : newEntities) {
                mEntities.push_back(std::move(newEntity));
            }
        }

        // 🍽 GESTION DE L'ALIMENTATION
        void Ecosystem::HandleEating() {
            // Ici on implémenterait la logique de recherche de nourriture
            // Pour l'instant, gestion simplifiée
            for (auto& entity : mEntities) {
                if (entity->GetType() == EntityType::PLANT) {
                    // Les plantes génèrent de l'énergie
                    entity->Eat(0.1f);
                }
            }
        }

        // 📊 MISE À JOUR DES STATISTIQUES
        void Ecosystem::UpdateStatistics() {

            mStats.totalHerbivores = 0;
            mStats.totalCarnivores = 0;
            mStats.totalPlants = 0;
            mStats.totalFood = mFoodSources.size();

            for (const auto& entity : mEntities) {
                switch (entity->GetType()) {
                    case EntityType::HERBIVORE:
                        mStats.totalHerbivores++;
                    break;
                    case EntityType::CARNIVORE:
                        mStats.totalCarnivores++;
                    break;
                    case EntityType::PLANT:
                        mStats.totalPlants++;
                    break;
                }
            }
        }

        // 🎲 CRÉATION D'ENTITÉ ALÉATOIRE
        void Ecosystem::SpawnRandomEntity(EntityType type) {
            if (mEntities.size() >= mMaxEntities) return;

            Vector2D position = GetRandomPosition();
            std::string name;

            switch (type) {
                case EntityType::HERBIVORE:
                    name = "Herbivore_" + std::to_string(mStats.totalHerbivores);
                break;
                case EntityType::CARNIVORE:
                    name = "Carnivore_" + std::to_string(mStats.totalCarnivores);
                break;
                case EntityType::PLANT:
                    name = "Plant_" + std::to_string(mStats.totalPlants);
                break;
            }

            mEntities.push_back(std::make_unique<Entity>(type, position, name));
        }

        // 🎯 POSITION ALÉATOIRE
        Vector2D Ecosystem::GetRandomPosition() const {

            std::uniform_real_distribution<float> distX(0.0f, mWorldWidth);
            std::uniform_real_distribution<float> distY(0.0f, mWorldHeight);
            
            return Vector2D(distX(mRandomGenerator), distY(mRandomGenerator));
        }

        // 🌿 CROISSANCE DES PLANTES
        void Ecosystem::HandlePlantGrowth(float deltaTime) {

            // Occasionnellement, faire pousser de nouvelles plantes
            std::uniform_real_distribution<float> chance(0.0f, 1.0f);
            
            if (chance(mRandomGenerator) < 0.01f && mEntities.size() < mMaxEntities) {
                SpawnRandomEntity(EntityType::PLANT);
            }
        }

        // 🎨 RENDU
        void Ecosystem::Render(SDL_Renderer* renderer) const {
            // Rendu de la nourriture
            for (const auto& food : mFoodSources) {
                SDL_FRect rect = { food.position.x - 3.0f, food.position.y - 3.0f, 6.0f, 6.0f };
                SDL_SetRenderDrawColor(renderer, food.color.r, food.color.g, food.color.b, food.color.a);
                SDL_RenderFillRect(renderer, &rect);
            }
            
            // Rendu des entités
            for (const auto& entity : mEntities) {
                entity->Render(renderer);
            }
        }

    } // namespace Core
} // namespace Ecosystem