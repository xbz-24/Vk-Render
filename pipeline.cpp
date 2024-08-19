#include "pipeline.hpp"

namespace vkinit
{
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
        catch(const vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to create pipeline_ pipeline_layout_!" << std::endl;
            }
            return vk::PipelineLayout{};
        }
    }
    vk::RenderPass make_render_pass(vk::Device device, vk::Format swapchainImageFormat, bool debug)
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
        catch(const vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to create render_pass_!" << std::endl;
            }
            return vk::RenderPass{};
        }
    }
    GraphicsPipelineOutBundle create_graphics_pipeline(GraphicsPipelineInBundle specification, bool debug)
    {

        vk::GraphicsPipelineCreateInfo pipelineInfo = { };

        pipelineInfo.flags = vk::PipelineCreateFlags();

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

        vk::VertexInputBindingDescription bindingDescription = vkmesh::getPosColorBindingDescription();
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = vkmesh::getPosColorAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = { };

        vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = 2;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;

        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = { };
        inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
        inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleStrip;

        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

        if(debug)
        {
            std::cout << "Create vertex shader module" << std::endl;
        }

        vk::ShaderModule vertexShader = vkutil::create_module(specification.vertexFilepath, specification.device, debug);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo = {};

        vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertexShaderInfo.module = vertexShader;
        vertexShaderInfo.pName = "main";

        shaderStages.push_back(vertexShaderInfo);

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

        if(debug)
        {
            std::cout << "Create fragment shader module" << std::endl;
        }
        vk::ShaderModule fragmentShader = vkutil::create_module(specification.fragmentFilepath, specification.device,
                                                                debug);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {};
        fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentShaderInfo.module = fragmentShader;
        fragmentShaderInfo.pName = "main";
        shaderStages.push_back(fragmentShaderInfo);
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages = shaderStages.data();

        vk::PipelineMultisampleStateCreateInfo multisampling = { };

        multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
        pipelineInfo.pMultisampleState = &multisampling;

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

        if(debug)
        {
            std::cout << "Create Pipeline Layout" << std::endl;
        }

        vk::PipelineLayout layout = make_pipeline_layout(specification.device, debug);
        pipelineInfo.layout = layout;

        if(debug)
        {
            std::cout << "Create RenderPass" << std::endl;
        }

        vk::RenderPass renderpass = make_render_pass(specification.device, specification.swapchainImageFormat, debug);
        pipelineInfo.renderPass = renderpass;

        pipelineInfo.basePipelineHandle = nullptr;

        if(debug)
        {
            std::cout << "Create Graphics Pipeline" << std::endl;
        }

        vk::Pipeline graphicsPipeline;

        try
        {
            graphicsPipeline = (specification.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
        }
        catch(const vk::SystemError &err)
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