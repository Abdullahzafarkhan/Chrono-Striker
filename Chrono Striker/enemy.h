#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>
using namespace std;

class EnemyBullet {
public:
    int damage;
    SDL_Rect position;
    SDL_Texture* texture;

    EnemyBullet(int d, const SDL_Rect& p, SDL_Texture* t) {
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
        delta_x -= 1;
        delta_x *= 15;

        if (position.y < 0) {
            position.x = tempX + 15, position.y = tempY + 55;
            fire = false;
        }
        else if (position.y > 768) {
            position.x = tempX + 15, position.y = tempY + 55;
            fire = false;
        }
        else if (position.x > 1360) {
            position.x = tempX + 15, position.y = tempY + 55;
            fire = false;

        }
        else if (position.x < 0) {
            position.x = tempX + 15, position.y = tempY + 55;
            fire = false;
        }

        position.x += static_cast<int>(delta_x);
        position.y += static_cast<int>(delta_y);
    }

    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, nullptr, &position);
    }
};

class Enemy {
public:
    int health;
    EnemyBullet* bullet;
    SDL_Rect position;
    SDL_Texture* texture;

    Enemy(int initialHealth, const SDL_Rect& initialPosition, SDL_Texture* texture, int d, SDL_Texture* t)
        : health(initialHealth), position(initialPosition), texture(texture) {
        bullet = new EnemyBullet(d, {position.x + 15, position.y + 55, 50, 10}, t);
    }
    
    void moveUp(bool& movement) {
        if (position.y - 10 >= 480) {
            position.y -= 1;
        }
        else {
            movement = false;
        }
    }

    void moveDown(bool& movement) {
        if (position.y + position.h + 10 <= 768) {
            position.y += 1;
        }
        else {
            movement = true;
        }
    }

    void render(SDL_Renderer* renderer, bool& fire) const {
        if (fire) {
            SDL_RenderCopy(renderer, bullet->texture, nullptr, &bullet->position);
        }
        SDL_RenderCopy(renderer, bullet->texture, nullptr, &bullet->position);
        SDL_RenderCopy(renderer, texture, nullptr, &position);
    }
};

#endif