/**
 * @file engine.hpp
 * @brief Defines the Engine class, which encapsulates the functionality of a Vulkan-based rendering engine.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_ENGINE_HPP
#define INC_3DLOADERVK_ENGINE_HPP
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "frame.hpp"
#include "scene.hpp"
#include "triangle_mesh.hpp"
#include "quad_mesh.hpp"
/**
 * @class Engine
 * @brief The Engine class initializes and manages the core components of a Vulkan-based graphics application.
 *
 * This class is responsible for setting up and managing Vulkan resources such as the instance_, device_,
 * swap chain, graphics pipeline_, and command buffers. It integrates with GLFW for window_ management and
 * provides functionality for rendering scenes.
 */
class Engine
{
public:
    /**
     * @brief Constructs an Engine object.
     * @param width The width_ of the rendering window_.
     * @param height The height_ of the rendering window_.
     * @param window Pointer to the GLFWwindow to be used for rendering.
     * @param debug Indicates whether to enable debug mode.
     */
    Engine(int width, int height, GLFWwindow* window, bool debug);
    /**
     * @brief Destructor that cleans up Vulkan and GLFW resources.
     */
    ~Engine();
    /**
     * @brief Renders a given scene_.
     * @param scene Pointer to the scene_ object to be rendered.
     */
    void render(Scene* scene);

private:
    // whether to print debug messages in functions
    bool debug_mode_;
    //glfw window_ parameters
    int width_;
    int height_;
    GLFWwindow* window_;

    //instance_ related variables
    vk::Instance instance_{ nullptr };
    vk::DebugUtilsMessengerEXT debug_messenger_{ nullptr };
    vk::DispatchLoaderDynamic dldi_;
    vk::SurfaceKHR surface_;

    //device_-related variables
    vk::PhysicalDevice physical_device_ {nullptr };
    vk::Device device_ { nullptr };
    vk::Queue graphics_queue_ { nullptr };
    vk::Queue present_queue_ { nullptr};
    vk::SwapchainKHR swapchain_ { nullptr };
    std::vector<vkutil::SwapChainFrame> swap_chain_frames_;
    vk::Format swapchain_format_;
    vk::Extent2D swapchain_extent_;

    //pipeline_-related variables
    vk::PipelineLayout pipeline_layout_;
    vk::RenderPass render_pass_;
    vk::Pipeline pipeline_;

    //command-related variables
    vk::CommandPool command_pool_;
    vk::CommandBuffer main_command_buffer_;

    //synchronization objects
    int max_frames_in_flight_;
    int frame_number_;

    //asset pointers
    TriangleMesh* triangle_mesh_;
    QuadMesh* quad_mesh_;

    //instance_ setup
    void MakeInstance();

    //device_ setup
    void MakeDevice();
    void MakeSwapchain();
    void RecreateSwapchain();

    //pipeline_ setup
    void MakePipeline();

    //final setup steps
    void FinalizeSetup();
    void MakeFramebuffers();
    void MakeFrameSyncObjects();

    void MakeAssets();
    void PrepareScene(vk::CommandBuffer commandBuffer);
    /**
     * @brief Records draw commands for the given scene_ into a Vulkan command buffer.
     * @param commandBuffer The command buffer to record into.
     * @param imageIndex The index of the image in the swap chain to draw to.
     * @param scene Pointer to the scene_ to be drawn.
     */
    void RecordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex, Scene* scene);
    void CleanupSwapchain();
};


#endif //INC_3DLOADERVK_ENGINE_HPP
