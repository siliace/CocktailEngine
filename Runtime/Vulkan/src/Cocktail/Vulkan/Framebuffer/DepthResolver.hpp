#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP

#include <filesystem>

#include <Cocktail/Core/Meta/Extends.hpp>
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
	class DepthResolver : public Extends<DepthResolver, Object>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit DepthResolver(Ref<RenderDevice> renderDevice);

		/**
		 * \brief 
		 * \param commandList
		 * \param renderPassMode 
		 * \param multisampleAttachment 
		 * \param attachment
		 * \param depthResolveMode 
		 */
		void Resolve(Renderer::CommandList& commandList, Renderer::RenderPassMode renderPassMode, Ref<Renderer::TextureView> multisampleAttachment, Ref<Renderer::TextureView> attachment, Renderer::ResolveMode depthResolveMode);

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
		static Ref<Renderer::Shader> LoadShader(RenderDevice& renderDevice, const std::filesystem::path& path, Renderer::ShaderType shaderType);

		/**
		 * \brief 
		 * \param attachment 
		 * \return 
		 */
		Renderer::Framebuffer* GetOrCreateFramebuffer(const Ref<Renderer::TextureView>& attachment);

		Ref<RenderDevice> mRenderDevice;
		Renderer::UniformSlot* mDepthSamplerSlot;
		Renderer::UniformSlot* mPipelineConstantsSlot;
		Ref<Renderer::ShaderProgram> mShaderProgram;
		CachePool<Ref<Renderer::TextureView>, Ref<Renderer::Framebuffer>> mFramebuffers;
	};
	
}
#endif // COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP
