/**
 * @file app.h
 * @brief Defines the App class, which serves as the main entry point and controller for the Vulkan-based graphics application.
 * @Date Created by Renato on 27-12-23.
 *
 * This class manages the creation and lifecycle of a GLFW window, initializes the graphics engine (Engine), and handles
 * the main application loop including rendering and frame rate calculations.
 */
#ifndef INC_3DLOADERVK_APP_HPP
#define INC_3DLOADERVK_APP_HPP
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "engine.hpp"
#include "scene.hpp"
/**
 * @class App
 * @brief The app class encapsulates the main application loop and initialization logic for a Vulkan-based graphics application.
 */
class App
{
public:
    /**
     * @brief Constructs an App object.
     *
     * Initializes the GLFW window and creates instances of the Engine and Scene classes.
     *
     * @param width The width of the GLFW window.
     * @param height The height of the GLFW window.
     * @param debug Flag indicating whether to run in debug mode, affecting logging verbosity.
     */
    App(int width, int height, bool debug);
    /**
     * @brief Destructor for the App class-
     *
     * Cleans up by deleting the graphics engine and the scene.
     */
    ~App();
    /**
     * @brief Runs the main application loop.
     *
     * Continuosly polls for GLFW events, renders the scene, and calculates the frame rate until the window should close.
     */
    void run();
private:
    Engine* graphicsEngine;
    GLFWwindow* window;
    Scene* scene;

    double lastTime;
    double currentTime;
    int numFrames;
    float frameTime;
    /**
     * @brief Initializes and builds a GLFW window.
     *
     * Sets up a GLFW window with the specified width, height, and debug mode. Handles window creation success or failure.
     *
     * @param width The width of the GLFW window.
     * @param height The height of the GLFW window.
     * @param debugMode Flag indicating whether to enable debug logging for window creation.
     */
    void build_glfw_window(int width, int height, bool debugMode);
    /**
     * @brief Calculates and updates the frame rate of the application.
     *
     * Measures the time elapsed and updates the window title with the current frame rate every second.
     */
    void calculateFrameRate();
};
#endif //INC_3DLOADERVK_APP_HPP