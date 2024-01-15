/**
 * @file app.cpp
 * @brief Implementation of the App class.
 * @date Created by Renato on 27-12-23.
 */
#include "app.hpp"
#include <iostream>
/**
 * @brief Constructs an App object.
 *
 * Initializes a new GLFW window and creates instances of the Engine and Scene classes.
 * This is the primary entry point for setting up the Vulkan-based graphics application.
 *
 * @param width The width of the GLFW window.
 * @param height The height of the GLFW window.
 * @param debug Indicates whether debugging features should be enabled.
 */
App::App(int width, int height, bool debug)
{
    build_glfw_window(width, height, debug);
    graphicsEngine = new Engine(width, height, window, debug);
    scene = new Scene();
}
/**
 * @brief Initializes and creates a GLFW window.
 *
 * Sets up the GLFW window with the specified dimensions and debug mode.
 * Outputs debug messages if the window is created successfully or if the creation fails.
 *
 * @param width The width of the window to create.
 * @param height The height of the window to create,
 * @param debugMode Indicates whether to enable debug logging.
 */
void App::build_glfw_window(int width, int height, bool debugMode)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    if(window = glfwCreateWindow(width, height, "ID Tech 12", nullptr, nullptr))
    {
        if(debugMode)
        {
            std::cout << "Successfully made a glfw window called \"ID Tech 12\", width: ";
            std::cout << width << ", height: " << height << '\n';
        }
    }
    else
    {
        if(debugMode)
        {
            std::cout << "GLFW window creation failed\n";
        }
    }
}
/**
 * @brief Runs the main application loop.
 *
 * Handles the primary loop of the application, including polling for window events,
 * rendering the scene, and calculating the frame rate. The loop continues until the
 * GLFW window should close.
 */
void App::run(){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        graphicsEngine->render(scene);
        calculateFrameRate();
    }
}
/**
 * @brief Calculates and displays the frame rate.
 *
 * Measures the time elapsed since the last frame and updates the window title with the
 * current frame rate every second. This helps in monitoring the performance of the application.
 */
void App::calculateFrameRate() {
    currentTime = glfwGetTime();
    double delta = currentTime - lastTime;

    if(delta >= 1){
        int framerate  {std::max(1, int(numFrames / delta))};
        std::stringstream title;
        title << "Running at " << framerate << " fps.";
        glfwSetWindowTitle(window, title.str().c_str());
        lastTime = currentTime;
        numFrames = -1;
        frameTime = float(1000.0 / framerate);
    }
    ++numFrames;
}
/**
 * @brief Destructor of the App class.
 *
 * Cleans up by deleting the instances of Engine and Scene, which were created in the constructor.
 * This ensures proper resource management and prevents memory leaks.
 */
App::~App(){
    delete graphicsEngine;
    delete scene;
}