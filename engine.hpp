//
// Created by daily on 27-12-23.
//

#ifndef INC_3DLOADERVK_ENGINE_HPP
#define INC_3DLOADERVK_ENGINE_HPP
#include "config.hpp"
#include "frame.hpp"

class Engine {
public:
    Engine(int width, int height, GLFWwindow* window, bool debug);
    ~Engine();
    void render();

private:
    // whether to print debug messages in functions
    bool debugMode;
    //glfw window parameters
    int width;
    int height;
    GLFWwindow* window;

    //instance related variables
    vk::Instance instance{ nullptr };
    vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
    vk::DispatchLoaderDynamic dldi;
    vk::SurfaceKHR surface;

    //device-related variables
    vk::PhysicalDevice physicalDevice { nullptr };
    vk::Device device { nullptr };
    vk::Queue graphicsQueue { nullptr };
    vk::Queue presentQueue { nullptr};
    vk::SwapchainKHR swapchain { nullptr };
    std::vector<vkUtil::SwapChainFrame> swapChainFrames;
    vk::Format swapchainFormat;
    vk::Extent2D swapchainExtent;

    //pipeline-related variables
    vk::PipelineLayout layout;
    vk::RenderPass renderpass;
    vk::Pipeline pipeline;

    //command-related variables
    vk::CommandPool commandPool;
    vk::CommandBuffer mainCommandBuffer;

    //synchronization-related variables
    vk::Fence inFlightFence;
    vk::Semaphore imageAvailable, renderFinished;



    //instance setup
    void make_instance();

    //device setup
    void make_device();

    //pipeline setup
    void make_pipeline();

    void finalize_setup();

    void record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex);
};


#endif //INC_3DLOADERVK_ENGINE_HPP
