#ifndef INPUT_H
#define INPUT_H

#include "Player.h"

class Input {
    private:
        Input();
    public:
        static void processInput(Player *player);
};
#endif