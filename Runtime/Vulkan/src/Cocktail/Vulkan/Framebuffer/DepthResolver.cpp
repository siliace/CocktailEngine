#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>
#include <Cocktail/Vulkan/RenderDevice.hpp>

namespace Ck::Vulkan
{
	class ShaderProgramUtils
	{
	public:

		static Renderer::UniformSlot* FindUniformSlot(const Renderer::ShaderProgram& shaderProgram, Renderer::DescriptorType descriptorType, std::string_view name)
		{
			Renderer::UniformSlot* slot;
			for (unsigned int i = 0; i < shaderProgram.GetUniformSlotCount(); i++)
			{
				shaderProgram.GetUniformSlots(&slot, 1, i);
				if (slot->GetName() == name && slot->GetDescriptorType() == descriptorType)
					return slot;
			}

			return nullptr;
		}
	};

	DepthResolver::DepthResolver(Ref<RenderDevice> renderDevice) :
		mRenderDevice(std::move(renderDevice)),
		mDepthSamplerSlot(nullptr),
		mPipelineConstantsSlot(nullptr)
	{
		Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
		shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
		shaderProgramCreateInfo.ShaderCount = 2;
		shaderProgramCreateInfo.Shaders[0] = LoadShader(*mRenderDevice, "builtin://vulkan/resources/depth_resolve.vert.spv", Renderer::ShaderType::Vertex);
		shaderProgramCreateInfo.Shaders[1] = LoadShader(*mRenderDevice, "builtin://vulkan/resources/depth_resolve.frag.spv", Renderer::ShaderType::Fragment);

		mShaderProgram = mRenderDevice->CreateShaderProgram(shaderProgramCreateInfo);

		mDepthSamplerSlot = ShaderProgramUtils::FindUniformSlot(*mShaderProgram, Renderer::DescriptorType::Texture, "inTexture");
		mPipelineConstantsSlot = ShaderProgramUtils::FindUniformSlot(*mShaderProgram, Renderer::DescriptorType::PipelineConstants, "FramgentStageInfo");

		assert(mPipelineConstantsSlot && mDepthSamplerSlot);
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

		commandList.UpdatePipelineConstant(mPipelineConstantsSlot, 0, sizeof(ResolveInfo), &resolveInfo);

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
		std::string shaderCode = FileUtils::ReadFile(path);

		Renderer::ShaderCreateInfo createInfo;
		createInfo.Type = shaderType;
		createInfo.CodeLength = shaderCode.length();
		createInfo.Code = reinterpret_cast<Uint32*>(shaderCode.data());

		return renderDevice.CreateShader(createInfo);
	}

	Renderer::Framebuffer* DepthResolver::GetOrCreateFramebuffer(const Ref<Renderer::TextureView>& attachment)
	{
		return mFramebuffers.GetOrCreate(attachment, [&]() {
			Renderer::FramebufferCreateInfo createInfo;
			createInfo.DepthStencilAttachment = attachment;

			return mRenderDevice->CreateFramebuffer(createInfo);
		}).Get();
	}
}
