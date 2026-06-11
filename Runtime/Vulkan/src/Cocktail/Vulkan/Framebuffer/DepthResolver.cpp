#include <Cocktail/Core/Utility/StorageUtils.hpp>

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
		mRenderDevice(renderDevice),
		mDepthSamplerSlot(nullptr)
	{
		Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
		shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
		shaderProgramCreateInfo.ShaderCount = 2;
		shaderProgramCreateInfo.Shaders[0] = LoadShader(*mRenderDevice, CK_TEXT("builtin://vulkan/resources/depth_resolve.vert.spv"), Renderer::ShaderType::Vertex);
		shaderProgramCreateInfo.Shaders[1] = LoadShader(*mRenderDevice, CK_TEXT("builtin://vulkan/resources/depth_resolve.frag.spv"), Renderer::ShaderType::Fragment);

		mShaderProgram = mRenderDevice->CreateShaderProgram(shaderProgramCreateInfo).StaticCast<ShaderProgram>();

		mDepthSamplerSlot = mShaderProgram->FindUniformSlot("inTexture");
		assert(mDepthSamplerSlot);
	}

	void DepthResolver::Resolve(CommandList& commandList, Renderer::RenderPassMode renderPassMode, SharedPtr<TextureView> multisampleAttachment, SharedPtr<TextureView> attachment, Renderer::ResolveMode depthResolveMode)
	{
		Renderer::RenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.Mode = renderPassMode;
		renderPassBeginInfo.Framebuffer = GetOrCreateFramebuffer(attachment);
		renderPassBeginInfo.DepthClearValue = 1.f;

		SharedPtr<AbstractTexture> multisampleTexture = multisampleAttachment->GetTexture().StaticCast<AbstractTexture>();
		assert(multisampleTexture->GetSamples() != Renderer::RasterizationSamples::e1);

		Renderer::GpuBarrier preBarriers[] = {
			Renderer::GpuBarrier::Of(multisampleTexture.Get(), Renderer::ResourceState::FramebufferAttachment, Renderer::ResourceState::GraphicShaderResource, Renderer::TextureSubResource::All(*multisampleTexture)),
		};
		commandList.Barrier(1, preBarriers);

		commandList.BeginRenderPass(renderPassBeginInfo);

		commandList.BindShaderProgram(mShaderProgram.Get());
		commandList.BindTexture(mDepthSamplerSlot, 0, multisampleAttachment.Get());

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
			Renderer::GpuBarrier::Of(multisampleTexture.Get(), Renderer::ResourceState::GraphicShaderResource, Renderer::ResourceState::FramebufferAttachment, Renderer::TextureSubResource::All(*multisampleTexture)),
		};
		commandList.Barrier(1, postBarriers);
	}

	SharedPtr<Shader> DepthResolver::LoadShader(RenderDevice& renderDevice, const URI& uri, Renderer::ShaderType shaderType)
	{
		ByteArray shaderCode = StorageUtils::ReadFile(uri);

		Renderer::ShaderCreateInfo createInfo;
		createInfo.Type = shaderType;
		createInfo.Code = shaderCode;

		return renderDevice.CreateShader(createInfo).StaticCast<Shader>();
	}

	Framebuffer* DepthResolver::GetOrCreateFramebuffer(SharedPtr<TextureView> attachment)
	{
	    return mFramebuffers.ComputeIfMissing(std::move(attachment), [this](const SharedPtr<TextureView>& attachment) {
	        Renderer::FramebufferCreateInfo createInfo;
            createInfo.DepthStencilAttachment = attachment;
            return mRenderDevice->CreateFramebuffer(createInfo).StaticCast<Framebuffer>();
	    }).Get();
	}
}
