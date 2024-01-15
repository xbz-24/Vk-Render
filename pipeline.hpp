//
// Created by daily on 27-12-23.
//
/**
 * @file pipeline_.hpp
 * @brief Defines functionality for creating Vulkan graphics pipelines.
 * @date Created by Renato on 27-12-23.
 */
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
    /**
     * @struct GraphicsPipelineInBundle
     * @brief Holds parameters required for creating a Vulkan graphics pipeline_.
     *
     * This structures includes the Vulkan device_, file paths for vertex and fragment shaders,
     * and specifications related to the swap chain such as image format and extent.
     */
    struct GraphicsPipelineInBundle
    {
        vk::Device device;
        std::string vertexFilepath;
        std::string fragmentFilepath;
        vk::Extent2D swapchainExtent;
        vk::Format swapchainImageFormat;
    };
    /**
     * @struct GraphicsPipelineOutBundle
     * @brief Holds the components of a created Vulkan graphics pipeline_
     *
     * This structure encapsulates the pipeline_ layout, render pass, and the graphics pipeline_ itself.
     */
    struct GraphicsPipelineOutBundle
    {
        vk::PipelineLayout layout;
        vk::RenderPass renderpass;
        vk::Pipeline pipeline;
    };
    /**
     * @brief Creates a Vulkan pipeline_ layout
     *
     * Initializes a pipeline_ layout necessary for a Vulkan graphics pipeline_, including push constants
     *
     * @param device The Vulkan logical device_.
     * @param debug Flag indicating whether to enable debug logging.
     * @return The created Vulkan pipeline_ layout.
     */
    vk::PipelineLayout make_pipeline_layout(vk::Device device, bool debug);
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
    vk::RenderPass make_renderpass(vk::Device device, vk::Format swapchainImageFormat, bool debug);
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
    GraphicsPipelineOutBundle create_graphics_pipeline(GraphicsPipelineInBundle specification, bool debug);
}
#endif //INC_3DLOADERVK_PIPELINE_HPP
