#include "Paddle.h"

#include "Game.h"

Paddle::Paddle(float x, float y, float width, float height, float vx, float vy, int player_id)
    : GameObject(x, y, width, height, vx, vy), player_id(player_id) {}

void Paddle::update() {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    const float speed = 2.0f;
    velocity.y = 0.0f;

    if (player_id == 0) { // Player 1 controls
        if (state[SDL_SCANCODE_W])
            velocity.y -= speed;
        if (state[SDL_SCANCODE_S])
            velocity.y += speed;
    } else if (player_id == 1) { // Player 2 controls
        if (state[SDL_SCANCODE_UP])
            velocity.y -= speed;
        if (state[SDL_SCANCODE_DOWN])
            velocity.y += speed;
    }

    // Update position based on velocity
    GameObject::update();

    // Constrain paddle within window bounds
    int width, height;
    SDL_GetWindowSize(Game::window, &width, &height);
    if (position.y < 0) position.y = 0;
    if (position.y > height - size.y) position.y = height - size.y;
}

void Paddle::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Paddle color: White
    SDL_Rect rect = { 
        static_cast<int>(position.x), 
        static_cast<int>(position.y), 
        static_cast<int>(size.x), 
        static_cast<int>(size.y) 
    };
    SDL_RenderFillRect(renderer, &rect);
}
