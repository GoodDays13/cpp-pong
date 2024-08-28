#include "Game.h"

const int WIDTH = 1400;
const int HEIGHT = 700;

int main (int argc, char *argv[]) {
    Game game;

    if (!game.init("Pong", WIDTH, HEIGHT)) {
        return -1;
    }

    game.run();
    game.clean();

    return 0;
}
