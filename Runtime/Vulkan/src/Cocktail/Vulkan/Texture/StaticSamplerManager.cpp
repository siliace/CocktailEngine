#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Texture/StaticSamplerManager.hpp>

namespace Ck::Vulkan
{
	StaticSamplerManager::StaticSamplerManager(RenderDevice* renderDevice) :
		mRenderDevice(renderDevice)
	{
		for (Renderer::StaticSampler staticSampler : Enum<Renderer::StaticSampler>::Values)
		{
			Renderer::SamplerCreateInfo samplerCreateInfo;
			samplerCreateInfo.EnableAnisotropy = true;
			samplerCreateInfo.MaxAnisotropy = 8.f;
			samplerCreateInfo.Name = Enum<Renderer::StaticSampler>::ToString(staticSampler);

			switch (staticSampler)
			{
			case Renderer::StaticSampler::TrilinearClamp:
			case Renderer::StaticSampler::TrilinearWrap:
				samplerCreateInfo.MipmapMode = Renderer::SamplerMipmapMode::Linear;
				break;

			default:
				samplerCreateInfo.MipmapMode = Renderer::SamplerMipmapMode::Nearest;
				break;
			}

			switch (staticSampler)
			{
			case Renderer::StaticSampler::LinearClamp:
			case Renderer::StaticSampler::LinearWrap:
			case Renderer::StaticSampler::TrilinearClamp:
			case Renderer::StaticSampler::TrilinearWrap:
				samplerCreateInfo.MinFilter = Renderer::Filter::Linear;
				samplerCreateInfo.MagFilter = Renderer::Filter::Linear;
				break;

			default:
				samplerCreateInfo.MinFilter = Renderer::Filter::Nearest;
				samplerCreateInfo.MagFilter = Renderer::Filter::Nearest;
				break;
			}

			switch (staticSampler)
			{
			case Renderer::StaticSampler::LinearWrap:
			case Renderer::StaticSampler::NearestWrap:
			case Renderer::StaticSampler::TrilinearWrap:
				samplerCreateInfo.AddressModeU = Renderer::SamplerAddressMode::Repeat;
				samplerCreateInfo.AddressModeV = Renderer::SamplerAddressMode::Repeat;
				samplerCreateInfo.AddressModeW = Renderer::SamplerAddressMode::Repeat;
				break;

			case Renderer::StaticSampler::LinearClamp:
			case Renderer::StaticSampler::NearestClamp:
			case Renderer::StaticSampler::TrilinearClamp:
				samplerCreateInfo.AddressModeU = Renderer::SamplerAddressMode::ClampToEdge;
				samplerCreateInfo.AddressModeV = Renderer::SamplerAddressMode::ClampToEdge;
				samplerCreateInfo.AddressModeW = Renderer::SamplerAddressMode::ClampToEdge;
				break;
			}

			mSamplers[staticSampler] = std::static_pointer_cast<Sampler>(mRenderDevice->CreateSampler(samplerCreateInfo));
		}
	}

	Sampler* StaticSamplerManager::GetSampler(Renderer::StaticSampler staticSampler) const
	{
		return mSamplers[staticSampler].get();
	}
}
