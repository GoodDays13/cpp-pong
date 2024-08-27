#ifndef PADDLE_H
#define PADDLE_H

#include "GameObject.h"

class Paddle : public GameObject {
    int player_id;
public:

    Paddle(float x, float y, float width, float height, float vx, float vy, int player_id);

    void update() override;
    void render(SDL_Renderer* renderer) override;
};

#endif // !PADDLE_H

