#include "shaders.hpp"

namespace vkutil
{
    std::vector<char> read_file(std::string filename, bool debug)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if(debug && !file.is_open())
        {
            std::cout << "Failed to load \"" << filename << "\"" << std::endl;
        }

        size_t filesize
        {
            static_cast<size_t>(file.tellg())
        };

        std::vector<char> buffer(filesize);

        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(filesize));
        file.close();

        return buffer;
    }
    vk::ShaderModule create_module(const std::string& filename, vk::Device device, bool debug)
    {
        std::vector<char> sourceCode = read_file(filename, debug);

        vk::ShaderModuleCreateInfo moduleInfo = {};

        moduleInfo.flags = vk::ShaderModuleCreateFlags();
        moduleInfo.codeSize = sourceCode.size();
        moduleInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());

        try
        {
            return device.createShaderModule(moduleInfo);
        }
        catch(const vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to create shader module for \""
                          << filename
                          << "\""
                          << std::endl;
            }

            return vk::ShaderModule{};
        }
    }
}