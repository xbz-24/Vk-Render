/**
 * @Class Engine
 * @Brief This class represents the main engine of a Vulkan-based graphics application.
 * @Author Renato
 * @Date Created on 27-12-23
 *
 * Engine initializes and manages the Vulkan instances, devices, swapchains, pipelines, and
 * other necessary components to render a scene_ using Vulkan. It is designed to
 * work with GLFW for window_ handling. The class provides methods for setting up Vulkan
 * components, recording draw commands, and rendering frames.
 * @param width
 * @param height
 * @param window
 * @param debugMode
 */
#include "engine.hpp"

/**
 * @brief Constructs an Engine object
 *
 * This constructor initializes the Vulkan instance_, logical device_, swap chain, and
 * graphics pipeline_, among other things. It sets up the graphics engine with the
 * specified width_, height_, and window_, and initializes debugging if requested.
 *
 * @param width The width_ of the rendering window_.
 * @param height The height_ of the rendering window_.
 * @param window Pointer to the GLFWindow.
 * @param debugMode Boolean flag to enable or disable debugging features.
 */
Engine::Engine(int width,
               int height,
               GLFWwindow* window,
               bool debugMode) {
    this->width_ = width;
    this->height_ = height;
    this->window_ = window;
    this->debug_mode_ = debugMode;
    if (debugMode) {
        std::cout << std::format("Making a graphics engine\n");
    }
    MakeInstance();
    MakeDevice();
    MakePipeline();
    FinalizeSetup();
    MakeAssets();
}
/**
 * @brief Initializes the Vulkan instance_.
 *
 * This method creates a Vulkan instance_ with a given debug mode and application name.
 * It also sets up a debug messenger if debugging is enabled, and abstracts the GLFW
 * window_ surface_ for Vulkan use.
 */
void Engine::MakeInstance() {
    instance_ = vkinit::make_instance(debug_mode_, "ID Tech 12");
    dldi_ =  vk::DispatchLoaderDynamic(instance_, 
                                       vkGetInstanceProcAddr);
    if (debug_mode_) {
        debug_messenger_ = vkinit::make_debug_messenger(instance_, 
                                                        dldi_);
    }
    VkSurfaceKHR c_style_surface;
    if (glfwCreateWindowSurface(instance_, window_ ,nullptr , &c_style_surface) != VK_SUCCESS) [[unlikely]] {
        if (debug_mode_) {
            std::cout << std::format("Failed to abstract the glfw surface_ for Vulkan.\n");
        }
    }
    else [[likely]] {
        if(debug_mode_){
            std::cout << std::format("Successfully abstracted the glfw surface_ for Vulkan.\n");
        }
    }
    surface_ = c_style_surface;
}
/**
 * @brief Sets up the Vulkan device_.
 *
 * Chooses and initalizes the physical device_, creates the logical device_, and sets up
 * the swap chain along with its related components like image format and extent. It also
 * initializes the queues for graphics and presentation
 */
void Engine::MakeDevice() {
    std::optional<vk::PhysicalDevice> physical_device_result = vkinit::ChoosePhysicalDevice(instance_,
                                                                                            debug_mode_);
    if(!physical_device_result.has_value()){
        throw std::runtime_error("Failed to find a suitable GPU! (choose physical device)");
    }
    
    physical_device_ = *physical_device_result; //physical_device_result.value()
//    physical_device_ = vkinit::ChoosePhysicalDevice(instance_,
//                                                    debug_mode_);
    
    
    //vkinit::findQueueFamilies(physical_device_, debug_mode_);
    std::optional<vk::Device> device_result = vkinit::CreateLogicalDevice(physical_device_,
                                                                         surface_,
                                                                         debug_mode_);
    if(!device_result.has_value()){
        throw std::runtime_error("Failed to create logical device)");
    }
    
    device_ = *device_result;
    
//    device_ = vkinit::CreateLogicalDevice(physical_device_,
//                                          surface_,
//                                          debug_mode_);
    auto [graphics_queue, present_queue] = vkinit::GetQueues(physical_device_, device_, surface_, debug_mode_);
    graphics_queue_ = graphics_queue;
    present_queue_ = present_queue;
    MakeSwapchain();
    frame_number_ = 0;
    //vkinit::query_swapchain_support(physical_device_, surface_, true);
}
void Engine::MakeSwapchain(){
    vkinit::SwapChainBundle bundle = vkinit::create_swapchain(device_, 
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
void Engine::RecreateSwapchain(){
    width_ = 0;
    height_ = 0;
    while(width_ == 0 || height_ == 0){
        glfwGetFramebufferSize(window_, 
                               &width_,
                               &height_);
        glfwWaitEvents();
    }
    device_.waitIdle();
    CleanupSwapchain();
    MakeSwapchain();
    MakeFramebuffers();
    MakeFrameSyncObjects();
    vkinit::commandBufferInputChunk commandBufferInput = {  
        device_,
        command_pool_,
        swap_chain_frames_
    };
    vkinit::make_frame_command_buffer(commandBufferInput, 
                                      debug_mode_);
}
/**
 * @brief Configures the graphics pipeline_.
 *
 * This method creates and configures the Vulkan graphics pipeline_, including the
 * shader stages, render pass, and pipeline_ layout.
 */
void Engine::MakePipeline(){
    vkinit::GraphicsPipelineInBundle specification = { 
    };
    specification.device = device_;
//    std::string vertexShaderPath, fragmentShaderPath;
//    try {
//        std::filesystem::path currentPath = std::filesystem::current_path();
//        std::cout << "Current Path: " << currentPath << std::endl;
//        currentPath = "";
//        std::string shaderBasePath = (currentPath / "Developer" / "Vk-Render" / "shaders/").string();
//        std::cout << "shaderBase Path: " << shaderBasePath << std::endl;
//        vertexShaderPath = shaderBasePath + "vertex.spv";
//        fragmentShaderPath = shaderBasePath + "fragment.spv";
//    } catch (const std::filesystem::filesystem_error& e) {
//        std::cerr << "Filesystem error: " << e.what() << '\n';
//    } catch (const std::exception& e) {
//        std::cerr << "General exception: " << e.what() << '\n';
//    }
    std::string vertexShaderPath = "/Users/xbz/Developer/Vk-Render/shaders/vertex.spv";
    std::string fragmentShaderPath = "/Users/xbz/Developer/Vk-Render/shaders/fragment.spv";
    specification.vertexFilepath = vertexShaderPath;
    specification.fragmentFilepath = fragmentShaderPath;
    specification.swapchainExtent = swapchain_extent_;
    specification.swapchainImageFormat = swapchain_format_;
    vkinit::GraphicsPipelineOutBundle output = vkinit::create_graphics_pipeline(specification, 
                                                                                debug_mode_);
    pipeline_layout_ = output.layout;
    render_pass_ = output.renderpass;
    pipeline_ = output.pipeline;
}
void Engine::MakeFramebuffers(){
    vkinit::framebufferInput framebufferInput;
    framebufferInput.device = device_;
    framebufferInput.renderpass = render_pass_;
    framebufferInput.swapchainExtent = swapchain_extent_;
    vkinit::make_framebuffers(framebufferInput, 
                              swap_chain_frames_,
                              debug_mode_);
}
void Engine::MakeFrameSyncObjects(){
    for(vkutil::SwapChainFrame& frame : swap_chain_frames_){
        frame.inFlight = vkinit::make_fence(device_, 
                                            debug_mode_);
        frame.imageAvailable = vkinit::make_semaphore(device_, 
                                                      debug_mode_);
        frame.renderFinished = vkinit::make_semaphore(device_, 
                                                      debug_mode_);
    }
}
/**
 * @brief Finalizes the engine setup.
 *
 * Completes the engine setup by crafting framebuffers, command pools, and command
 * buffers. It also prepares synchronization primitives for rendering.
 */
void Engine::FinalizeSetup(){
    MakeFramebuffers();
    
    //    command_pool_ = vkinit::make_command_pool(device_, physical_device_, surface_, debug_mode_);
    // replacing the above
    // for the below
    VulkanResult<vk::CommandPool> command_pool_result = vkinit::make_command_pool(device_,
                                                                                physical_device_,
                                                                                surface_,
                                                                                debug_mode_);
    if (!command_pool_result.isSuccess()) {
        throw std::runtime_error("Failed to create command pool: " + command_pool_result.error);
    }
    command_pool_ = command_pool_result.value.value();
    
    vkinit::commandBufferInputChunk commandBufferInput = {
        device_, command_pool_, swap_chain_frames_
    };
    VulkanResult<vk::CommandBuffer> command_buffer_result = vkinit::make_command_buffer(commandBufferInput,
                                                                                        debug_mode_);
    if (!command_buffer_result.isSuccess()) {
        throw std::runtime_error("Failed to allocate main command buffer: " + command_buffer_result.error);
    }
    main_command_buffer_ = command_buffer_result.value.value();

//    main_command_buffer_ = vkinit::make_command_buffer(commandBufferInput, debug_mode_);
    
    vkinit::make_frame_command_buffer(commandBufferInput, debug_mode_);
    MakeFrameSyncObjects();
}

void Engine::MakeAssets(){
//    triangle_mesh_ = std::make_unique<TriangleMesh>(device_, physical_device_);
    
    auto initializeAllocator = [this]() {
        VmaAllocator allocator;

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.instance = instance_;
        allocatorInfo.physicalDevice = physical_device_;
        allocatorInfo.device = device_;

        if (vmaCreateAllocator(&allocatorInfo, &allocator) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan Memory Allocator.");
        }

        return allocator;
    };

    // Call lambda function to initialize allocator
    allocator_ = initializeAllocator();
    
    // Now, you can proceed with creating assets using the allocator
    quad_mesh_ = std::make_unique<QuadMesh>(device_, physical_device_, allocator_);

}

void Engine::PrepareScene(vk::CommandBuffer commandBuffer){
    vk::Buffer vertexBuffers[] = {
        //            triangle_mesh_ -> vertex_buffer.buffer,
        quad_mesh_ -> vertex_buffer.buffer
    };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
}

/**
 * @brief Records draw commands into a command buffer.
 *
 * Prepares a command buffer for drawing operations, binding the graphics pipeline_ and
 * specifying the vertex data to be rendered. It also manages render passes and push
 * constants for each frame.
 *
 * @param commandBuffer The command buffer to record the drawing commands into.
 * @param imageIndex The index of the swap chain image that will be rendered.
 * @param scene Pointer to the scene_ to be rendered.
 */
void Engine::RecordDrawCommands(vk::CommandBuffer commandBuffer, 
                                uint32_t imageIndex,
                                Scene* scene) {
    vk::CommandBufferBeginInfo beginInfo = { };
    try{
        commandBuffer.begin(beginInfo);
    }
    catch(vk::SystemError &err){
        if(debug_mode_){
            std::cout << std::format("Failed to begin recording command buffer\n");
        }
    }
    vk::RenderPassBeginInfo renderPassInfo = { };
    renderPassInfo.renderPass = render_pass_;
    renderPassInfo.framebuffer = swap_chain_frames_[imageIndex].framebuffer;
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = swapchain_extent_;

    vk::ClearValue clearColor = { std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    commandBuffer.beginRenderPass(&renderPassInfo, 
                                  vk::SubpassContents::eInline);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics , 
                               pipeline_);

    PrepareScene(commandBuffer);
    int index = 0;
    for(glm::vec3 position : scene->triangle_positions_){
        glm::mat4 model = glm::translate(glm::mat4(1.0f), 
                                         position);
        if(index == 1){
            model = glm::rotate(model, 
                                glm::radians(180.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f));
        }
        vkutil::ObjectData objectdata{ 
            
        };
        objectdata.model = model;
        commandBuffer.pushConstants(pipeline_layout_,
                                    vk::ShaderStageFlagBits::eVertex,
                                    0,
                                    sizeof(objectdata), 
                                    &objectdata);
        commandBuffer.draw(4, 
                           1,
                           0,
                           0);
        index++;
    }
    commandBuffer.endRenderPass();
    try{
        commandBuffer.end();
    }
    catch(vk::SystemError &err){
        if(debug_mode_){
            std::cout << std::format("Failed to finish recording command buffer\n");
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
 * @param scene Pointer to the scene_ to be rendered.
 */
void Engine::render(Scene* scene){
    vk::Result waitResult = device_.waitForFences(1, 
                                                  &swap_chain_frames_[static_cast<size_t>(frame_number_)].inFlight,
                                                  VK_TRUE,
                                                  UINT64_MAX);
    if (waitResult != vk::Result::eSuccess){
        std::cerr << "Error: Failed to wait for fence. Result: " << waitResult << std::endl;
        return;
    }
    uint32_t imageIndex;
    try{
        vk::ResultValue acquire = device_.acquireNextImageKHR(swapchain_,
                                                              UINT64_MAX,
                                                              swap_chain_frames_[static_cast<size_t>(frame_number_)].imageAvailable,
                                                              nullptr);
        imageIndex = acquire.value;
    }
    catch(vk::OutOfDateKHRError &error){
        std::cout << std::format("Recreate\n");
        RecreateSwapchain();
        return;
    }
    vk::CommandBuffer commandBuffer = swap_chain_frames_[imageIndex].commandbuffer;
    commandBuffer.reset();
    RecordDrawCommands(commandBuffer, 
                       imageIndex,
                       scene);
    vk::SubmitInfo submitInfo = {
        
    };
    vk::Semaphore waitSemaphores[] = {
        swap_chain_frames_[static_cast<size_t>(frame_number_)].imageAvailable
    };
    vk::PipelineStageFlags waitStages[] = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput
    };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vk::Semaphore signalSemaphores[] = {
        swap_chain_frames_[static_cast<size_t>(frame_number_)].renderFinished
    };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    vk::Result resetResult = device_.resetFences(1, &swap_chain_frames_[static_cast<size_t>(frame_number_)].inFlight);
    if (resetResult != vk::Result::eSuccess){
        std::cerr << "Error: Failed to reset fences. Result: " << resetResult << std::endl;
    }
    try{
        graphics_queue_.submit(submitInfo, swap_chain_frames_[static_cast<size_t>(frame_number_)].inFlight);
    }
    catch(vk::SystemError &err){
        if(debug_mode_){
            std::cout << std::format("Failed to submit draw command buffer\n");
        }
    }
    vk::PresentInfoKHR presentInfo = { 
        
    };
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    vk::SwapchainKHR swapchains[] = {
        swapchain_
    };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;
    vk::Result present;
    try{
        present = present_queue_.presentKHR(presentInfo);
    }
    catch(vk::OutOfDateKHRError &error){
        present = vk::Result::eErrorOutOfDateKHR;
    }
    if(present == vk::Result::eErrorOutOfDateKHR || present == vk::Result::eSuboptimalKHR){
        std::cout << std::format("Recreate\n");
        RecreateSwapchain();
        return;
    }
    frame_number_ = (frame_number_ + 1) % max_frames_in_flight_;
}
void Engine::CleanupSwapchain(){
    for(vkutil::SwapChainFrame frame : swap_chain_frames_){
        device_.destroyImageView(frame.imageView);
        device_.destroyFramebuffer(frame.framebuffer);
        device_.destroyFence(frame.inFlight);
        device_.destroySemaphore(frame.imageAvailable);
        device_.destroySemaphore(frame.renderFinished);
    }
    device_.destroySwapchainKHR(swapchain_);
}

/**
 * @brief Destructor for the Engine class.
 *
 * Cleans up and releases all Vulkan resources, including the device_, swap chain,
 * command pool, and any synchronization primitives. Also handles the termination of GLFW.
 */
Engine::~Engine(){
    device_.waitIdle();
    if(debug_mode_) {
        std::cout << std::format("Goodbye see you!\n");
    }
//    device_.destroyFence(inFlightFence);
//    device_.destroySemaphore(imageAvailable);
//    device_.destroySemaphore(renderFinished);
//
    device_.destroyCommandPool(command_pool_);
    device_.destroyPipeline(pipeline_);
    device_.destroyPipelineLayout(pipeline_layout_);
    device_.destroyRenderPass(render_pass_);
    CleanupSwapchain();
//    delete triangle_mesh_;
//    delete quad_mesh_;
    device_.destroy();
    instance_.destroySurfaceKHR(surface_);
    if(debug_mode_){
        instance_.destroyDebugUtilsMessengerEXT(debug_messenger_, nullptr, dldi_);
    }
    instance_.destroy();
    glfwTerminate();
}
