#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>

namespace Ck::Vulkan
{
	DepthResolver::DepthResolver(Ref<RenderDevice> renderDevice) :
		mRenderDevice(std::move(renderDevice)),
		mDepthSamplerSlot(nullptr)
	{
		Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
		shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
		shaderProgramCreateInfo.ShaderCount = 2;
		shaderProgramCreateInfo.Shaders[0] = LoadShader(*mRenderDevice, "builtin://vulkan/resources/depth_resolve.vert.spv", Renderer::ShaderType::Vertex);
		shaderProgramCreateInfo.Shaders[1] = LoadShader(*mRenderDevice, "builtin://vulkan/resources/depth_resolve.frag.spv", Renderer::ShaderType::Fragment);

		mShaderProgram = mRenderDevice->CreateShaderProgram(shaderProgramCreateInfo);

		mDepthSamplerSlot = mShaderProgram->FindUniformSlot("inTexture");
		assert(mDepthSamplerSlot);
	}

	void DepthResolver::Resolve(Renderer::CommandList& commandList, Renderer::RenderPassMode renderPassMode, Ref<Renderer::TextureView> multisampleAttachment, Ref<Renderer::TextureView> attachment, Renderer::ResolveMode depthResolveMode)
	{
		Renderer::RenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.Mode = renderPassMode;
		renderPassBeginInfo.Framebuffer = GetOrCreateFramebuffer(attachment);
		renderPassBeginInfo.DepthClearValue = 1.f;

		commandList.BeginRenderPass(renderPassBeginInfo);

		commandList.BindShaderProgram(mShaderProgram.Get());
		commandList.BindTexture(mDepthSamplerSlot, 0, multisampleAttachment.Get());

		Ref<AbstractTexture> multisampleTexture = AbstractTexture::Cast(multisampleAttachment->GetTexture());
		assert(multisampleTexture->GetSamples() != Renderer::RasterizationSamples::e1);

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
	}

	Ref<Renderer::Shader> DepthResolver::LoadShader(RenderDevice& renderDevice, const std::filesystem::path& path, Renderer::ShaderType shaderType)
	{
		ByteArray shaderCode = FileUtils::ReadFile(path);

		Renderer::ShaderCreateInfo createInfo;
		createInfo.Type = shaderType;
		createInfo.CodeLength = shaderCode.GetSize();
		createInfo.Code = reinterpret_cast<const Uint32*>(shaderCode.GetData());

		return renderDevice.CreateShader(createInfo);
	}

	Renderer::Framebuffer* DepthResolver::GetOrCreateFramebuffer(const Ref<Renderer::TextureView>& attachment)
	{
		if (auto it = mFramebuffers.find(attachment); it != mFramebuffers.end())
			return it->second.Get();

		Renderer::FramebufferCreateInfo createInfo;
		createInfo.DepthStencilAttachment = attachment;
		Ref<Renderer::Framebuffer> framebuffer = mRenderDevice->CreateFramebuffer(createInfo);
		mFramebuffers[attachment] = framebuffer;

		return framebuffer.Get();
	}
}
