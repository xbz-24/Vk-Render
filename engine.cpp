//
// Created by daily on 27-12-23.
//
/**
 * @class Engine
 *
 * @brief This class represents the main engine of a Vulkan-based graphics application.
 *
 * Engine initializes and manages the Vulkan instances, devices, swapchains, pipelines, and
 * other necessary components to render a scene using Vulkan. It is designed to
 * work with GLFW for window handling. The class provides methods for setting up Vulkan
 * components, recording draw commands, and rendering frames.
 * @param width
 * @param height
 * @param window
 * @param debugMode
 */
#include "engine.hpp"
#include "instance.hpp"
#include "logging.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "framebuffer.hpp"
#include "commands.hpp"
#include "sync.hpp"

/**
 * @brief Constructs an Engine object
 *
 * This constructor initializes the Vulkan instance, logical device, swap chain, and
 * graphics pipeline, among other things. It sets up the graphics engine with the
 * specified width, height, and window, and initializes debugging if requested.
 *
 * @param width The width of the rendering window.
 * @param height The height of the rendering window.
 * @param window Pointer to the GLFWindow.
 * @param debugMode Boolean flag to enable or disable debugging features.
 */
Engine::Engine(int width, int height, GLFWwindow* window, bool debugMode) {
    this->width = width;
    this->height = height;
    this->window = window;
    this->debugMode = debugMode;
    if(debugMode) {
        std::cout << "Making a graphics engine\n";
    }
    make_instance();
    make_device();
    make_pipeline();
    finalize_setup();
    make_assets();
}
/**
 * @brief Initializes the Vulkan instance.
 *
 * This method creates a Vulkan instance with a given debug mode and application name.
 * It also sets up a debug messenger if debugging is enabled, and abstracts the GLFW
 * window surface for Vulkan use.
 */
void Engine::make_instance() {
    instance = vkInit::make_instance(debugMode, "ID Tech 12");
    dldi =  vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
    if(debugMode){
        debugMessenger = vkInit::make_debug_messenger(instance, dldi);
    }
    VkSurfaceKHR c_style_surface;
    if(glfwCreateWindowSurface(instance, window, nullptr, &c_style_surface) != VK_SUCCESS){
        if(debugMode){
            std::cout << "Failed to abstract the glfw surface for Vulkan.\n";
        }
    }
    else if(debugMode){
        std::cout << "Successfully abstracted the glfw surface for Vulkan.\n";
    }
    surface = c_style_surface;
}
/**
 * @brief Sets up the Vulkan device.
 *
 * Chooses and initalizes the physical device, creates the logical device, and sets up
 * the swap chain along with its related components like image format and extent. It also
 * initializes the queues for graphics and presentation
 */
void Engine::make_device() {
    physicalDevice = vkInit::choose_physical_device(instance, debugMode);
    //vkInit::findQueueFamilies(physicalDevice, debugMode);
    device = vkInit::create_logical_device(physicalDevice, surface, debugMode);
    std::array<vk::Queue, 2> queues = vkInit::get_queues(physicalDevice, device, surface, debugMode);
    graphicsQueue = queues[0];
    presentQueue = queues[1];
    make_swapchain();
    frameNumber = 0;
    //vkInit::query_swapchain_support(physicalDevice, surface, true);
}

void Engine::make_swapchain() {
    vkInit::SwapChainBundle bundle = vkInit::create_swapchain(device,physicalDevice,surface,width,height,debugMode);
    swapchain = bundle.swapchain;
    swapChainFrames = bundle.frames;
    swapchainFormat = bundle.format;
    swapchainExtent = bundle.extent;
    maxFramesInFlight = static_cast<int>(swapChainFrames.size());
}

void Engine::recreate_swapchain() {
    width = 0;
    height = 0;
    while(width == 0 || height == 0){
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    device.waitIdle();
    cleanup_swapchain();
    make_swapchain();
    make_framebuffers();
    make_frame_sync_objects();
    vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool, swapChainFrames };
    vkInit::make_frame_command_buffer(commandBufferInput, debugMode);
}

/**
 * @brief Configures the graphics pipeline.
 *
 * This method creates and configures the Vulkan graphics pipeline, including the
 * shader stages, render pass, and pipeline layout.
 */
void Engine::make_pipeline() {
    vkInit::GraphicsPipelineInBundle specification = { };
    specification.device = device;
    specification.vertexFilepath = "../shaders/vertex.spv";
    specification.fragmentFilepath = "../shaders/fragment.spv";
    specification.swapchainExtent = swapchainExtent;
    specification.swapchainImageFormat = swapchainFormat;
    vkInit::GraphicsPipelineOutBundle output = vkInit::create_graphics_pipeline(specification, debugMode);
    pipelineLayout = output.layout;
    renderpass = output.renderpass;
    pipeline = output.pipeline;
}

void Engine::make_framebuffers() {
    vkInit::framebufferInput framebufferInput;
    framebufferInput.device = device;
    framebufferInput.renderpass = renderpass;
    framebufferInput.swapchainExtent = swapchainExtent;
    vkInit::make_framebuffers(framebufferInput, swapChainFrames, debugMode);
}
void Engine::make_frame_sync_objects() {
    for(vkUtil::SwapChainFrame& frame : swapChainFrames){
        frame.inFlight = vkInit::make_fence(device, debugMode);
        frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
        frame.renderFinished = vkInit::make_semaphore(device, debugMode);
    }
}
/**
 * @brief Finalizes the engine setup.
 *
 * Completes the engine setup by crafting framebuffers, command pools, and command
 * buffers. It also prepares synchronization primitives for rendering.
 */
void Engine::finalize_setup() {
    make_framebuffers();
    commandPool = vkInit::make_command_pool(device,physicalDevice,surface, debugMode);
    vkInit::commandBufferInputChunk commandBufferInput = { device, commandPool, swapChainFrames };
    mainCommandBuffer = vkInit::make_command_buffer(commandBufferInput, debugMode);
    vkInit::make_frame_command_buffer(commandBufferInput, debugMode);
    make_frame_sync_objects();
}

void Engine::make_assets() {
    triangleMesh = new TriangleMesh(device, physicalDevice);
}

void Engine::prepare_scene(vk::CommandBuffer commandBuffer) {
    vk::Buffer vertexBuffers[] = { triangleMesh -> vertexBuffer.buffer };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
}

/**
 * @brief Records draw commands into a command buffer.
 *
 * Prepares a command buffer for drawing operations, binding the graphics pipeline and
 * specifying the vertex data to be rendered. It also manages render passes and push
 * constants for each frame.
 *
 * @param commandBuffer The command buffer to record the drawing commands into.
 * @param imageIndex The index of the swap chain image that will be rendered.
 * @param scene Pointer to the scene to be rendered.
 */
void Engine::record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex, Scene* scene) {
    vk::CommandBufferBeginInfo beginInfo = { };
    try{
        commandBuffer.begin(beginInfo);
    }
    catch(vk::SystemError err){
        if(debugMode){
            std::cout << "Failed to begin recording command buffer" << std::endl;
        }
    }
    vk::RenderPassBeginInfo renderPassInfo = { };
    renderPassInfo.renderPass = renderpass;
    renderPassInfo.framebuffer = swapChainFrames[imageIndex].framebuffer;
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = swapchainExtent;

    vk::ClearValue clearColor = { std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics , pipeline);

    prepare_scene(commandBuffer);

    for(glm::vec3 position : scene->trianglePositions){
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        vkUtil::ObjectData objectdata;
        objectdata.model = model;
        commandBuffer.pushConstants(pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(objectdata), &objectdata);
        commandBuffer.draw(3, 1, 0, 0);
    }
    commandBuffer.endRenderPass();
    try{
        commandBuffer.end();
    }
    catch(vk::SystemError err){
        if(debugMode){
            std::cout << "Failed to finish recording command buffer" << std::endl;
        }
    }
}
/**
 * @brief Renders a frame.
 *
 * Handles the rendering of a single frame. This involves acquiring an image from the
 * swap chain, recording drawing commands, submitting the command buffer to the graphics
 * queue, and presenting the rendered image to the screen.
 *
 * @param scene Pointer to the scene to be rendered.
 */
void Engine::render(Scene* scene){
    device.waitForFences(1, &swapChainFrames[frameNumber].inFlight, VK_TRUE, UINT64_MAX);
    uint32_t imageIndex;
    try {
        vk::ResultValue acquire = device.acquireNextImageKHR(
                swapchain, UINT64_MAX, swapChainFrames[frameNumber].imageAvailable, nullptr
        );
        imageIndex = acquire.value;
    }
    catch(vk::OutOfDateKHRError){
        std::cout << "Recreate" << std::endl;
        recreate_swapchain();
        return;
    }
    vk::CommandBuffer commandBuffer = swapChainFrames[imageIndex].commandbuffer;
    commandBuffer.reset();
    record_draw_commands(commandBuffer, imageIndex, scene);
    vk::SubmitInfo submitInfo = { };
    vk::Semaphore waitSemaphores[] = { swapChainFrames[frameNumber].imageAvailable };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vk::Semaphore signalSemaphores[] = { swapChainFrames[frameNumber].renderFinished };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    device.resetFences(1,&swapChainFrames[frameNumber].inFlight);
    try{
        graphicsQueue.submit(submitInfo, swapChainFrames[frameNumber].inFlight);
    }
    catch(vk::SystemError err){
        if(debugMode){
            std::cout << "Failed to submit draw command buffer" << std::endl;
        }
    }
    vk::PresentInfoKHR presentInfo = {};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    vk::SwapchainKHR swapchains[] = { swapchain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;
    vk::Result present;
    try{
        present = presentQueue.presentKHR(presentInfo);
    }
    catch(vk::OutOfDateKHRError error) {
        present = vk::Result::eErrorOutOfDateKHR;
    }
    if(present == vk::Result::eErrorOutOfDateKHR || present == vk::Result::eSuboptimalKHR){
        std::cout << "Recreate" << std::endl;
        recreate_swapchain();
        return;
    }
    frameNumber = (frameNumber + 1) % maxFramesInFlight;
}
void Engine::cleanup_swapchain() {
    for(vkUtil::SwapChainFrame frame : swapChainFrames){
        device.destroyImageView(frame.imageView);
        device.destroyFramebuffer(frame.framebuffer);
        device.destroyFence(frame.inFlight);
        device.destroySemaphore(frame.imageAvailable);
        device.destroySemaphore(frame.renderFinished);
    }

    device.destroySwapchainKHR(swapchain);
}

/**
 * @brief Destructor for the Engine class.
 *
 * Cleans up and releases all Vulkan resources, including the device, swap chain,
 * command pool, and any synchronization primitives. Also handles the termination of GLFW.
 */
Engine::~Engine() {
    device.waitIdle();
    if(debugMode) {
        std::cout << "Goodbye see you!\n";
    }
//    device.destroyFence(inFlightFence);
//    device.destroySemaphore(imageAvailable);
//    device.destroySemaphore(renderFinished);
//
    device.destroyCommandPool(commandPool);
    device.destroyPipeline(pipeline);
    device.destroyPipelineLayout(pipelineLayout);
    device.destroyRenderPass(renderpass);
    cleanup_swapchain();
    delete triangleMesh;
    device.destroy();
    instance.destroySurfaceKHR(surface);
    if(debugMode){
        instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
    }
    instance.destroy();
    // terminate glfw
    glfwTerminate();
}