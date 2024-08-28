#include "Game.h"

#include <SDL2/SDL.h>
#include <iostream>

#include "Paddle.h"
#include "Ball.h"

SDL_Window* Game::window = nullptr;
SDL_Renderer* Game::renderer = nullptr;

Game::Game()
    : isRunning(false) {}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Initialize game objects
    gameObjects.emplace_back(std::make_unique<Paddle>(50.0f, 100.0f, 50, height / 3, 0.0f, 0.0f, 0));
    gameObjects.emplace_back(std::make_unique<Paddle>(width - 100.0f, 100.0f, 50, height / 3, 0.0f, 0.0f, 1));
    gameObjects.emplace_back(std::make_unique<Ball>(200.0f, 100.0f, 50, 50, 2.0f, 0.5f));

    isRunning = true;
    return true;
}

void Game::run() {
    unsigned long previousTime = SDL_GetTicks64();
    unsigned short lag = 0;
    while (isRunning) {
        handleEvents();

        unsigned long currentTime = SDL_GetTicks64();
        unsigned short elapsedTime = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsedTime;
        while (lag >= 10) {
            update();
            lag -= 10;
        }

        render();
    }
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void Game::update() {
    // Update all game objects
    for (auto& obj : gameObjects) {
        obj->update();

        std::vector<GameObject*> otherObjs;
        for (auto& other : gameObjects) {
            if (other != obj)
                otherObjs.push_back(other.get());
        }
        obj->collisionCheck(otherObjs);
    }
}

void Game::render() {
    // Clear the screen with black color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50,255);
    for (int i = 0; i < height; i += 50) {
        SDL_Rect centerLine = {
            width / 2 + 4,
            i,
            8,
            40
        };
        SDL_RenderFillRect(renderer, &centerLine);
    }

    // Render all game objects
    for (const auto& obj : gameObjects) {
        obj->render(renderer);
    }

    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

