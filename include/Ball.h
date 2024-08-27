#pragma once

#include "GameObject.h"
#include "Paddle.h"

class Ball : public GameObject {
public:
    int outTimer; // Tracks how long the ball has been out of bounds

    Ball(float x, float y, float width, float height, float vx, float vy);

    void update() override;
    void collisionCheck(std::vector<GameObject*> objects) override;

private:
    void checkCollision(Paddle& paddle);
};
