#ifndef COCKTAIL_RENDERER_RENDERDEVICE_HPP
#define COCKTAIL_RENDERER_RENDERDEVICE_HPP

#include <Cocktail/Renderer/RenderDeviceExtension.hpp>
#include <Cocktail/Renderer/Buffer/Buffer.hpp>
#include <Cocktail/Renderer/Buffer/BufferCreateInfo.hpp>
#include <Cocktail/Renderer/Buffer/BufferView.hpp>
#include <Cocktail/Renderer/Buffer/BufferViewCreateInfo.hpp>
#include <Cocktail/Renderer/Command/Synchronization/Fence.hpp>
#include <Cocktail/Renderer/Command/Synchronization/FenceCreateInfo.hpp>
#include <Cocktail/Renderer/Context/RenderContext.hpp>
#include <Cocktail/Renderer/Context/RenderContextCreateInfo.hpp>
#include <Cocktail/Renderer/Context/RenderSurface.hpp>
#include <Cocktail/Renderer/Context/RenderSurfaceCreateInfo.hpp>
#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgram.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/Sampler.hpp>
#include <Cocktail/Renderer/Texture/SamplerCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/Texture.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/TextureView.hpp>
#include <Cocktail/Renderer/Texture/TextureViewCreateInfo.hpp>

namespace Ck::Renderer
{
    enum class MessageType
    {
	    /**
	     * \brief 
	     */
	    General,

	    /**
    	 * \brief 
    	 */
    	Validation,

	    /**
         * \brief 
         */
        Performance,
    };

	/**
     * \brief 
     */
    class RenderDevice : public Implements<RenderDevice, Interface>
    {
    public:

	    /**
         * \brief 
         * \param createInfo
         * \return 
         */
        virtual Ref<Buffer> CreateBuffer(const BufferCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual Ref<BufferView> CreateBufferView(const BufferViewCreateInfo& createInfo) = 0;

	    /**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		virtual Ref<Fence> CreateFence(const FenceCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \param createInfo
         * \return
         */
        virtual Ref<Framebuffer> CreateFramebuffer(const FramebufferCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual Ref<RenderContext> CreateRenderContext(const RenderContextCreateInfo& createInfo) = 0;

	    /**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		virtual Ref<RenderSurface> CreateRenderSurface(const RenderSurfaceCreateInfo& createInfo) = 0;
    	
	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual Ref<Sampler> CreateSampler(const SamplerCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \param createInfo
         * \return
         */
        virtual Ref<Shader> CreateShader(const ShaderCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual Ref<ShaderProgram> CreateShaderProgram(const ShaderProgramCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual Ref<Texture> CreateTexture(const TextureCreateInfo& createInfo) = 0;
        
	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual Ref<TextureView> CreateTextureView(const TextureViewCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param extension 
         * \return 
         */
        virtual bool IsExtensionSupported(RenderDeviceExtension extension) const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual Signal<MessageType, std::string_view>& OnDebugMessage() = 0;
    };
}

#endif // COCKTAIL_RENDERER_RENDERDEVICE_HPP
