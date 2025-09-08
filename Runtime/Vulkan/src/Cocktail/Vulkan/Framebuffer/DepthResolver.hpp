#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP

#include <Cocktail/Core/System/FileSystem/Path.hpp>

#include <Cocktail/Renderer/Command/RenderPassBeginInfo.hpp>
#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck::Vulkan
{
	class CommandList;
	class Framebuffer;
	class RenderDevice;
	class Shader;
	class ShaderProgram;
	class TextureView;

	/**
	 * \brief 
	 */
	class DepthResolver
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit DepthResolver(RenderDevice* renderDevice);

		/**
		 * \brief 
		 * \param commandList
		 * \param renderPassMode 
		 * \param multisampleAttachment 
		 * \param attachment
		 * \param depthResolveMode 
		 */
		void Resolve(CommandList& commandList, Renderer::RenderPassMode renderPassMode, std::shared_ptr<TextureView> multisampleAttachment, std::shared_ptr<TextureView> attachment, Renderer::ResolveMode depthResolveMode);

	private:

		struct ResolveInfo
		{
			unsigned int SampleCount = 0;
			unsigned int DepthResolveMode = 0;
		};

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param path 
		 * \param shaderType 
		 * \return 
		 */
		static std::shared_ptr<Shader> LoadShader(RenderDevice& renderDevice, const Path& path, Renderer::ShaderType shaderType);

		/**
		 * \brief 
		 * \param attachment 
		 * \return 
		 */
		Framebuffer* GetOrCreateFramebuffer(std::shared_ptr<TextureView> attachment);

		RenderDevice* mRenderDevice;
		Renderer::UniformSlot* mDepthSamplerSlot;
		std::shared_ptr<ShaderProgram> mShaderProgram;
		std::unordered_map<std::shared_ptr<TextureView>, std::shared_ptr<Framebuffer>> mFramebuffers;
	};
	
}
#endif // COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP
