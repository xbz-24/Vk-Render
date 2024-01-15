/**
 * @file app.h
 * @brief Defines the App class, which serves as the main entry point and controller for the Vulkan-based graphics application.
 * @Date Created by Renato on 27-12-23.
 *
 * This class manages the creation and lifecycle of a GLFW window_, initializes the graphics engine (Engine), and handles
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
     * Initializes the GLFW window_ and creates instances of the Engine and Scene classes.
     *
     * @param width The width_ of the GLFW window_.
     * @param height The height_ of the GLFW window_.
     * @param is_debug Flag indicating whether to run in is_debug mode, affecting logging verbosity.
     */
    App(int width, int height, bool is_debug);
    /**
     * @brief Destructor for the App class-
     *
     * Cleans up by deleting the graphics engine and the scene_.
     */
    ~App();
    /**
     * @brief Runs the main application loop.
     *
     * Continuosly polls for GLFW events, renders the scene_, and calculates the frame rate until the window_ should close.
     */
    void run();
private:
    Engine* graphics_engine_;
    GLFWwindow* window_;
    Scene* scene_;

    double last_time_;
    double current_time_;
    int num_frames_;
    float frame_time_;
    /**
     * @brief Initializes and builds a GLFW window_.
     *
     * Sets up a GLFW window_ with the specified width_, height_, and debug mode. Handles window_ creation success or failure.
     *
     * @param width The width_ of the GLFW window_.
     * @param height The height_ of the GLFW window_.
     * @param is_debug_mode Flag indicating whether to enable debug logging for window_ creation.
     */
    void buildGlfwWindow(int width, int height, bool is_debug_mode);
    /**
     * @brief Calculates and updates the frame rate of the application.
     *
     * Measures the time elapsed and updates the window_ title with the current frame rate every second.
     */
    void calculateFrameRate();
};
#endif //INC_3DLOADERVK_APP_HPP