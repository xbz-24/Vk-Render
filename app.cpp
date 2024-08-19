#include "app.hpp"
#include <iostream>
#include <sstream>

App::App(int width, int height, bool is_debug)
{
    build_glfw_window(width, height, is_debug);
    graphics_engine_ = new Engine(width, height, window_, is_debug);
    scene_ = new Scene();
}
void App::build_glfw_window(int width, int height, bool is_debug_mode)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window_ = glfwCreateWindow(width, height, "ID Tech 12", nullptr, nullptr);
    if(window_)
    {
        if(is_debug_mode)
        {
            std::cout << "Successfully made a glfw window_ called \"ID Tech 12\", width_: ";
            std::cout << width << ", height_: " << height << '\n';
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
void App::run()
{
    while(!glfwWindowShouldClose(window_))
    {
        glfwPollEvents();
        graphics_engine_->render(scene_);
        calculate_frame_rate();
    }
}
void App::calculate_frame_rate()
{
    current_time_ = glfwGetTime();
    double delta = current_time_ - last_time_;

    if(delta >= 1)
    {
        int framerate = std::max(1, int(num_frames_ / delta));
        std::stringstream title;
        title << "Running at " << framerate << " fps.";
        glfwSetWindowTitle(window_, title.str().c_str());
        last_time_ = current_time_;
        num_frames_ = -1;
        frame_time_ = float(1000.0 / framerate);
    }
    ++num_frames_;
}
App::~App()
{
    delete graphics_engine_;
    delete scene_;
}