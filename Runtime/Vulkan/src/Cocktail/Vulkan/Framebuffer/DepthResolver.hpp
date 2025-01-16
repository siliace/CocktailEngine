#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP

#include <filesystem>

#include <Cocktail/Renderer/Command/RenderPassBeginInfo.hpp>
#include <Cocktail/Renderer/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

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
		std::unordered_map<Ref<Renderer::TextureView>, Ref<Renderer::Framebuffer>> mFramebuffers;
	};
	
}
#endif // COCKTAIL_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP
