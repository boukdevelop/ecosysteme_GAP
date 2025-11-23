#pragma once

#include "Structs.h"
#include <SDL3/SDL.h>
#include <memory>
#include <random>
#include <vector>

namespace Ecosystem {
    namespace Core {

        // 🎯 ÉNUMÉRATION DES TYPES D'ENTITÉS
        enum class EntityType {
            HERBIVORE,
            CARNIVORE,
            PLANT
        };

        class Entity {
            private :
                // 🔒 DONNÉES PRIVÉES - État interne protégé
                float mEnergy;
                float mMaxEnergy;
                int mAge;
                int mMaxAge;
                bool mIsAlive;
                Vector2D mVelocity;
                EntityType mType;

                // 🎲 Générateur aléatoire
                mutable std::mt19937 mRandomGenerator;
            
            public:
                    // 🔓 DONNÉES PUBLIQUES - Accès direct sécurisé
                    Vector2D position;
                    Color color;
                    float size;
                    std::string name;
                    // 🏗 CONSTRUCTEURS
                    Entity(EntityType type, Vector2D pos, std::string entityName = "Unnamed");
                    Entity(const Entity& other); // Constructeur de copie

                    // DESTRUCTEURS
                    ~Entity();

                    // ⚙️ MÉTHODES PUBLIQUES
                    void Update(float deltaTime);
                    void Move(float deltaTime);
                    void Eat(float energy);
                    bool CanReproduce() const;
                    std::unique_ptr<Entity> Reproduce();
                    void ApplyForce(Vector2D force);

                    // 📊 GETTERS - Accès contrôlé aux données privées
                    float GetEnergy() const { return mEnergy; }
                    float GetEnergyPercentage() const { return mEnergy / mMaxEnergy; }
                    int GetAge() const { return mAge; }
                    bool IsAlive() const { return mIsAlive; }
                    EntityType GetType() const { return mType; }
                    Vector2D GetVelocity() const { return mVelocity; }

                    // 🎯 MÉTHODES DE COMPORTEMENT
                    Vector2D SeekFood(const std::vector<Food>& foodSources) const;
                    Vector2D AvoidPredators(const std::vector<Entity>& predators) const;
                    Vector2D StayInBounds(float worldWidth, float worldHeight) const;

                    // 🎨 MÉTHODE DE RENDU
                    void Render(SDL_Renderer* renderer) const;

            private:
                // 🔐 MÉTHODES PRIVÉES - Logique interne
                void ConsumeEnergy(float deltaTime);
                void Age(float deltaTime);
                void CheckVitality();
                Vector2D GenerateRandomDirection();
                Color CalculateColorBasedOnState() const;      
        };
    } // namesapce Core
} // namespace Ecosystem