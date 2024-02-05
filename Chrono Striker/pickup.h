#ifndef PICKUP_H
#define PICKUP_H

#include <SDL.h>
#include <SDL_image.h>
using namespace std;

class Pickup {
public:
	SDL_Rect position;
	SDL_Texture* texture;

	Pickup(const SDL_Rect& initialPosition, SDL_Texture* texture) : position(initialPosition), texture(texture) {

	}

	void move(bool& pickupStop) {
		position.y += 10;
		if (position.y == 460) {
			pickupStop = true;
		}
	}

	void render(SDL_Renderer* renderer) const {
		SDL_RenderCopy(renderer, texture, nullptr, &position);
	}
};

#endif