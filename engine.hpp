/**
 * @file engine.hpp
 * @brief Defines the Engine class, which encapsulates the functionality of a Vulkan-based rendering engine.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_ENGINE_HPP
#define INC_3DLOADERVK_ENGINE_HPP

#include "config.hpp"
#include "frame.hpp"
#include "scene.h"

/**
 * @class Engine
 * @brief The Engine class initializes and manages the core components of a Vulkan-based graphics application.
 *
 * This class is responsible for setting up and managing Vulkan resources such as the instance, device,
 * swap chain, graphics pipeline, and command buffers. It integrates with GLFW for window management and
 * provides functionality for rendering scenes.
 */

class Engine {
public:
    /**
     * @brief Constructs an Engine object.
     * @param width The width of the rendering window.
     * @param height The height of the rendering window.
     * @param window Pointer to the GLFWwindow to be used for rendering.
     * @param debug Indicates whether to enable debug mode.
     */
    Engine(int width, int height, GLFWwindow* window, bool debug);
    /**
     * @brief Destructor that cleans up Vulkan and GLFW resources.
     */
    ~Engine();
    /**
     * @brief Renders a given scene.
     * @param scene Pointer to the scene object to be rendered.
     */
    void render(Scene* scene);

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
    vk::PipelineLayout pipelineLayout;
    vk::RenderPass renderpass;
    vk::Pipeline pipeline;

    //command-related variables
    vk::CommandPool commandPool;
    vk::CommandBuffer mainCommandBuffer;

    //synchronization-related variables
    int maxFramesInFlight;
    int frameNumber;


    //instance setup
    void make_instance();

    //device setup
    void make_device();

    //pipeline setup
    void make_pipeline();

    void finalize_setup();
    /**
     * @brief Records draw commands for the given scene into a Vulkan command buffer.
     * @param commandBuffer The command buffer to record into.
     * @param imageIndex The index of the image in the swap chain to draw to.
     * @param scene Pointer to the scene to be drawn.
     */
    void record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex, Scene* scene);
};


#endif //INC_3DLOADERVK_ENGINE_HPP
