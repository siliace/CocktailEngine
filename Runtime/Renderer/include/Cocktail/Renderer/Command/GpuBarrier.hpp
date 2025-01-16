#ifndef COCKTAIL_RENDERER_COMMAND_GPUBARRIER_HPP
#define COCKTAIL_RENDERER_COMMAND_GPUBARRIER_HPP

#include <Cocktail/Renderer/Command/CommandQueueType.hpp>
#include <Cocktail/Renderer/Memory/ResourceState.hpp>
#include <Cocktail/Renderer/Texture/TextureSubResource.hpp>

namespace Ck::Renderer
{
    class Buffer;
    class Texture;

	/**
     * \brief 
     */
    enum class GpuBarrierType
    {
	    /**
         * \brief 
         */
        Memory,

	    /**
         * \brief 
         */
        Buffer,

	    /**
         * \brief 
         */
        Texture,
    };

    /**
     * \brief 
     */
    struct ResourceQueueTransfer
    {
	    /**
         * \brief 
         */
        CommandQueueType SourceQueueType;

	    /**
         * \brief 
         */
        CommandQueueType DestinationQueueType;
    };

    /**
     * \brief 
     */
    struct GpuBarrier
    {
	    /**
         * \brief
         */
        struct BufferBarrier
        {
	        /**
             * \brief 
             */
            const Buffer* Resource = nullptr;

	        /**
             * \brief 
             */
            ResourceState OldState;

	        /**
             * \brief 
             */
            ResourceState NewState;

	        /**
             * \brief 
             */
            std::size_t Offset = 0;

	        /**
             * \brief 
             */
            std::size_t Size = 0;
        };

	    /**
         * \brief 
         */
        struct TextureBarrier
        {
	        /**
             * \brief 
             */
            const Texture* Resource = nullptr;

	        /**
             * \brief 
             */
            TextureSubResource SubResource;

	        /**
             * \brief 
             */
            ResourceState OldState;

	        /**
             * \brief 
             */
            ResourceState NewState;
        };

	    /**
         * \brief 
         * \param resource 
         * \param before 
         * \param after 
         * \param offset 
         * \param size 
         * \return 
         */
        static GpuBarrier Of(const Buffer* resource, ResourceState before, ResourceState after, std::size_t offset = 0, std::size_t size = -1)
        {
            GpuBarrier barrier{};
            barrier.Type = GpuBarrierType::Buffer;
            barrier.Buffer.Resource = resource;
            barrier.Buffer.OldState = before;
            barrier.Buffer.NewState = after;
            barrier.Buffer.Offset = offset;
            barrier.Buffer.Size = size;

            return barrier;
        }

	    /**
         * \brief 
         * \param resource 
         * \param before 
         * \param after 
         * \param subResource 
         * \return 
         */
        static GpuBarrier Of(const Texture* resource, ResourceState before, ResourceState after, const TextureSubResource& subResource)
        {
            GpuBarrier barrier{};
            barrier.Type = GpuBarrierType::Texture;
            barrier.Texture.SubResource = subResource;
            barrier.Texture.Resource = resource;
            barrier.Texture.OldState = before;
            barrier.Texture.NewState = after;

            return barrier;
        }

	    /**
         * \brief 
         */
        GpuBarrierType Type = GpuBarrierType::Memory;

	    /**
    	 * \brief 
    	 */
    	BufferBarrier Buffer;

	    /**
         * \brief 
         */
        TextureBarrier Texture;

	    /**
         * \brief 
         */
        ResourceQueueTransfer* QueueTransfer = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_COMMAND_GPUBARRIER_HPP
