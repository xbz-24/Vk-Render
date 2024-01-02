//
// Created by daily on 27-12-23.
//
/**
 * @file framebuffer.hpp
 * @brief Defines functionality for crafting Vulkan framebuffers.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_FRAMEBUFFER_HPP
#define INC_3DLOADERVK_FRAMEBUFFER_HPP

#include "config.hpp"
#include "frame.hpp"

/**
 * @namespace vkInit
 * @brief Encapsulates Vulkan initialization functionalities.
 *
 * This namespace includes utility functions for setting up various Vulkan components,
 * such as framebuffers, which are essential for rendering operations in Vulkan.
 */
namespace vkInit{
    /**
     * @struct framebufferInput
     * @brief Holds parameters required for creating Vulkan framebuffers.
     *
     * This structure encapsulates the Vulkan logical device, render pass, and swap chain
     * extent, providing the necessary inputs for framebuffer creation.
     */
    struct framebufferInput{
        vk::Device device;
        vk::RenderPass renderpass;
        vk::Extent2D swapchainExtent;
    };
    /**
     * @brief Creates framebuffers for each swap chain image.
     *
     * Iterates over the swap chain frames and creates a framebuffer for each one using
     * the provided device, render pass, and swap chain extent. This function is essential
     * for setting up the render targets for the swap chain images.
     *
     * @param inputChunk The parameters for creating framebuffers.
     * @param frames A vector of SwapChainFrame objects to create framebuffers for.
     * @param debug Flag indicating whether to enable debug logging.
     */
    void make_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug){
        for(int i = 0; i < frames.size(); i++){
            std::vector<vk::ImageView> attachments = {
              frames[i].imageView
            };
            vk::FramebufferCreateInfo framebufferInfo = { };
            framebufferInfo.flags = vk::FramebufferCreateFlags();
            framebufferInfo.renderPass = inputChunk.renderpass;
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = inputChunk.swapchainExtent.width;
            framebufferInfo.height = inputChunk.swapchainExtent.height;
            framebufferInfo.layers = 1;
            try{
                frames[i].framebuffer = inputChunk.device.createFramebuffer(framebufferInfo);
                if(debug){
                    std::cout << "Created framebuffer for frame "<< i << std::endl;
                }
            }
            catch(vk::SystemError err){
                if(debug){
                    std::cout << "Failed to create framebuffer for frame " << i << std::endl;
                }
            }
        }
    }
}



#endif //INC_3DLOADERVK_FRAMEBUFFER_HPP
