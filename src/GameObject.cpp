#include "GameObject.h"

GameObject::GameObject(float x, float y, float width, float height, float vx, float vy)
    : position(x, y), size(width, height), velocity(vx, vy) {}

void GameObject::update() {
    position.x += velocity.x;
    position.y += velocity.y;
}

void GameObject::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Default color: Red
    SDL_Rect rect = { 
        static_cast<int>(position.x), 
        static_cast<int>(position.y), 
        static_cast<int>(size.x), 
        static_cast<int>(size.y) 
    };
    SDL_RenderFillRect(renderer, &rect);
}

void GameObject::collisionCheck(std::vector<GameObject*> objects) {
}
