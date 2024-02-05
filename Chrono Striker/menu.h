#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"

class Menu {
private:
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;

    GameManager* gameManager;
    bool startGameFlag;
    bool loadingGameFlag;
    bool instructionsRunning = false;

    int selectedOption;
    bool startGame = false;

    const int optionWidth = 200;
    const int optionHeight = 40;

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Texture* menuTexture;
    SDL_Texture* instructionTexture;

public:
    Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight)
        : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight), selectedOption(0), menuTexture(nullptr), gameManager(nullptr), startGameFlag(false), loadingGameFlag(false) {
        loadTexture();
        instructionTexture = loadTextureFromFile("img/instruction.png");
    }

    void setGameManager(GameManager* manager) {
        gameManager = manager;
    }

    bool shouldStartGame() const {
        return startGameFlag;
    }

    bool shouldLoadGame() {
        return loadingGameFlag;
    }

    bool getInstructionBool() {
        return instructionsRunning;
    }

    void loadTexture() {
        menuTexture = loadTextureFromFile("img/menu.png");
        if (!menuTexture) {
            std::cerr << "Error loading menu texture\n";
        }
    }

    SDL_Texture* loadTextureFromFile(const char* filePath) {
        SDL_Surface* surface = IMG_Load(filePath);
        if (!surface) {
            std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        return texture;
    }

    void handleInput(SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_p:
                instructionsRunning = true;
                break;
            case SDLK_l:
                loadingGameFlag = true;
                break;
            case SDLK_e:
                exitGame();
                break;
            default:
                break;
            }
        }
    }

    //handling instructions screen
    void instructionsLoop() {
        SDL_Event event;
        while (instructionsRunning) {
            handleInstructionsInput();
            renderInstructions();
        }
    }

    void handleInstructionsInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                cleanup();
                exit(0);
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_p) {
                    instructionsRunning = false;
                    startGameFlag = true;
                }
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    cleanup();
                    exit(0);
                }
            }
        }
    }

    //rendering elements
    void render() {
        SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    void renderInstructions() {
        SDL_RenderCopy(renderer, instructionTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    //cleanup and exit game
    void exitGame() {
        cleanup();
        exit(0);
    }

    void cleanup() {
        SDL_DestroyTexture(menuTexture);
    }
};

#endif
