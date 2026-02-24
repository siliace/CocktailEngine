#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Material/Material.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgram.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp>

namespace Ck
{
	namespace
	{
		template <typename E>
		unsigned int EvaluateCompatibility(Flags<E> supported, Flags<E> requested)
		{
			unsigned int score = 1;
			for (E value : Enum<E>::Values)
			{
				/// No supported and not requeted, still compatible but does not impact the score
				if (!(requested & value) && !(supported & value))
					continue;

				// Supported but not request, this would lead to an incomplete drawcall, total incompatibility
				if (!(requested & value) && supported & value)
					return 0u;

				// Not supported yet required, don't lead to an incomplete drawcall but does not improve the score
				if (requested & value && !(supported & value))
					continue;

				++score;
			}

			return score;
		}
	}

    BindingSlot MaterialProgram::GetMaterialTextureBindingSlot(Material::TextureType textureType)
    {
	    switch (textureType)
	    {
            case Material::TextureType::BaseColor: return MaterialBindingSlots::BaseColor;
            case Material::TextureType::MetallicRoughness: return MaterialBindingSlots::MetallicRoughness;
            case Material::TextureType::Normal: return MaterialBindingSlots::Normal;
            case Material::TextureType::Alpha: return MaterialBindingSlots::Alpha;
            case Material::TextureType::Emission: return MaterialBindingSlots::Emission;
        }

	    return InvalidBindingSlot;
    }

    MaterialProgram::MaterialProgram(Renderer::RenderDevice* renderDevice, const MaterialProgramCreateInfo& createInfo) :
		mName(createInfo.Name),
        mInterface(createInfo.Interface),
        mShadingMode(createInfo.ShadingMode)
	{
		for (const EnumMap<Renderer::ShaderType, ByteArray>& variantBinaries : createInfo.VariantsBinaries)
		{
			unsigned int shaderCount = 0;
			Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
			shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
			for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
			{
				const ByteArray& binary = variantBinaries[shaderType];
				if (binary.IsEmpty())
					continue;

				Renderer::ShaderCreateInfo shaderCreateInfo;
				shaderCreateInfo.Type = shaderType;
				shaderCreateInfo.Code = binary;
				shaderProgramCreateInfo.Shaders[shaderCount] = renderDevice->CreateShader(shaderCreateInfo);
				++shaderCount;
			}

			assert(shaderCount > 0);
			shaderProgramCreateInfo.ShaderCount = shaderCount;

		    Array<Renderer::StaticSamplerInfo> staticSamplers;
		    for (ShaderBindingDomain domain : Enum<ShaderBindingDomain>::Values)
		    {
                const BindingDomainInterface* bindingDomainInterface = mInterface->GetBindingDomainInterface(domain);
		        if (!bindingDomainInterface)
		            continue;

		        for (unsigned int i = 0; i < bindingDomainInterface->GetSlotInterfaceCount(); i++)
		        {
		            const SlotInterface& slotInterface = bindingDomainInterface->GetSlotInterface(i);
		            if (slotInterface.Type == Renderer::DescriptorType::TextureSampler)
		            {
                        Renderer::StaticSamplerInfo& staticSampler = staticSamplers.Emplace();
		                staticSampler.Member = slotInterface.Name;
		                staticSampler.Sampler = slotInterface.Sampler;
		            }
		        }
		    }

			shaderProgramCreateInfo.StaticSamplerCount = staticSamplers.GetSize();
			shaderProgramCreateInfo.StaticSamplers = staticSamplers.GetData();

			std::shared_ptr<Renderer::ShaderProgram> shaderProgram = renderDevice->CreateShaderProgram(shaderProgramCreateInfo);
			UniquePtr<MaterialProgramVariant> variant = MakeUnique<MaterialProgramVariant>(createInfo.Interface, std::move(shaderProgram));

			Flags<VertexAttributeSemantic> vertexAttributes;
			for (VertexAttributeSemantic attribute : Enum<VertexAttributeSemantic>::Values)
			{
				if (variant->GetVertexAttributeLocation(attribute) == nullptr)
					continue;

				vertexAttributes |= attribute;
			}

			Flags<Material::TextureType> materialTextures;
			for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
			{
			    BindingSlot bindingSlot = GetMaterialTextureBindingSlot(textureType);
				if (bindingSlot == InvalidBindingSlot || variant->GetSlot(ShaderBindingDomain::Material, bindingSlot) == nullptr)
					continue;

				materialTextures |= textureType;
			}

			mVariants.insert_or_assign(VariantKey(vertexAttributes, materialTextures), std::move(variant));
		}
	}

	MaterialProgramVariant* MaterialProgram::GetVariant(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const
	{
		/// First try to find the best program
		for (const auto& [key, materialProgram] : mVariants)
		{
			if (key == VariantKey(vertexAttributes, materialTextures))
				return materialProgram.Get();
		}

		/// Otherwise fallback on one that can just partially support our material
		unsigned int bestScore = 0;
		MaterialProgramVariant* bestVariant = nullptr;
		for (const auto& [key, variant] : mVariants)
		{
			const unsigned int vertexAttributeScore = EvaluateCompatibility(std::get<0>(key), vertexAttributes);
			if (!vertexAttributeScore)
				continue;

			const unsigned int materialTextureScore = EvaluateCompatibility(std::get<1>(key), materialTextures);
			if (!materialTextureScore)
				continue;

			const unsigned int score = vertexAttributeScore + materialTextureScore;
			if (score > bestScore)
			{
				bestScore = score;
				bestVariant = variant.Get();
			}
		}

		return bestVariant;
	}

	const String& MaterialProgram::GetName() const
	{
		return mName;
	}

    Material::ShadingMode MaterialProgram::GetShadingMode() const
    {
	    return mShadingMode;
    }
}
