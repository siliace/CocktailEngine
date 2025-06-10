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
     * \brief Interface representing a GPU
     * Its main use is to create resources that can be used by CommandList to perform rendering operations.
	 * This class is thread safe and can be call from any thread.
     */
    class RenderDevice
    {
    public:

	    /**
         * \brief Default destructor
         */
        virtual ~RenderDevice() = default;

	    /**
         * \brief  
         * \brief Create a new Buffer
         * \param createInfo Parameters to use to create the Buffer
         * \return The created Buffer
         */
        virtual std::shared_ptr<Buffer> CreateBuffer(const BufferCreateInfo& createInfo) = 0;

    	/**
         * \brief
         * \brief Create a new BufferView
         * \param createInfo Parameters to use to create the BufferView
         * \return The created BufferView
         */
        virtual std::shared_ptr<BufferView> CreateBufferView(const BufferViewCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \brief Create a new Fence
         * \param createInfo Parameters to use to create the Fence
         * \return The created Fence
         */
		virtual std::shared_ptr<Fence> CreateFence(const FenceCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \brief Create a new Framebuffer
         * \param createInfo Parameters to use to create the Framebuffer
         * \return The created Framebuffer
         */
        virtual std::shared_ptr<Framebuffer> CreateFramebuffer(const FramebufferCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \brief Create a new RenderContext
         * \param createInfo Parameters to use to create the RenderContext
         * \return The created RenderContext
         */
        virtual std::shared_ptr<RenderContext> CreateRenderContext(const RenderContextCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \brief Create a new RenderSurface
         * \param createInfo Parameters to use to create the RenderSurface
         * \return The created RenderSurface
         */
		virtual std::shared_ptr<RenderSurface> CreateRenderSurface(const RenderSurfaceCreateInfo& createInfo) = 0;
    	
        /**
         * \brief
         * \brief Create a new Sampler
         * \param createInfo Parameters to use to create the Sampler
         * \return The created Sampler
         */
        virtual std::shared_ptr<Sampler> CreateSampler(const SamplerCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \brief Create a new Shader
         * \param createInfo Parameters to use to create the Shader
         * \return The created Shader
         */
        virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \brief Create a new ShaderProgram
         * \param createInfo Parameters to use to create the ShaderProgram
         * \return The created ShaderProgram
         */
        virtual std::shared_ptr<ShaderProgram> CreateShaderProgram(const ShaderProgramCreateInfo& createInfo) = 0;

        /**
         * \brief
         * \brief Create a new Texture
         * \param createInfo Parameters to use to create the Texture
         * \return The created Texture
         */
        virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& createInfo) = 0;
        
        /**
         * \brief
         * \brief Create a new TextureView
         * \param createInfo Parameters to use to create the TextureView
         * \return The created TextureView
         */
        virtual std::shared_ptr<TextureView> CreateTextureView(const TextureViewCreateInfo& createInfo) = 0;

	    /**
         * \brief Tell whether an extension is supported
         * Support of extensions strongly rely on the graphic driver
         * \param extension The extension to check
         * \return True if the extension is supported, false otherwise
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
         * \brief Get the signal emitted when the underlying api get a debug message from the driver
         * \return The signal
         */
        virtual Signal<LogLevel, MessageType, std::string_view>& OnDebugMessage() = 0;
    };
}

#endif // COCKTAIL_RENDERER_RENDERDEVICE_HPP
