// 📁 src/Graphics/Window.cpp
#include "Graphics/Window.h"
#include <iostream>
#include "Core/Logger.h"


namespace Ecosystem {
    namespace Graphics {

        // 🏗 CONSTRUCTEUR
        Window::Window(const std::string& title, float width, float height)
        : mTitle(title), mWidth(width), mHeight(height),
        mWindow(nullptr), mRenderer(nullptr), mIsInitialized(false) {}
        
        // 🗑 DESTRUCTEUR
        Window::~Window() {
            Shutdown();
        }

        // ⚙️ INITIALISATION
        bool Window::Initialize() {
            if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                std::cerr << "❌ Erreur SDL_Init: " << SDL_GetError() << std::endl;
                return false;
            }
            mWindow = SDL_CreateWindow(mTitle.c_str(),
                                      static_cast<int>(mWidth),
                                      static_cast<int>(mHeight),
                                      SDL_WINDOW_RESIZABLE);
            if (!mWindow) {
                std::cerr << "❌ Erreur création fenêtre: " << SDL_GetError() << std::endl;
                SDL_Quit();
                return false;
            }
            // SDL3 renderer creation: provide a name (or nullptr)
            mRenderer = SDL_CreateRenderer(mWindow, nullptr);
            if (!mRenderer) {
                std::cerr << "❌ Erreur création renderer: " << SDL_GetError() << std::endl;
                SDL_DestroyWindow(mWindow);
                SDL_Quit();
                return false;
            }

            mIsInitialized = true;
            ::Ecosystem::Core::Log(std::string("✅ Fenêtre initialisée: ") + mTitle + " (" + std::to_string((int)mWidth) + "x" + std::to_string((int)mHeight) + ")");
            return true;
        }

        // 🧹 FERMETURE
        void Window::Shutdown() {
            if (mRenderer) {
                SDL_DestroyRenderer(mRenderer);
                mRenderer = nullptr;
            }

            if (mWindow) {
                SDL_DestroyWindow(mWindow);
                mWindow = nullptr;
            }

            SDL_Quit();
            mIsInitialized = false;
            ::Ecosystem::Core::Log("🔄 Fenêtre fermée");
        }

        // 🎨 NETTOYAGE DE L'ÉCRAN
        void Window::Clear(const Core::Color& color) {
            if (mRenderer) {
                SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
                SDL_RenderClear(mRenderer);
            }
        }

        // 🔄 AFFICHAGE
        void Window::Present() {
            if (mRenderer) {
                SDL_RenderPresent(mRenderer);
            }
        }

    } // namespace Graphics
} // namespace Ecosystem