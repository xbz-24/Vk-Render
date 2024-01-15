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
 * Initializes a new GLFW window_ and creates instances of the Engine and Scene classes.
 * This is the primary entry point for setting up the Vulkan-based graphics application.
 *
 * @param width The width of the GLFW window_.
 * @param height The height of the GLFW window_.
 * @param is_debug Indicates whether debugging features should be enabled.
 */
App::App(int width, int height, bool is_debug)
{
    buildGlfwWindow(width, height, is_debug);
    graphics_engine_ = new Engine(width, height, window_, is_debug);
    scene_ = new Scene();
}
/**
 * @brief Initializes and creates a GLFW window_.
 *
 * Sets up the GLFW window_ with the specified dimensions and debug mode.
 * Outputs debug messages if the window_ is created successfully or if the creation fails.
 *
 * @param width The width of the window_ to create.
 * @param height The height of the window_ to create,
 * @param is_debug_mode Indicates whether to enable debug logging.
 */
void App::buildGlfwWindow(int width, int height, bool is_debug_mode)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    if(window_ = glfwCreateWindow(width, height, "ID Tech 12", nullptr, nullptr))
    {
        if(is_debug_mode)
        {
            std::cout << "Successfully made a glfw window_ called \"ID Tech 12\", width: ";
            std::cout << width << ", height: " << height << '\n';
        }
    }
    else
    {
        if(is_debug_mode)
        {
            std::cout << "GLFW window_ creation failed\n";
        }
    }
}
/**
 * @brief Runs the main application loop.
 *
 * Handles the primary loop of the application, including polling for window_ events,
 * rendering the scene_, and calculating the frame rate. The loop continues until the
 * GLFW window_ should close.
 */
void App::run(){
    while(!glfwWindowShouldClose(window_)){
        glfwPollEvents();
        graphics_engine_->render(scene_);
        calculateFrameRate();
    }
}
/**
 * @brief Calculates and displays the frame rate.
 *
 * Measures the time elapsed since the last frame and updates the window_ title with the
 * current frame rate every second. This helps in monitoring the performance of the application.
 */
void App::calculateFrameRate() {
    current_time_ = glfwGetTime();
    double delta = current_time_ - last_time_;

    if(delta >= 1){
        int framerate  {std::max(1, int(num_frames_ / delta))};
        std::stringstream title;
        title << "Running at " << framerate << " fps.";
        glfwSetWindowTitle(window_, title.str().c_str());
        last_time_ = current_time_;
        num_frames_ = -1;
        frame_time_ = float(1000.0 / framerate);
    }
    ++num_frames_;
}
/**
 * @brief Destructor of the App class.
 *
 * Cleans up by deleting the instances of Engine and Scene, which were created in the constructor.
 * This ensures proper resource management and prevents memory leaks.
 */
App::~App(){
    delete graphics_engine_;
    delete scene_;
}