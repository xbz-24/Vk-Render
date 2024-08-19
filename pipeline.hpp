#ifndef INC_3DLOADERVK_PIPELINE_HPP
#define INC_3DLOADERVK_PIPELINE_HPP

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include "shaders.hpp"
#include "render_structs.hpp"
#include "mesh.hpp"

namespace vkinit
{
    struct GraphicsPipelineInBundle
    {
        vk::Device device;
        std::string vertexFilepath;
        std::string fragmentFilepath;
        vk::Extent2D swapchainExtent;
        vk::Format swapchainImageFormat;
    };
    struct GraphicsPipelineOutBundle
    {
        vk::PipelineLayout layout;
        vk::RenderPass renderpass;
        vk::Pipeline pipeline;
    };
    vk::PipelineLayout make_pipeline_layout(vk::Device device, bool debug);
    vk::RenderPass make_render_pass(vk::Device device, vk::Format swapchainImageFormat, bool debug);
    GraphicsPipelineOutBundle create_graphics_pipeline(GraphicsPipelineInBundle specification,
                                                       bool debug);
}
#endif //INC_3DLOADERVK_PIPELINE_HPP
