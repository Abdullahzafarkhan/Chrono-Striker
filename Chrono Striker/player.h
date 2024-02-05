#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class Bullet {
public:
    int damage;
    SDL_Rect position;
    SDL_Texture* texture;

    Bullet() {
        damage = 0;
        position = {0,0,0,0};
        texture = nullptr;
    }

    void setBullet(int d, const SDL_Rect& p, SDL_Texture* t) {
        damage = d;
        position = p;
        texture = t;
    }

    void setPosition(int x, int y) {
        position.x = x;
        position.y = y;
    }

    void move(int tempX, int tempY, bool& fire) {
        float delta_x = 0, delta_y = 0;
        delta_x += 1;
        delta_x *= 25;

        if (position.y < 0) {
            position.x = tempX + 50, position.y = tempY + 50;
            fire = false;
        }
        else if (position.y > 768) {
            position.x = tempX + 50, position.y = tempY + 50;
            fire = false;
        }
        else if (position.x > 1360) {
            position.x = tempX, position.y = tempY + 50;
            fire = false;

        }
        else if (position.x < 0) {
            position.x = tempX + 50, position.y = tempY + 50;
            fire = false;
        }

        position.x += static_cast<int>(delta_x);
        position.y += static_cast<int>(delta_y);
    }

    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, nullptr, &position);
    }
};

class Player {
public:
    int health;
    Bullet* bullet;
    SDL_Rect position;
    SDL_Texture* texture;
    bool weaponPickup = false;

    Player(int initialHealth, const SDL_Rect& initialPosition, SDL_Texture* texture)
        : health(initialHealth), position(initialPosition), texture(texture) {
        bullet = new Bullet();
    }

    void setBullet(int damage, const SDL_Rect& p, SDL_Texture* t) {
        bullet->setBullet(damage, p, t);
    }

    void setHeightAndWidth(int x, int y) {
        position.w = x;
        position.h = y;
    }

    //limited movement for final battle
    void finalBattleMoveUp() {
        if (position.y - 10 >= 500) {
            position.y -= 10;
        }
    }

    void finalBattleMoveDown() {
        if (position.y + position.h + 10 <= 768) {
            position.y += 10;
        }
    }

    //regular movement functions
    void moveUp() {
        if (position.y - 10 >= 0) {
            position.y -= 10;
        }
    }

    void moveDown() {
        if (position.y + position.h + 10 <= 768) {
            position.y += 10;
        }
    }

    void moveLeft() {
        if (position.x - 10 >= 0) {
            position.x -= 10;
        }
    }

    void moveRight() {
        if (position.x + position.w + 10 <= 1360) {
            position.x += 10;
        }
    }

    //player render
    void render(SDL_Renderer* renderer, bool& fire) const {
        if (fire) {
            SDL_RenderCopy(renderer, bullet->texture, nullptr, &bullet->position);
        }
        SDL_RenderCopy(renderer, texture, nullptr, &position);
    }
};

#endif