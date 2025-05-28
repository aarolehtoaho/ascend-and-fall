#ifndef INPUT_H
#define INPUT_H

#include "Camera.h"

class Input {
    private:
        Input();
    public:
        static void processInput(Camera *camera);
};
#endif