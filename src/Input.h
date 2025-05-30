#ifndef INPUT_H
#define INPUT_H

#include "Player.h"

class Input {
    private:
        Input();
        static bool hasReleasedW;
    public:
        static void processInput(Player *player);
};
#endif