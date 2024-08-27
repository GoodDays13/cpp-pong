#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <vector>
#include "Vector2D.h"

class GameObject {
public:
    Vector2D position;
    Vector2D size;
    Vector2D velocity;

    GameObject(float x, float y, float width, float height, float vx, float vy);
    virtual ~GameObject() = default;

    virtual void update();
    virtual void render(SDL_Renderer* renderer);
    virtual void collisionCheck(std::vector<GameObject*> objects);
};

#endif // !GAMEOBJECT_H

