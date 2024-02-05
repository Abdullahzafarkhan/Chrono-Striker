#include <iostream>
#include <SDL.h>
#include "Menu.h" 
#include "game.h" 

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Chrono Striker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1360, 768, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!window || !renderer) {
        std::cerr << "SDL_CreateWindow or SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Menu menu(renderer, 1360, 768);
    GameManager gameManager;

    if (!gameManager.initialize(window, renderer)) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    menu.setGameManager(&gameManager);

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            else {
                if (menu.shouldStartGame()) {
                    gameManager.handleInput();
                }
                else {
                    menu.handleInput(event);
                }
            }
        }

        menu.render();

        if (menu.getInstructionBool()) {
            menu.instructionsLoop();
        }

        if (menu.shouldStartGame()) {
            gameManager.runGameLoop();

            menu.cleanup();
        }

        if (menu.shouldLoadGame()) {
            gameManager.loadGame("savegame.txt");
            gameManager.runGameLoop();
            menu.cleanup();
        }
    }

    gameManager.cleanup();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
