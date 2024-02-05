#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector> 
#include <string>
#include "menu.h"
#include "player.h"
#include "enemy.h"
#include "pickup.h"
#include <fstream>
using namespace std;

class GameManager {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* npcInteractionTexture;
    SDL_Texture* npcOne;
    SDL_Texture* missionOneTexture;
    SDL_Texture* bulletTexture;
    SDL_Texture* enemyOneTexture;
    SDL_Texture* enemyTwoTexture;
    SDL_Texture* weaponPickupTexture;
    SDL_Texture* finalMissionTexture;
    SDL_Texture* finalBossTexture;
    SDL_Texture* gameOverTexture;
    SDL_Rect laboratoryRect;
    SDL_Rect townhallRect;
    SDL_Rect chatAreaRect;
    SDL_Rect npcRect;

    bool isLabRunning = false;
    bool isGameOver = false;
    bool isBulletFired = false;
    bool enemyFire = false;
    bool waveOneCompleted = false;
    bool waveTwoCompleted = false;
    bool npcInteractStatus = false;
    bool delay = false;
    bool pickupStop = false;
    bool finalMissionRunning = false;
    bool finalBossMovement = false;
    bool gameOverScreen = false;
    int npcScreen = 0;

    SDL_Color textColor = { 255, 255, 255, 255 };
    vector<Enemy> enemiesWaveOne;
    vector<Enemy> enemiesWaveTwo;
    Enemy* finalBoss;
    int enemyKillCount = 0;

    Player player;
    Pickup* weaponPickup;
    TTF_Font* font;

    bool loadFont(const char* fontFilePath, int fontSize) {
        font = TTF_OpenFont(fontFilePath, fontSize);
        if (!font) {
            cerr << "TTF_OpenFont Error: " << TTF_GetError() << endl;
            return false;
        }
        return true;
    }

    void renderText(const string& text, int x, int y, const SDL_Color& color) const {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface) {
            cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << endl;
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
            return;
        }

        SDL_Rect textRect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        SDL_DestroyTexture(texture);
    }

    //these two functions check for intersection between bullets and characters
    //characters include player and enemies
    bool checkBulletEnemyCollision(Bullet* bullet, const Enemy& enemy) {
        return SDL_HasIntersection(&bullet->position, &enemy.position);
    }

    bool checkEnemyBulletCollision(EnemyBullet* bullet, Player& player) {
        return SDL_HasIntersection(&bullet->position, &player.position);
    }

public:
    GameManager() : window(nullptr), renderer(nullptr), player(350, { 0, 500, 80, 150 }, nullptr) {
        laboratoryRect = { 300, 500, 400, 300 };
        townhallRect = { 900,300, 400, 300 };
        chatAreaRect = { 400, 0, 400, 300 };
    }

    //these 3 functions are used to initialize the game
    bool initialize(SDL_Window* window, SDL_Renderer* renderer) {
        this->window = window;
        this->renderer = renderer;

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            cerr << "Mix_OpenAudio Error: " << Mix_GetError() << endl;
            return false;
        }

        Mix_Music* backgroundMusic = Mix_LoadMUS("music/bg_music.mp3");
        if (!backgroundMusic) {
            cerr << "Mix_LoadMUS Error: " << Mix_GetError() << endl;
            return false;
        }

        Mix_PlayMusic(backgroundMusic, -1);
        
        if (TTF_Init() != 0) {
            cerr << "TTF_Init Error: " << TTF_GetError() << endl;
            return false;
        }
        
        if (!loadFont("fonts/Squartiqa4F.ttf", 30)) {
            return false;
        }

        player.texture = loadTexture("img/player-1.png");
        bulletTexture = loadTexture("img/laser.png");
        backgroundTexture = loadTexture("img/game.png");
        missionOneTexture = loadTexture("img/location-1.png");
        player.setBullet(10, { player.position.x + 50, player.position.y + 50, 50, 10 }, bulletTexture);
        enemyOneTexture = loadTexture("img/enemy-1.png");
        enemyTwoTexture = loadTexture("img/enemy-2.png");
        weaponPickupTexture = loadTexture("img/player-2.png");
        finalMissionTexture = loadTexture("img/bg1.png");
        finalBossTexture = loadTexture("img/final-boss.png");
        gameOverTexture = loadTexture("img/game-over.png");
        npcInteractionTexture = loadTexture("img/interact-1.png");
        npcOne = loadTexture("img/npc1.png");
        npcRect = { 500, 50, 100, 130 };

        weaponPickup = new Pickup({ 680, 0, 140, 180 }, weaponPickupTexture);
        finalBoss = new Enemy(500, { 1100, 500, 160, 220 }, finalBossTexture, 30, bulletTexture);
        setEnemiesWaveOne();
        setEnemiesWaveTwo();
        return true;
    }

    void setEnemiesWaveOne() {
        enemiesWaveOne.push_back(Enemy(100, { 1100, 450, 100, 100 }, enemyOneTexture, 3, bulletTexture));
        enemiesWaveOne.push_back(Enemy(100, { 1000, 470, 100, 100 }, enemyOneTexture, 3, bulletTexture));
        enemiesWaveOne.push_back(Enemy(100, { 900, 500, 100, 100 }, enemyOneTexture, 3, bulletTexture));
        enemiesWaveOne.push_back(Enemy(100, { 800, 530, 100, 100 }, enemyOneTexture, 3, bulletTexture));
        enemiesWaveOne.push_back(Enemy(100, { 900, 560, 100, 100 }, enemyOneTexture, 3, bulletTexture));
        enemiesWaveOne.push_back(Enemy(100, { 1000, 590, 100, 100 }, enemyOneTexture, 3, bulletTexture));
        enemiesWaveOne.push_back(Enemy(100, { 1100, 620, 100, 100 }, enemyOneTexture, 3, bulletTexture));
        enemiesWaveOne.push_back(Enemy(100, { 1200, 500, 100, 100 }, enemyOneTexture, 3, bulletTexture));
        enemiesWaveOne.push_back(Enemy(100, { 1200, 560, 100, 100 }, enemyOneTexture, 3, bulletTexture));
    }
    
    void setEnemiesWaveTwo() {
        enemiesWaveTwo.push_back(Enemy(100, { 1100, 450, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
        enemiesWaveTwo.push_back(Enemy(100, { 1000, 470, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
        enemiesWaveTwo.push_back(Enemy(100, { 900, 500, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
        enemiesWaveTwo.push_back(Enemy(100, { 800, 530, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
        enemiesWaveTwo.push_back(Enemy(100, { 900, 560, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
        enemiesWaveTwo.push_back(Enemy(100, { 1000, 590, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
        enemiesWaveTwo.push_back(Enemy(100, { 1100, 620, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
        enemiesWaveTwo.push_back(Enemy(100, { 1200, 500, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
        enemiesWaveTwo.push_back(Enemy(100, { 1200, 560, 100, 100 }, enemyTwoTexture, 8, bulletTexture));
    }

    //background Render Helper
    void renderBackground() const {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    }

    SDL_Texture* loadTexture(const char* filePath) {
        SDL_Surface* surface = IMG_Load(filePath);
        if (!surface) {
            cerr << "IMG_Load Error: " << IMG_GetError() << endl;
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
            return nullptr;
        }

        return texture;
    }

    //cleanup function
    void cleanup() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    //central game loop
    void runGameLoop() {
        while (!isGameOver) {
            handleInput();
            renderGame();
            SDL_Delay(16);
        }
    }

    //game input handler
    //also help direct the game flow
    void handleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                cleanup();
                exit(0);
            }
            else if (event.type == SDL_KEYDOWN) {

                if (event.key.keysym.sym == SDLK_LEFT) {
                    player.moveLeft();
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    player.moveRight();
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    player.moveUp();
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    player.moveDown();
                }

                if (event.key.keysym.sym == SDLK_e) {
                    if (SDL_HasIntersection(&player.position, &chatAreaRect) && !npcInteractStatus) {
                        this->runNPCLoop();
                    }
                    if (SDL_HasIntersection(&player.position, &laboratoryRect) && !player.weaponPickup && npcInteractStatus) {
                        isLabRunning = true;
                        this->runLabGameLoop();
                    }
                    if (SDL_HasIntersection(&player.position, &townhallRect) && waveOneCompleted && waveTwoCompleted) {
                        finalMissionRunning = true;
                        player.position.x = 120;
                        player.position.y = 500;
                        this->runFinalMissionLoop();
                    }
                }

                if (event.key.keysym.sym == SDLK_s) {
                    this->saveGame("saveGame.txt");
                }
            }
        }
    }

    //rendering game elements
    void renderGame() {
        SDL_RenderClear(renderer);
        renderBackground();
        player.render(renderer,isBulletFired);
        SDL_RenderCopy(renderer, npcOne, nullptr, &npcRect);
        SDL_RenderPresent(renderer);
    }

/*------------------------------------------NPC Interaction-------------------------------------------------------------*/
    //handling npc interaction
    void runNPCLoop() {
        while (!npcInteractStatus) {
            handleNPCInput();
            updateNPCLogic();
            renderNPCInteraction();
            SDL_Delay(16);
        }
    }

    void handleNPCInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                cleanup();
                exit(0);
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_e) {
                    npcScreen += 1;
                }
            }
        }
    }

    //updating npc elements
    void updateNPCLogic() {
        if (npcScreen == 1) {
            npcInteractionTexture = loadTexture("img/interact-2.png");
        }
        else if(npcScreen >= 2) {
            npcInteractStatus = true;
        }
        else {
            npcInteractionTexture = loadTexture("img/interact-1.png");
        }
    }

    //rendering game elements
    void renderNPCInteraction() {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, npcInteractionTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

/*---------------------------------------Laboratory Mission-------------------------------------------------------------*/
    //laboratory mission handling (enemy waves)
    void runLabGameLoop() {
        while (isLabRunning) {
            handleLabInput();
            updateLabGameLogic();
            renderLabGame();
            SDL_Delay(16);  // Adjust as needed
        }
    }

    void handleLabInput() {
        SDL_Event labEvent;
        bool fire = false;
        while (SDL_PollEvent(&labEvent)) {
            if (labEvent.type == SDL_QUIT) {
                isLabRunning = false;
            }
            else if (labEvent.type == SDL_KEYDOWN) {
                if (labEvent.key.keysym.sym == SDLK_r) {
                    isLabRunning = false;
                }
                if (labEvent.key.keysym.sym == SDLK_LEFT) {
                    player.moveLeft();
                }
                if (labEvent.key.keysym.sym == SDLK_RIGHT) {
                    player.moveRight();
                }
                if (labEvent.key.keysym.sym == SDLK_UP) {
                    player.moveUp();
                }
                if (labEvent.key.keysym.sym == SDLK_DOWN) {
                    player.moveDown();
                }
                if (labEvent.key.keysym.sym == SDLK_SPACE) {
                    isBulletFired = true;
                }
                
            }
            
        }
    }

    //these two functions handle collision of player bullet
    //with enemies
    void handleWaveOneCollision() {
        for (auto& enemy : enemiesWaveOne) {
            if (checkBulletEnemyCollision(player.bullet, enemy)) {
                enemy.position.x = -10000;
                player.bullet->position.x = -1000;
                enemyKillCount += 1;
                isBulletFired = false;
            }
        }
    }

    void handleWaveTwoCollision() {
        for (auto& enemy : enemiesWaveTwo) {
            if (checkBulletEnemyCollision(player.bullet, enemy)) {
                enemy.position.x = -10000;
                player.bullet->position.x = -1000;
                enemyKillCount += 1;
                isBulletFired = false;
            }
        }
    }

    //these two functions handle collision of enemy bullets
    //with player
    void handleEnemyBulletCollisionOne() {
        for (auto& enemy : enemiesWaveOne) {
            if (checkEnemyBulletCollision(enemy.bullet, player)) {
                player.health -= enemy.bullet->damage;
                enemy.bullet->position.x = -1000;
                enemyFire = false;
            }
        }
    }

    void handleEnemyBulletCollisionTwo() {
        for (auto& enemy : enemiesWaveTwo) {
            if (checkEnemyBulletCollision(enemy.bullet, player)) {
                player.health -= enemy.bullet->damage;
                enemy.bullet->position.x = -1000;
                enemyFire = false;
            }
        }
    }

    //updating lab mission
    void updateLabGameLogic() {
        if (delay) {
            SDL_Delay(3000);
            delay = !delay;
        }

        if (player.weaponPickup) {
            player.position.x = 120;
            player.position.y = 460;
            isLabRunning = false;
        }

        if (player.health <= 0) {
            player.position.x = -10000;
            player.position.y = -10000;
            gameOverScreen = true;
            gameOverTexture = loadTexture("img/game-lost.png");
            this->runGameOverLoop();
        }

        if (isBulletFired) {
            player.bullet->move(player.position.x, player.position.y,isBulletFired);
            if (!waveOneCompleted) {
                handleWaveOneCollision();
            }
            else if (!waveTwoCompleted) {
                handleWaveTwoCollision();
            }
        }
        else {
            player.bullet->setPosition(player.position.x + 50, player.position.y + 50);
        }

        if (!waveOneCompleted) {
            for (auto& enemy : enemiesWaveOne) {
                enemy.bullet->move(enemy.position.x, enemy.position.y, enemyFire);
            }
            handleEnemyBulletCollisionOne();
        }
        else if (!waveTwoCompleted) {
            for (auto& enemy : enemiesWaveTwo) {
                enemy.bullet->move(enemy.position.x, enemy.position.y, enemyFire);
            }
            handleEnemyBulletCollisionTwo();
        }

        if (!waveOneCompleted && enemyKillCount == 9) {
            waveOneCompleted = true;
            player.health += 50;
            delay = !delay;
        }
        if (!waveTwoCompleted && enemyKillCount == 18) {
            waveTwoCompleted = true;
            player.health = 250;
            delay = !delay;
        }
        if (waveOneCompleted && waveTwoCompleted) {
            if (!pickupStop) {
                weaponPickup->move(pickupStop);
            }
            if (SDL_HasIntersection(&player.position, &weaponPickup->position)) {
                weaponPickup->position.x = -10000;
                player.texture = loadTexture("img/player-2.png");
                player.setHeightAndWidth(140,180);
                player.health = 500;
                player.bullet->damage = 40;
                player.weaponPickup = true;
                delay = !delay;
            }
        }
    }

    //rendering the lab mission elements
    void renderLabGame() {
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, missionOneTexture, nullptr, nullptr);
        player.render(renderer,isBulletFired);
        renderText("Player Health: " + to_string(player.health), 10, 0, textColor);
        if (!waveOneCompleted) {
            for (auto& enemy : enemiesWaveOne) {
                enemy.render(renderer, enemyFire);
            }
        }
        else if (!waveTwoCompleted) {
            for (auto& enemy : enemiesWaveTwo) {
                enemy.render(renderer, enemyFire);
            }
        }
        else {
            weaponPickup->render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

/*---------------------------------------Final Mission-------------------------------------------------------------*/
    //final mission handling
    void runFinalMissionLoop() {
        while (finalMissionRunning) {
            handleFinalMissionInput();
            updateFinalMissionLogic();
            renderFinalMission();
            SDL_Delay(16);
        }
    }

    void handleFinalMissionInput() {
        SDL_Event finalMissionEvent;
        bool fire = false;
        while (SDL_PollEvent(&finalMissionEvent)) {
            if (finalMissionEvent.type == SDL_QUIT) {
                finalMissionRunning = false;
            }
            else if (finalMissionEvent.type == SDL_KEYDOWN) {
                if (finalMissionEvent.key.keysym.sym == SDLK_r) {
                    finalMissionRunning = false;
                }
                if (finalMissionEvent.key.keysym.sym == SDLK_UP) {
                    player.finalBattleMoveUp();
                }
                if (finalMissionEvent.key.keysym.sym == SDLK_DOWN) {
                    player.finalBattleMoveDown();
                }
                if (finalMissionEvent.key.keysym.sym == SDLK_SPACE) {
                    isBulletFired = true;
                }

            }

        }
    }

    //this functions handle collision of player bullet
    //with enemy
    void handlePlayerBulletCollision() {
        if (SDL_HasIntersection(&player.bullet->position, &finalBoss->position)) {
            finalBoss->health -= player.bullet->damage;
            player.bullet->position.x = -1000;
            isBulletFired = false;
        }
    }

    //this functions handle collision of enemy bullets
    //with player
    void handleEnemyBulletCollision() {
        if (checkEnemyBulletCollision(finalBoss->bullet, player)) {
            player.health -= finalBoss->bullet->damage;
            finalBoss->bullet->position.x = -1000;
            enemyFire = false;
        }
    }
                            
    //updating final mission elements
    void updateFinalMissionLogic() {
        if (delay) {
            SDL_Delay(3000);
            delay = !delay;
        }

        if (player.health <= 0) {
            player.position.x = -10000;
            player.position.y = -10000;
            finalMissionRunning = false;
            gameOverScreen = true;
            gameOverTexture = loadTexture("img/game-lost.png");
            this->runGameOverLoop();
        }

        if (finalBoss->health <= 0) {
            finalBoss->position.x = -10000;
            finalMissionRunning = false;
            gameOverScreen = true;
            this->runGameOverLoop();
        }

        if (!finalBossMovement) {
            finalBoss->moveDown(finalBossMovement);
        }
        else {
            finalBoss->moveUp(finalBossMovement);
        }

        if (isBulletFired) {
            player.bullet->move(player.position.x, player.position.y, isBulletFired);
        }
        else {
            player.bullet->setPosition(player.position.x + 50, player.position.y + 50);
        }

        handlePlayerBulletCollision();
        handleEnemyBulletCollision();
        finalBoss->bullet->move(finalBoss->position.x, finalBoss->position.y, enemyFire);
    }

    //rendering the final mission elements
    void renderFinalMission() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue color for the laboratory game
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, finalMissionTexture, nullptr, nullptr);
        player.render(renderer, isBulletFired);
        finalBoss->render(renderer, enemyFire);
        renderText("Player Health: " + to_string(player.health), 10, 0, textColor);
        renderText("Enemy Health: " + to_string(finalBoss->health), 970, 0, textColor);
        SDL_RenderPresent(renderer);
    }
/*------------------------------------------Game Over-------------------------------------------------------------*/
    //game over handling
    void runGameOverLoop() {
        while (gameOverScreen) {
            handleGameOverInput();
            renderGameOver();
            SDL_Delay(16);
        }
    }

    void handleGameOverInput() {
        SDL_Event gameOverEvent;
        while (SDL_PollEvent(&gameOverEvent)) {
            if (gameOverEvent.type == SDL_QUIT) {
                cleanup();
                exit(0);
            }
            else if (gameOverEvent.type == SDL_KEYDOWN) {
                if (gameOverEvent.key.keysym.sym == SDLK_ESCAPE) {
                    cleanup();
                    exit(0);
                }
            }
        }
    }

    //rendering the final mission elements
    void renderGameOver() {
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, gameOverTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

/*------------------------------------------Save Game-------------------------------------------------------------*/
    void saveGame(const string& filename) {
        ofstream outputFile(filename);

        if (outputFile.is_open()) {
            outputFile << npcInteractStatus << endl;
            outputFile << waveOneCompleted << endl;
            outputFile << waveTwoCompleted << endl;
            outputFile << player.weaponPickup << endl;
            if (player.weaponPickup) {
                outputFile << "img/player-2.png" << endl;
            }
            else {
                outputFile << "img/player-1.png" << endl;
            }
            
            outputFile << player.health << endl;
            outputFile << player.bullet->damage << endl;

            outputFile.close();
        }
        else {
            cerr << "Unable to open the file for saving." << endl;
        }
    }

    void loadGame(const std::string& filename) {
        std::ifstream inputFile(filename);
        string s1;

        if (inputFile.is_open()) {
            inputFile >> npcInteractStatus;
            inputFile >> waveOneCompleted;
            inputFile >> waveTwoCompleted;
            inputFile >> player.weaponPickup;
            inputFile >> s1;
            inputFile >> player.health;
            inputFile >> player.bullet->damage;

            inputFile.close();
            if (player.weaponPickup) {
                player.setHeightAndWidth(140, 180);
            }
            const char* playerTexture = s1.c_str();
            player.texture = loadTexture(playerTexture);
        }
        else {
            std::cerr << "Unable to open the file for loading." << std::endl;
        }
    }
};

#endif
