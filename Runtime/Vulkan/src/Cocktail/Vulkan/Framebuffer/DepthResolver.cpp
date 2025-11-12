#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>
#include <Cocktail/Vulkan/Shader/ShaderProgram.hpp>
#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	DepthResolver::DepthResolver(RenderDevice* renderDevice) :
		mRenderDevice(std::move(renderDevice)),
		mDepthSamplerSlot(nullptr)
	{
		Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
		shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
		shaderProgramCreateInfo.ShaderCount = 2;
		shaderProgramCreateInfo.Shaders[0] = LoadShader(*mRenderDevice, CK_TEXT("builtin://vulkan/resources/depth_resolve.vert.spv"), Renderer::ShaderType::Vertex);
		shaderProgramCreateInfo.Shaders[1] = LoadShader(*mRenderDevice, CK_TEXT("builtin://vulkan/resources/depth_resolve.frag.spv"), Renderer::ShaderType::Fragment);

		mShaderProgram = std::static_pointer_cast<ShaderProgram>(mRenderDevice->CreateShaderProgram(shaderProgramCreateInfo));

		mDepthSamplerSlot = mShaderProgram->FindUniformSlot("inTexture");
		assert(mDepthSamplerSlot);
	}

	void DepthResolver::Resolve(CommandList& commandList, Renderer::RenderPassMode renderPassMode, std::shared_ptr<TextureView> multisampleAttachment, std::shared_ptr<TextureView> attachment, Renderer::ResolveMode depthResolveMode)
	{
		Renderer::RenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.Mode = renderPassMode;
		renderPassBeginInfo.Framebuffer = GetOrCreateFramebuffer(attachment);
		renderPassBeginInfo.DepthClearValue = 1.f;

		std::shared_ptr<AbstractTexture> multisampleTexture = std::static_pointer_cast<AbstractTexture>(multisampleAttachment->GetTexture());
		assert(multisampleTexture->GetSamples() != Renderer::RasterizationSamples::e1);

		Renderer::GpuBarrier preBarriers[] = {
			Renderer::GpuBarrier::Of(multisampleTexture.get(), Renderer::ResourceState::FramebufferAttachment, Renderer::ResourceState::GraphicShaderResource, Renderer::TextureSubResource::All(*multisampleTexture)),
		};
		commandList.Barrier(1, preBarriers);

		commandList.BeginRenderPass(renderPassBeginInfo);

		commandList.BindShaderProgram(mShaderProgram.get());
		commandList.BindTexture(mDepthSamplerSlot, 0, multisampleAttachment.get());

		ResolveInfo resolveInfo;
		resolveInfo.SampleCount = static_cast<unsigned int>(multisampleTexture->GetSamples());
		resolveInfo.DepthResolveMode = static_cast<unsigned int>(depthResolveMode);

		commandList.UpdatePipelineConstant(Renderer::ShaderType::Fragment, 0, sizeof(ResolveInfo), &resolveInfo);

		commandList.EnableVertexBinding(0, false);
		commandList.SetCullMode(Renderer::CullMode::None);
		commandList.EnableDepthTest(true);
		commandList.SetDepthCompareOp(Renderer::CompareOp::Always);
		commandList.EnableDepthWrite(true);

		commandList.Draw(3, 1, 0, 0);

		commandList.EndRenderPass();

		Renderer::GpuBarrier postBarriers[] = {
			Renderer::GpuBarrier::Of(multisampleTexture.get(), Renderer::ResourceState::GraphicShaderResource, Renderer::ResourceState::FramebufferAttachment, Renderer::TextureSubResource::All(*multisampleTexture)),
		};
		commandList.Barrier(1, postBarriers);
	}

	std::shared_ptr<Shader> DepthResolver::LoadShader(RenderDevice& renderDevice, const Path& path, Renderer::ShaderType shaderType)
	{
		ByteArray shaderCode = FileUtils::ReadFile(path);

		Renderer::ShaderCreateInfo createInfo;
		createInfo.Type = shaderType;
		createInfo.Code = shaderCode;

		return std::static_pointer_cast<Shader>(renderDevice.CreateShader(createInfo));
	}

	Framebuffer* DepthResolver::GetOrCreateFramebuffer(std::shared_ptr<TextureView> attachment)
	{
		if (auto it = mFramebuffers.find(attachment); it != mFramebuffers.end())
			return it->second.get();

		Renderer::FramebufferCreateInfo createInfo;
		createInfo.DepthStencilAttachment = attachment;
		std::shared_ptr<Framebuffer> framebuffer = std::static_pointer_cast<Framebuffer>(mRenderDevice->CreateFramebuffer(createInfo));
		mFramebuffers[attachment] = framebuffer;

		return framebuffer.get();
	}
}
