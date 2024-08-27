#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

const int WIDTH = 1400;
const int HEIGHT = 700;

// Define a simple Vector2D struct
struct Vector2D {
    float x, y;

    Vector2D() : x(0), y(0) {}
    Vector2D(float x, float y) : x(x), y(y) {}
};

// Define a GameObject struct
struct GameObject {
    Vector2D position;
    Vector2D size;
    Vector2D velocity;

    GameObject(float x, float y, int sx, int sy, float vx, float vy) 
        : position(x, y), size(sx, sy), velocity(vx, vy) {}

    // Update the position based on velocity
    virtual void update() {
        position.x += velocity.x;
        position.y += velocity.y;
    }

    virtual void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = { static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(size.x), static_cast<int>(size.y) };
        SDL_RenderFillRect(renderer, &rect);
    }
};

struct Ball : public GameObject {
    int outTimer = 0;

    Ball(float x, float y, int sx, int sy, float vx, float vy) 
        : GameObject(x, y, sx, sy, vx, vy) {}

    void update() override {
        GameObject::update();

        if (position.x < 0 || position.x + size.x > WIDTH) {
            outTimer++;
        }
        if (position.y < 0) {
            velocity.y = abs(velocity.y);
        } else if (position.y + size.y > HEIGHT) {
            velocity.y = -abs(velocity.y);
        }
    }
};

struct Paddle : public GameObject {
    int player;

    Paddle(float x, float y, int sx, int sy, float vx, float vy, int player) 
        : GameObject(x, y, sx, sy, vx, vy), player(player) {}
    
    void update() override {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        const float speed = 2;
        velocity.y = 0;
        if (player == 0) {
            if (state[SDL_SCANCODE_W])
                velocity.y -= speed;
            if (state[SDL_SCANCODE_S])
                velocity.y += speed;
        } else if (player == 1) {
            if (state[SDL_SCANCODE_UP])
                velocity.y -= speed;
            if (state[SDL_SCANCODE_DOWN])
                velocity.y += speed;
        }

        GameObject::update();
        if (position.y < 0) position.y = 0;
        if (position.y > HEIGHT - size.y) position.y = HEIGHT - size.y;
    }

    void render(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect = { static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(size.x), static_cast<int>(size.y) };
        SDL_RenderFillRect(renderer, &rect);
    }
};

void checkCollision(GameObject& ball, GameObject& paddle) {
    // Get the bounding box edges for both ball and paddle
    float ballLeft = ball.position.x;
    float ballRight = ball.position.x + ball.size.x;
    float ballTop = ball.position.y;
    float ballBottom = ball.position.y + ball.size.y;

    float paddleLeft = paddle.position.x;
    float paddleRight = paddle.position.x + paddle.size.x;
    float paddleTop = paddle.position.y;
    float paddleBottom = paddle.position.y + paddle.size.y;

    if (ballLeft > paddleRight || paddleLeft > ballRight)
        return;
    if (ballTop > paddleBottom || paddleTop > ballBottom)
        return;

    //std::cout << "Ball hit paddle." << std::endl;

    // Calculate overlap amounts on each side
    float overlapLeft = ballRight - paddleLeft;
    float overlapRight = paddleRight - ballLeft;
    float overlapTop = ballBottom - paddleTop;
    float overlapBottom = paddleBottom - ballTop;

    // Find the smallest overlap
    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

    // Determine which side of the paddle the ball is hitting based on the smallest overlap
    if (minOverlap == overlapLeft)  {
        ball.velocity.x = -abs(ball.velocity.x);
    } else if (minOverlap == overlapRight) {
        ball.velocity.x = abs(ball.velocity.x);
    } else if (minOverlap == overlapTop) {
        ball.velocity.y *= -1;
        paddle.position.y += 4;
    }
    else if (minOverlap == overlapBottom) {
        ball.velocity.y *= -1;
        paddle.position.y -= 4;
    }
    ball.velocity.x *= 1.1;
    float paddleMiddle = paddleTop + paddle.size.y / 2;
    paddleMiddle -= ball.size.y / 2;
    float paddleHalf = paddleMiddle - paddleTop + ball.size.y;
    ball.velocity.y = 4 * ((ballTop - paddleMiddle) / paddleHalf);
}

// Global container to store all game objects
std::vector<std::unique_ptr<GameObject>> gameObjects;

void update() {
    // Update all game objects
    for (auto& obj : gameObjects) {
        obj->update();
        if (Ball* ball = dynamic_cast<Ball*>(obj.get())) {
            for (auto& otherObj : gameObjects) {
                if (Paddle* paddle = dynamic_cast<Paddle*>(otherObj.get())) {
                    checkCollision(*ball, *paddle);
                }
            }
            if (ball->outTimer >= 300) {
                ball->outTimer = 0;
                ball->position.x = WIDTH / 2.0 - ball->size.x / 2;
                ball->position.y = HEIGHT / 2.0 - ball->size.y / 2;
                ball->velocity.y = rand() % 9 - 4;
                ball->velocity.x = 2.0;
                if (rand() % 2 == 0)
                    ball->velocity.x *= -1;
            }
        }
    }
}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render all game objects
    for (const auto& obj : gameObjects) {
        obj->render(renderer);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Example: Creating a few game objects
    gameObjects.emplace_back(std::make_unique<Paddle>(50, 100, 50, HEIGHT / 3, 0.0f, 0.0f, 0));
    gameObjects.emplace_back(std::make_unique<Paddle>(WIDTH - 100, 100, 50, HEIGHT / 3, 0.0f, 0.0f, 1));
    gameObjects.emplace_back(std::make_unique<Ball>(200, 100, 50, 50, 2.0f, 0.5f));

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        update();
        render(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
