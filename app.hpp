#ifndef INC_3DLOADERVK_APP_HPP
#define INC_3DLOADERVK_APP_HPP
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "engine.hpp"
#include "scene.hpp"
#include <memory>
#include <functional>

class App
{
public:
    App(int width, int height, bool is_debug);
    ~App();
    void run();
    void build_glfw_window(int width, int height, bool is_debug_mode);
    void calculate_frame_rate();
private:
    std::unique_ptr<Engine> graphics_engine_;
    std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> window_;
    std::unique_ptr<Scene> scene_;
    double last_time_;
    double current_time_;
    int num_frames_;
    float frame_time_;
};
#endif //INC_3DLOADERVK_APP_HPP