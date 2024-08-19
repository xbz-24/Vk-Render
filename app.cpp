#include "app.hpp"
#include <iostream>
#include <sstream>
#include <memory>
#include <functional>

App::App(int width, int height, bool is_debug)
{
    build_glfw_window(width, height, is_debug);
    graphics_engine_ = std::make_unique<Engine>(width, height, window_.get(), is_debug);
    scene_ = std::make_unique<Scene>();
}
void App::build_glfw_window(int width, int height, bool is_debug_mode)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>>(
            glfwCreateWindow(width, height, "ID Tech 12", nullptr, nullptr),
            [](GLFWwindow* w){
                if(w){
                    glfwDestroyWindow(w);
                }
            });

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
    while(!glfwWindowShouldClose(window_.get()))
    {
        glfwPollEvents();
        graphics_engine_->render(scene_.get());
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

        glfwSetWindowTitle(window_.get(), title.str().c_str());

        last_time_ = current_time_;
        num_frames_ = -1;
        frame_time_ = float(1000.0 / framerate);
    }
    num_frames_++;
}
App::~App() = default;