#include "framebuffer.hpp"

namespace vkinit
{
    void make_frame_buffers(FramebufferInput inputChunk, std::vector<vkutil::SwapChainFrame>& frames, bool debug)
    {
        for(size_t i = 0; i < frames.size(); i++)
        {
            std::vector<vk::ImageView> attachments =
            {
               frames[i].imageView
            };

            vk::FramebufferCreateInfo framebufferInfo = { };

            framebufferInfo.flags = vk::FramebufferCreateFlags();
            framebufferInfo.renderPass = inputChunk.renderpass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = inputChunk.swapchainExtent.width;
            framebufferInfo.height = inputChunk.swapchainExtent.height;
            framebufferInfo.layers = 1;

            try
            {
                frames[i].framebuffer = inputChunk.device.createFramebuffer(framebufferInfo);
                if(debug)
                {
                    std::cout << "Created framebuffer for frame " << i << std::endl;
                }
            }
            catch(const vk::SystemError &err)
            {
                if(debug)
                {
                    std::cout << "Failed to create framebuffer for frame " << i << std::endl;
                }
            }
        }
    }
}
