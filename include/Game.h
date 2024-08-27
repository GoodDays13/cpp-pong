#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include "GameObject.h"

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void clean();

    static SDL_Window* window;
    static SDL_Renderer* renderer;
private:
    bool isRunning;
    SDL_Event event;

    std::vector<std::unique_ptr<GameObject>> gameObjects;

    void handleEvents();
    void update();
    void render();
};

#endif // !GAME_H
