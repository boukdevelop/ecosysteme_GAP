#pragma once

#include <SDL3/SDL.h>
#include "Core/Structs.h"

namespace Ecosystem {
    namespace Graphics {

        // Petite interface utilitaire pour le rendu (minimal pour SDL3)
        class Renderer {
        public:
            static void DrawFilledRect(SDL_Renderer* renderer, const Core::Vector2D& pos, float w, float h, const Core::Color& color) {
                if (!renderer) return;
                SDL_FRect rect = { pos.x, pos.y, w, h };
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(renderer, &rect);
            }
        };

    } // namespace Graphics
} // namespace Ecosystem
