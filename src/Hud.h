#ifndef HUD_H
#define HUD_H

#include "Texture.h"
#include "Shader.h"

class Renderer;

class Hud {
private:
    Texture crosshairTexture;
    Shader shader;
    double crosshairX;
    double crosshairY;
public:
    Hud();
    void setCrosshairPosition(double x, double y);
    void render(Renderer *renderer);
};

#endif