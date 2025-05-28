#ifndef UTILS_H
#define UTILS_H

class Utils {
private:
    static float deltaTime;
    static float lastFrame;
    Utils() {}
public:
    static void updateDeltaTime();

    static float getDeltaTime();
};
#endif