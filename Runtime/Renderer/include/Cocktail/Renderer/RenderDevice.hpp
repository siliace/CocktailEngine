#ifndef COCKTAIL_RENDERER_RENDERDEVICE_HPP
#define COCKTAIL_RENDERER_RENDERDEVICE_HPP

#include <Cocktail/Core/Log/LogLevel.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

#include <Cocktail/Renderer/RenderDeviceExtension.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>

namespace Ck::Renderer
{
    class Buffer;
    struct BufferCreateInfo;
    class BufferView;
    struct BufferViewCreateInfo;
    class Fence;
    struct FenceCreateInfo;
    class Framebuffer;
    struct FramebufferCreateInfo;
    class RenderContext;
    struct RenderContextCreateInfo;
    class RenderSurface;
    struct RenderSurfaceCreateInfo;
    class Sampler;
    struct SamplerCreateInfo;
    class Shader;
    struct ShaderCreateInfo;
    class ShaderProgram;
    struct ShaderProgramCreateInfo;
    class Texture;
    struct TextureCreateInfo;
    class TextureView;
    struct TextureViewCreateInfo;

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
    class RenderDevice
    {
    public:

	    /**
         * \brief 
         */
        virtual ~RenderDevice() = default;

	    /**
         * \brief 
         * \param createInfo
         * \return 
         */
        virtual std::shared_ptr<Buffer> CreateBuffer(const BufferCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual std::shared_ptr<BufferView> CreateBufferView(const BufferViewCreateInfo& createInfo) = 0;

	    /**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		virtual std::shared_ptr<Fence> CreateFence(const FenceCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \param createInfo
         * \return
         */
        virtual std::shared_ptr<Framebuffer> CreateFramebuffer(const FramebufferCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual std::shared_ptr<RenderContext> CreateRenderContext(const RenderContextCreateInfo& createInfo) = 0;

	    /**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		virtual std::shared_ptr<RenderSurface> CreateRenderSurface(const RenderSurfaceCreateInfo& createInfo) = 0;
    	
	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual std::shared_ptr<Sampler> CreateSampler(const SamplerCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \param createInfo
         * \return
         */
        virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual std::shared_ptr<ShaderProgram> CreateShaderProgram(const ShaderProgramCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& createInfo) = 0;
        
	    /**
         * \brief 
         * \param createInfo 
         * \return 
         */
        virtual std::shared_ptr<TextureView> CreateTextureView(const TextureViewCreateInfo& createInfo) = 0;

	    /**
         * \brief 
         * \param extension 
         * \return 
         */
        virtual bool IsExtensionSupported(RenderDeviceExtension extension) const = 0;

	    /**
         * \brief Get texture usage flags supported for a given \p format and a \p MemoryType
         * \param format The PixelFormat to check
         * \param memoryType The MemoryType to check
         * \return The supported texture usage
         */
        virtual TextureUsageFlags GetTextureFormatSupport(const PixelFormat& format, MemoryType memoryType) const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual Signal<LogLevel, MessageType, std::string_view>& OnDebugMessage() = 0;
    };
}

#endif // COCKTAIL_RENDERER_RENDERDEVICE_HPP
