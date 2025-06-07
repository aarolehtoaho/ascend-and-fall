#include "Hud.h"
#include "Renderer.h"
#include "Window.h"

Hud::Hud() 
    : crosshairX(0.0f), crosshairY(0.0f), crosshairTexture("assets/textures/cursor.png"), \
      shader("assets/shaders/hudshader.vs", "assets/shaders/hudshader.fs") {}

void Hud::setCrosshairPosition(double x, double y) {
    crosshairX = x;
    crosshairY = y;
}

void Hud::render(Renderer *renderer) {
    shader.use();
    crosshairTexture.bind();
    shader.setInt("hudTexture", crosshairTexture.getUnit());

    float size = 32.0f;
    float offsetX = size / 4.0f;
    float offsetY = size / 2.0f;

    renderer->drawStaticSquare(shader, glm::vec3(crosshairX + offsetX, Window::getHeight() - crosshairY - offsetY, 0.0f), glm::vec3(size, size, 1.0f));
}