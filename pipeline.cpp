/**
 * @file pipeline.cpp
 * @brief
 * @date Created on 27-12-23.
 * @author Renato German Chavez Chicoma
 */
#include "pipeline.hpp"
namespace vkinit
{

    /**
     * @brief Creates a Vulkan pipeline_ layout
     *
     * Initializes a pipeline_ layout necessary for a Vulkan graphics pipeline_, including push constants
     *
     * @param device The Vulkan logical device_.
     * @param debug Flag indicating whether to enable debug logging.
     * @return The created Vulkan pipeline_ layout.
     */
    vk::PipelineLayout make_pipeline_layout(vk::Device device, bool debug)
    {
        vk::PipelineLayoutCreateInfo layoutInfo;
        layoutInfo.flags = vk::PipelineLayoutCreateFlags();
        layoutInfo.setLayoutCount = 0;

        layoutInfo.pushConstantRangeCount = 1;
        vk::PushConstantRange pushConstantInfo;
        pushConstantInfo.offset = 0;
        pushConstantInfo.size = sizeof(vkutil::ObjectData);
        pushConstantInfo.stageFlags = vk::ShaderStageFlagBits::eVertex;
        layoutInfo.pPushConstantRanges = &pushConstantInfo;

        try
        {
            return device.createPipelineLayout(layoutInfo);
        }
        catch(vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to create pipeline_ pipeline_layout_!" << std::endl;
            }
            return vk::PipelineLayout{};
        }
    }
    /**
     * @brief Creates a Vulkan render pass.
     *
     * Initializes a render pass for the graphics pipeline_, specifying how color and depth attachments are handled.
     *
     * @param device The Vulkan logical device_.
     * @param swapchainImageFormat The format of the swap chain images.
     * @param debug Flag indicating whether to enable debug logging.
     * @return The created Vulkan render pass.
     */
    vk::RenderPass make_renderpass(vk::Device device, vk::Format swapchainImageFormat, bool debug)
    {
        vk::AttachmentDescription colorAttachment = { };
        colorAttachment.flags = vk::AttachmentDescriptionFlags();
        colorAttachment.format = swapchainImageFormat;
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentReference colorAttachmentRef = { };
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::SubpassDescription subpass = { };
        subpass.flags = vk::SubpassDescriptionFlags();
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        vk::RenderPassCreateInfo renderpassInfo = { };
        renderpassInfo.flags = vk::RenderPassCreateFlags();
        renderpassInfo.attachmentCount = 1;
        renderpassInfo.pAttachments = &colorAttachment;
        renderpassInfo.subpassCount = 1;
        renderpassInfo.pSubpasses = &subpass;
        try
        {
            return device.createRenderPass(renderpassInfo);
        }
        catch(vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to create render_pass_!" << std::endl;
            }
            return vk::RenderPass{};
        }
    }
    /**
     * @brief Creates a Vulkan graphics pipeline_
     *
     * Sets up the entire graphics pipeline_, including shader stages, viewport and scissor states,
     * rasterization, multisampling, color blending, and more, based on the provided specifications.
     *
     * @param specification The specifications for creating the graphics pipeline_.
     * @param debug Flag indicating whether to enable debug logging.
     * @return A bundle containing the components of the created graphics pipeline_.
     */
    GraphicsPipelineOutBundle create_graphics_pipeline(GraphicsPipelineInBundle specification, bool debug)
    {

        vk::GraphicsPipelineCreateInfo pipelineInfo = { };

        pipelineInfo.flags = vk::PipelineCreateFlags();

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

        // vertex input
        vk::VertexInputBindingDescription bindingDescription = vkmesh::getPosColorBindingDescription();
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = vkmesh::getPosColorAttributeDescriptions();
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = { };
        vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = 2;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        //Input Assembly
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = { };
        inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
//        inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
        inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleStrip;
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

        //vertex shader
        if(debug)
        {
            std::cout << "Create vertex shader module" << std::endl;
        }
        vk::ShaderModule vertexShader = vkutil::createModule(specification.vertexFilepath, specification.device, debug);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = {};
        vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertexShaderInfo.module = vertexShader;
        vertexShaderInfo.pName = "main";
        shaderStages.push_back(vertexShaderInfo);

        //viewport and scissor
        vk::Viewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(specification.swapchainExtent.width);
        viewport.height = static_cast<float>(specification.swapchainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vk::Rect2D scissor = { };
        scissor.offset.x = 0.0f;
        scissor.offset.y = 0.0f;
        scissor.extent = specification.swapchainExtent;
        vk::PipelineViewportStateCreateInfo viewportState = { };
        viewportState.flags = vk::PipelineViewportStateCreateFlags();
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
        pipelineInfo.pViewportState = &viewportState;

        //rasterizer
        vk::PipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = vk::PolygonMode::eFill;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = vk::CullModeFlagBits::eBack;
        rasterizer.frontFace = vk::FrontFace::eClockwise;
        rasterizer.depthBiasEnable = VK_FALSE;
        pipelineInfo.pRasterizationState = &rasterizer;

        //vertex shader
        if(debug)
        {
            std::cout << "Create fragment shader module" << std::endl;
        }
        vk::ShaderModule fragmentShader = vkutil::createModule(specification.fragmentFilepath, specification.device, debug);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {};
        fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentShaderInfo.module = fragmentShader;
        fragmentShaderInfo.pName = "main";
        shaderStages.push_back(fragmentShaderInfo);
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages = shaderStages.data();

        //multisampling
        vk::PipelineMultisampleStateCreateInfo multisampling = { };
        multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
        pipelineInfo.pMultisampleState = &multisampling;

        //color blend
        vk::PipelineColorBlendAttachmentState colorBlendAttachment = { };
        colorBlendAttachment.colorWriteMask =   vk::ColorComponentFlagBits::eR |
                                                vk::ColorComponentFlagBits::eG |
                                                vk::ColorComponentFlagBits::eB |
                                                vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = VK_FALSE;
        vk::PipelineColorBlendStateCreateInfo colorBlending = { };
        colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = vk::LogicOp::eCopy;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;
        pipelineInfo.pColorBlendState = &colorBlending;

        //pipeline_ pipeline_layout_
        if(debug)
        {
            std::cout << "Create Pipeline Layout" << std::endl;
        }
        vk::PipelineLayout layout = make_pipeline_layout(specification.device, debug);
        pipelineInfo.layout = layout;

        //Renderpass
        if(debug)
        {
            std::cout << "Create RenderPass" << std::endl;
        }
        vk::RenderPass renderpass = make_renderpass(specification.device, specification.swapchainImageFormat, debug);
        pipelineInfo.renderPass = renderpass;

        //extra stuff
        pipelineInfo.basePipelineHandle = nullptr;

        //make the pipeline_
        if(debug)
        {
            std::cout << "Create Graphics Pipeline" << std::endl;
        }
        vk::Pipeline graphicsPipeline;
        try
        {
            graphicsPipeline = (specification.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
        }
        catch(vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to create Graphics Pipeline!"<<std::endl;
            }
        }

        GraphicsPipelineOutBundle output = {};
        output.layout = layout;
        output.renderpass = renderpass;
        output.pipeline = graphicsPipeline;

        specification.device.destroyShaderModule(vertexShader);
        specification.device.destroyShaderModule(fragmentShader);
        return output;
    }
}
