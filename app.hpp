#ifndef INC_3DLOADERVK_APP_HPP
#define INC_3DLOADERVK_APP_HPP
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "engine.hpp"
#include "scene.hpp"

class App
{
public:
    App(int width, int height, bool is_debug);
    ~App();
    void run();
    void build_glfw_window(int width, int height, bool is_debug_mode);
    void calculate_frame_rate();
private:
    Engine* graphics_engine_;
    GLFWwindow* window_;
    Scene* scene_;
    double last_time_;
    double current_time_;
    int num_frames_;
    float frame_time_;
};
#endif //INC_3DLOADERVK_APP_HPP