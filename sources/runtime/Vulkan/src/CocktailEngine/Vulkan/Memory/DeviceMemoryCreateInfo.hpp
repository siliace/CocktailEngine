#ifndef COCKTAILENGINE_VULKAN_MEMORY_DEVICEMEMORYCREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_MEMORY_DEVICEMEMORYCREATEINFO_HPP

#include <utility>

namespace Ck::Vulkan
{
    class AbstractTexture;
    class Buffer;

    /**
     * \brief
     */
    struct DeviceMemoryCreateInfo
    {
        float Priority = 1.f;

        /**
         * \brief
         */
        std::size_t Size = 0;

        /**
         * \brief
         */
        unsigned int MemoryTypeIndex = 0;

        /**
         * \brief
         */
        bool Dedicated = false;

        /**
         * \brief
         */
        const AbstractTexture* TextureResource;

        /**
         * \brief
         */
        const Buffer* BufferResource;
    };
} // namespace Ck::Vulkan

#endif // COCKTAILENGINE_VULKAN_MEMORY_DEVICEMEMORYCREATEINFO_HPP
