//
// Created by daily on 27-12-23.
//

#ifndef INC_3DLOADERVK_APP_H
#define INC_3DLOADERVK_APP_H

#include "config.hpp"
#include "engine.hpp"

class App{
private:
    Engine* graphicsEngine;
    GLFWwindow* window;
    double lastTime, currentTime;
    int numFrames;
    float frameTime;

    void build_glfw_window(int width, int height, bool debugMode);

    void calculateFrameRate();
public:
    App(int width, int height, bool debug);
    ~App();
    void run();
};


#endif //INC_3DLOADERVK_APP_H
