#include "engine.hpp"

#include "instance.hpp"
#include "logging.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "framebuffer.hpp"
#include "commands.hpp"
#include "sync.hpp"
#include <glm/gtc/matrix_transform.hpp>

Engine::Engine(int width, int height, GLFWwindow *window, bool debugMode) : debug_mode_(debugMode), width_(width), height_(height) , window_(window){
    if (debugMode)
    {
        std::cout << "Making a graphics engine\n";
    }
    make_instance();
    make_device();
    make_pipeline();
    finalize_setup();
    make_assets();
}

void Engine::make_instance()
{
    instance_ = vkinit::make_instance(debug_mode_, "ID Tech 12");
    dldi_ = vk::DispatchLoaderDynamic(instance_, vkGetInstanceProcAddr);

    if (debug_mode_)
    {
        debug_messenger_ = vkinit::make_debug_messenger(instance_, dldi_);
    }
    VkSurfaceKHR c_style_surface;

    if (glfwCreateWindowSurface(instance_, window_, nullptr, &c_style_surface) != VK_SUCCESS)
    {
        if (debug_mode_)
        {
            std::cout << "Failed to abstract the glfw surface_ for Vulkan.\n";
        }
    }
    else if (debug_mode_)
    {
        std::cout << "Successfully abstracted the glfw surface_ for Vulkan.\n";
    }
    surface_ = c_style_surface;
}

void Engine::make_device()
{
    physical_device_ = vkinit::choose_physical_device(instance_, debug_mode_);
    device_ = vkinit::create_logical_device(physical_device_,
                                            surface_, debug_mode_);
    std::array<vk::Queue, 2> queues = vkinit::get_queues(physical_device_,
                                                         device_,
                                                         surface_,
                                                         debug_mode_);
    graphics_queue_ = queues[0];
    present_queue_ = queues[1];
    make_swapchain();
    frame_number_ = 0;
}

void Engine::make_swapchain() {
    vkinit::SwapChainBundle bundle = vkinit::create_swap_chain(device_,
                                                               physical_device_,
                                                               surface_,
                                                               width_,
                                                               height_,
                                                               debug_mode_);
    swapchain_ = bundle.swapchain;
    swap_chain_frames_ = bundle.frames;
    swapchain_format_ = bundle.format;
    swapchain_extent_ = bundle.extent;
    max_frames_in_flight_ = static_cast<int>(swap_chain_frames_.size());
}

void Engine::recreate_swapchain() {
    width_ = 0;
    height_ = 0;
    while (width_ == 0 || height_ == 0) {
        glfwGetFramebufferSize(window_, &width_, &height_);
        glfwWaitEvents();
    }
    device_.waitIdle();
    cleanup_swap_chain();
    make_swapchain();
    make_frame_buffers();
    make_frame_sync_objects();
    vkinit::CommandBufferInputChunk commandBufferInput = {device_, command_pool_, swap_chain_frames_};
    vkinit::make_frame_command_buffer(commandBufferInput, debug_mode_);
}

void Engine::make_pipeline()
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Path: " << currentPath << std::endl;
    vkinit::GraphicsPipelineInBundle specification = {};
    specification.device = device_;
    std::string prefix = "../../"; // remove prefix addition unless using visual studio (windows only)
    prefix = "";
    specification.vertexFilepath = prefix + "../shaders/vertex.spv";
    specification.fragmentFilepath = prefix + "../shaders/fragment.spv";
    specification.swapchainExtent = swapchain_extent_;
    specification.swapchainImageFormat = swapchain_format_;
    vkinit::GraphicsPipelineOutBundle output = vkinit::create_graphics_pipeline(specification, debug_mode_);
    pipeline_layout_ = output.layout;
    render_pass_ = output.renderpass;
    pipeline_ = output.pipeline;
}

void Engine::make_frame_buffers()
{
    vkinit::FramebufferInput framebufferInput;
    framebufferInput.device = device_;
    framebufferInput.renderpass = render_pass_;
    framebufferInput.swapchainExtent = swapchain_extent_;
    vkinit::make_frame_buffers(framebufferInput, swap_chain_frames_, debug_mode_);
}

void Engine::make_frame_sync_objects()
{
    for (vkutil::SwapChainFrame &frame: swap_chain_frames_)
    {
        frame.inFlight = vkinit::make_fence(device_, debug_mode_);
        frame.imageAvailable = vkinit::make_semaphore(device_, debug_mode_);
        frame.renderFinished = vkinit::make_semaphore(device_, debug_mode_);
    }
}

void Engine::finalize_setup()
{
    make_frame_buffers();
    command_pool_ = vkinit::make_command_pool(device_, physical_device_, surface_, debug_mode_);
    vkinit::CommandBufferInputChunk commandBufferInput = {device_, command_pool_, swap_chain_frames_};
    main_command_buffer_ = vkinit::make_command_buffer(commandBufferInput, debug_mode_);
    vkinit::make_frame_command_buffer(commandBufferInput, debug_mode_);
    make_frame_sync_objects();
}

void Engine::make_assets()
{
    quad_mesh_ = new QuadMesh(device_, physical_device_);
}

void Engine::prepare_scene(vk::CommandBuffer commandBuffer)
{
    vk::Buffer vertexBuffers[] =
    {
      quad_mesh_->vertex_buffer.buffer
    };
    vk::DeviceSize offsets[] = {0};
    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
}

void Engine::record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex, Scene *scene) {
    vk::CommandBufferBeginInfo beginInfo = {};
    try
    {
        commandBuffer.begin(beginInfo);
    }
    catch (const vk::SystemError &err)
    {
        if (debug_mode_)
        {
            std::cout << "Failed to begin recording command buffer" << std::endl;
        }
    }
    vk::RenderPassBeginInfo renderPassInfo = {};

    renderPassInfo.renderPass = render_pass_;
    renderPassInfo.framebuffer = swap_chain_frames_[imageIndex].framebuffer;
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = swapchain_extent_;

    vk::ClearValue clearColor = {std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f}};

    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_);

    prepare_scene(commandBuffer);
    int index = 0;
    for (glm::vec3 position: scene->triangle_positions_)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        if (index == 1)
        {
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        vkutil::ObjectData objectdata{};
        objectdata.model = model;
        commandBuffer.pushConstants(pipeline_layout_, vk::ShaderStageFlagBits::eVertex, 0, sizeof(objectdata),
                                    &objectdata);
        commandBuffer.draw(4, 1, 0, 0);
        index++;
    }
    commandBuffer.endRenderPass();
    try
    {
        commandBuffer.end();
    }
    catch (const vk::SystemError &err)
    {
        if (debug_mode_)
        {
            std::cout << "Failed to finish recording command buffer" << std::endl;
        }
    }
}

void Engine::render(Scene *scene)
{
    vk::Result waitResult = device_.waitForFences(1, &swap_chain_frames_[static_cast<size_t>(frame_number_)].inFlight,
                                                  VK_TRUE, UINT64_MAX);
    if (waitResult != vk::Result::eSuccess)
    {
        std::cerr << "Error: Failed to wait for fence. Result: " << waitResult << std::endl;
        return;
    }
    uint32_t imageIndex;
    try
    {
        vk::ResultValue acquire = device_.acquireNextImageKHR
                (
                        swapchain_,
                        UINT64_MAX,
                        swap_chain_frames_[static_cast<size_t>(frame_number_)].imageAvailable,
                        nullptr
                );
        imageIndex = acquire.value;
    }
    catch (vk::OutOfDateKHRError &error) {
        std::cout << "Recreate" << std::endl;
        recreate_swapchain();
        return;
    }
    vk::CommandBuffer commandBuffer = swap_chain_frames_[imageIndex].commandbuffer;
    commandBuffer.reset();
    record_draw_commands(commandBuffer, imageIndex, scene);
    vk::SubmitInfo submitInfo = {};
    vk::Semaphore waitSemaphores[] = {swap_chain_frames_[static_cast<size_t>(frame_number_)].imageAvailable};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vk::Semaphore signalSemaphores[] = {swap_chain_frames_[static_cast<size_t>(frame_number_)].renderFinished};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    vk::Result resetResult = device_.resetFences(1, &swap_chain_frames_[static_cast<size_t>(frame_number_)].inFlight);
    if (resetResult != vk::Result::eSuccess) {
        std::cerr << "Error: Failed to reset fences. Result: " << resetResult << std::endl;
    }
    try {
        graphics_queue_.submit(submitInfo, swap_chain_frames_[static_cast<size_t>(frame_number_)].inFlight);
    }
    catch (vk::SystemError &err) {
        if (debug_mode_) {
            std::cout << "Failed to submit draw command buffer" << std::endl;
        }
    }
    vk::PresentInfoKHR presentInfo = {};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    vk::SwapchainKHR swapchains[] = {swapchain_};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;
    vk::Result present;
    try {
        present = present_queue_.presentKHR(presentInfo);
    }
    catch (vk::OutOfDateKHRError &error) {
        present = vk::Result::eErrorOutOfDateKHR;
    }
    if (present == vk::Result::eErrorOutOfDateKHR || present == vk::Result::eSuboptimalKHR) {
        std::cout << "Recreate" << std::endl;
        recreate_swapchain();
        return;
    }
    frame_number_ = (frame_number_ + 1) % max_frames_in_flight_;
}

void Engine::cleanup_swap_chain() {
    for (vkutil::SwapChainFrame frame: swap_chain_frames_) {
        device_.destroyImageView(frame.imageView);
        device_.destroyFramebuffer(frame.framebuffer);
        device_.destroyFence(frame.inFlight);
        device_.destroySemaphore(frame.imageAvailable);
        device_.destroySemaphore(frame.renderFinished);
    }

    device_.destroySwapchainKHR(swapchain_);
}

Engine::~Engine() {
    device_.waitIdle();
    if (debug_mode_) {
        std::cout << "Goodbye see you!\n";
    }
    device_.destroyCommandPool(command_pool_);
    device_.destroyPipeline(pipeline_);
    device_.destroyPipelineLayout(pipeline_layout_);
    device_.destroyRenderPass(render_pass_);
    cleanup_swap_chain();
    delete quad_mesh_;
    device_.destroy();
    instance_.destroySurfaceKHR(surface_);
    if (debug_mode_) {
        instance_.destroyDebugUtilsMessengerEXT(debug_messenger_, nullptr, dldi_);
    }
    instance_.destroy();
    glfwTerminate();
}