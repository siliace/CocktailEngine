#ifndef COCKTAILENGINE_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP
#define COCKTAILENGINE_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP

#include <CocktailEngine/Core/System/FileSystem/URI.hpp>

#include <CocktailEngine/Renderer/Command/RenderPassBeginInfo.hpp>
#include <CocktailEngine/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <CocktailEngine/Renderer/Shader/UniformSlot.hpp>

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
		void Resolve(CommandList& commandList, Renderer::RenderPassMode renderPassMode, SharedPtr<TextureView> multisampleAttachment, SharedPtr<TextureView> attachment, Renderer::ResolveMode depthResolveMode);

	private:

		struct ResolveInfo
		{
			unsigned int SampleCount = 0;
			unsigned int DepthResolveMode = 0;
		};

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param uri
		 * \param shaderType 
		 * \return 
		 */
		static SharedPtr<Shader> LoadShader(RenderDevice& renderDevice, const URI& uri, Renderer::ShaderType shaderType);

		/**
		 * \brief 
		 * \param attachment 
		 * \return 
		 */
		Framebuffer* GetOrCreateFramebuffer(SharedPtr<TextureView> attachment);

		RenderDevice* mRenderDevice;
		Renderer::UniformSlot* mDepthSamplerSlot;
		SharedPtr<ShaderProgram> mShaderProgram;
		HashMap<SharedPtr<TextureView>, SharedPtr<Framebuffer>> mFramebuffers;
	};
	
}
#endif // COCKTAILENGINE_VULKAN_FRAMEBUFFER_DEPTHRESOLVER_HPP
