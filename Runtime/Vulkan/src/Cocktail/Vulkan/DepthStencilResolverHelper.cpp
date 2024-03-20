#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Vulkan/DepthStencilResolverHelper.hpp>
#include <Cocktail/Vulkan/RenderDevice.hpp>

namespace Ck::Vulkan
{
	DepthStencilResolverHelper::DepthStencilResolverHelper(Ref<RenderDevice> renderDevice) :
		mRenderDevice(std::move(renderDevice))
	{
		Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
		shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
		shaderProgramCreateInfo.ShaderCount = 2;
		shaderProgramCreateInfo.Shaders[0] = LoadShader(*mRenderDevice, "builtin://vulkan/resources/depth_stencil_resolve.vert.spv", Renderer::ShaderType::Vertex);
		shaderProgramCreateInfo.Shaders[1] = LoadShader(*mRenderDevice, "builtin://vulkan/resources/depth_stencil_resolve.frag.spv", Renderer::ShaderType::Fragment);

		mShaderProgram = mRenderDevice->CreateShaderProgram(shaderProgramCreateInfo);
		mShaderProgram->GetUniformSlots(mUniformSlots, 2, 0);
	}

	void DepthStencilResolverHelper::Resolve(Renderer::CommandList& commandList, Ref<Renderer::TextureView> multisampleAttachment, Ref<Renderer::TextureView> attachment)
	{
		ResolveInfo resolveInfo;
		resolveInfo.SampleCount = 4;
		resolveInfo.DepthResolveMode = 2;
		resolveInfo.StencilResolveMode = 1;

		Renderer::RenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.Mode = Renderer::RenderPassMode::Clear;
		renderPassBeginInfo.Framebuffer = GetOrCreateFramebuffer(attachment);
		renderPassBeginInfo.DepthClearValue = 1.f;

		commandList.BindShaderProgram(mShaderProgram.Get());
		commandList.BindTexture(mUniformSlots[0], 0, multisampleAttachment.Get());
		commandList.UpdatePipelineConstant(mUniformSlots[1], 0, sizeof(ResolveInfo), &resolveInfo);

		commandList.BeginRenderPass(renderPassBeginInfo);
		commandList.Draw(3, 1, 0, 0);
		commandList.EndRenderPass();
	}

	Ref<Renderer::Shader> DepthStencilResolverHelper::LoadShader(RenderDevice& renderDevice, const std::filesystem::path& path, Renderer::ShaderType shaderType)
	{
		std::string shaderCode = FileUtils::ReadFile(path);

		Renderer::ShaderCreateInfo createInfo;
		createInfo.Type = shaderType;
		createInfo.CodeLength = shaderCode.length();
		createInfo.Code = reinterpret_cast<Uint32*>(shaderCode.data());

		return renderDevice.CreateShader(createInfo);
	}

	Renderer::Framebuffer* DepthStencilResolverHelper::GetOrCreateFramebuffer(const Ref<Renderer::TextureView>& attachment)
	{
		return mFramebuffers.GetOrCreate(attachment, [&]() {
			Renderer::FramebufferCreateInfo createInfo;
			createInfo.DepthStencilAttachment = attachment;

			return mRenderDevice->CreateFramebuffer(createInfo);
		}).Get();
	}
}
