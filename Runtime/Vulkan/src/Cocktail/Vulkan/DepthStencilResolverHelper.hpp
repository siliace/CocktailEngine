#ifndef COCKTAIL_VULKAN_DEPTHSTENCILRESOLVERHELPER_HPP
#define COCKTAIL_VULKAN_DEPTHSTENCILRESOLVERHELPER_HPP

#include <filesystem>

#include <Cocktail/Core/Utility/Cache/CachePool.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class DepthStencilResolverHelper
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit DepthStencilResolverHelper(Ref<RenderDevice> renderDevice);

		/**
		 * \brief 
		 * \param commandList 
		 * \param multisampleAttachment 
		 * \param attachment 
		 */
		void Resolve(Renderer::CommandList& commandList, Ref<Renderer::TextureView> multisampleAttachment, Ref<Renderer::TextureView> attachment);

	private:

		struct ResolveInfo
		{
			unsigned int SampleCount = 0;
			unsigned int DepthResolveMode = 0;
			unsigned int StencilResolveMode = 0;
		};

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param path 
		 * \param shaderType 
		 * \return 
		 */
		static Ref<Renderer::Shader> LoadShader(RenderDevice& renderDevice, const std::filesystem::path& path, Renderer::ShaderType shaderType);

		/**
		 * \brief 
		 * \param attachment 
		 * \return 
		 */
		Renderer::Framebuffer* GetOrCreateFramebuffer(const Ref<Renderer::TextureView>& attachment);

		Ref<RenderDevice> mRenderDevice;
		Renderer::UniformSlot* mUniformSlots[2];
		Ref<Renderer::ShaderProgram> mShaderProgram;
		CachePool<Ref<Renderer::TextureView>, Ref<Renderer::Framebuffer>> mFramebuffers;
	};
	
}
#endif // COCKTAIL_VULKAN_DEPTHSTENCILRESOLVERHELPER_HPP
