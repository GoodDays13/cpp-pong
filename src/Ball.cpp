#include "Ball.h"

#include <algorithm>

#include "Game.h"

Ball::Ball(float x, float y, float width, float height, float vx, float vy)
    : GameObject(x, y, width, height, vx, vy), outTimer(0) {}

void Ball::update() {
    GameObject::update();

    int width, height;
    SDL_GetWindowSize(Game::window, &width, &height);

    // Check if the ball is out of horizontal bounds
    if (position.x < 0 || position.x + size.x > width) {
        outTimer++;
    }

    // Bounce off top and bottom boundaries
    if (position.y < 0) {
        velocity.y = std::abs(velocity.y);
    } else if (position.y + size.y > height) {
        velocity.y = -std::abs(velocity.y);
    }

    // Reset ball position and velocity after being out for a certain time
    if (outTimer >= 300) {
        outTimer = 0;
        position.x = width / 2.0f - size.x / 2.0f;
        position.y = height / 2.0f - size.y / 2.0f;
        velocity.y = static_cast<float>(rand() % 9 - 4); // Random y velocity between -4 and 4
        velocity.x = 2.0f;
        if (rand() % 2 == 0)
            velocity.x *= -1; // Randomize direction
    }
}

void Ball::checkCollision(Paddle& paddle) {
    // Calculate the edges of the ball
    float ballLeft = position.x;
    float ballRight = position.x + size.x;
    float ballTop = position.y;
    float ballBottom = position.y + size.y;

    // Calculate the edges of the paddle
    float paddleLeft = paddle.position.x;
    float paddleRight = paddle.position.x + paddle.size.x;
    float paddleTop = paddle.position.y;
    float paddleBottom = paddle.position.y + paddle.size.y;

    // Check for collision
    if (ballLeft > paddleRight || paddleLeft > ballRight)
        return;
    if (ballTop > paddleBottom || paddleTop > ballBottom)
        return;

    // Calculate overlap on each side
    float overlapLeft = ballRight - paddleLeft;
    float overlapRight = paddleRight - ballLeft;
    float overlapTop = ballBottom - paddleTop;
    float overlapBottom = paddleBottom - ballTop;

    // Determine the smallest overlap
    float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

    // Adjust ball velocity based on collision side
    if (minOverlap == overlapLeft)  {
        velocity.x = -std::abs(velocity.x);
    }
    else if (minOverlap == overlapRight) {
        velocity.x = std::abs(velocity.x);
    }
    else if (minOverlap == overlapTop) {
        velocity.y *= -1;
        paddle.position.y += 4; // Slightly move paddle to prevent sticking
    }
    else if (minOverlap == overlapBottom) {
        velocity.y *= -1;
        paddle.position.y -= 4; // Slightly move paddle to prevent sticking
    }

    // Increase ball speed after collision
    velocity.x *= 1.1f;

    // Adjust ball's Y velocity based on where it hit the paddle
    float paddleMiddle = paddle.position.y + paddle.size.y / 2.0f;
    float distanceFromMiddle = position.y + size.y / 2.0f - paddleMiddle;
    float normalizedDistance = distanceFromMiddle / (paddle.size.y / 2.0f);
    velocity.y = 4.0f * normalizedDistance;
}

void Ball::collisionCheck(std::vector<GameObject*> objects) {
    for (auto& obj : objects) {
        if (Paddle* paddle = dynamic_cast<Paddle*>(obj)) {
            checkCollision(*paddle);
        }
    }
}
