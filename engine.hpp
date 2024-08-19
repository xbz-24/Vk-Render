#ifndef INC_3DLOADERVK_ENGINE_HPP
#define INC_3DLOADERVK_ENGINE_HPP
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "frame.hpp"
#include "scene.hpp"
#include "triangle_mesh.hpp"
#include "quad_mesh.hpp"
#include <filesystem>

class Engine
{
public:
    Engine(int width, int height, GLFWwindow* window, bool debug);
    ~Engine();
    void render(Scene* scene);
    void make_instance();
    void make_device();
    void make_swapchain();
    void recreate_swapchain();
    void make_pipeline();
    void finalize_setup();
    void make_frame_buffers();
    void make_frame_sync_objects();
    void make_assets();
    void prepare_scene(vk::CommandBuffer commandBuffer);
    void record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex, Scene* scene);
    void cleanup_swap_chain();
private:
    bool debug_mode_;
    int width_;
    int height_;
    GLFWwindow* window_;
    vk::Instance instance_{ nullptr };
    vk::DebugUtilsMessengerEXT debug_messenger_{ nullptr };
    vk::DispatchLoaderDynamic dldi_;
    vk::SurfaceKHR surface_;
    vk::PhysicalDevice physical_device_ {nullptr };
    vk::Device device_ { nullptr };
    vk::Queue graphics_queue_ { nullptr };
    vk::Queue present_queue_ { nullptr};
    vk::SwapchainKHR swapchain_ { nullptr };
    std::vector<vkutil::SwapChainFrame> swap_chain_frames_;
    vk::Format swapchain_format_;
    vk::Extent2D swapchain_extent_;
    vk::PipelineLayout pipeline_layout_;
    vk::RenderPass render_pass_;
    vk::Pipeline pipeline_;
    vk::CommandPool command_pool_;
    vk::CommandBuffer main_command_buffer_;
    int max_frames_in_flight_;
    int frame_number_;
    TriangleMesh* triangle_mesh_;
    QuadMesh* quad_mesh_;
};
#endif //INC_3DLOADERVK_ENGINE_HPP