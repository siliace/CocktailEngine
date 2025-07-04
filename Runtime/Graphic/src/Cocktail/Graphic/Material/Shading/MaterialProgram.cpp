#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgram.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp>

#include "Cocktail/Graphic/Material/Material.hpp"

namespace Ck
{
	namespace
	{
		template <typename E>
		unsigned int EvaluateCompatibility(Flags<E> supported, Flags<E> requested)
		{
			unsigned int score = 0;
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

	MaterialProgram::MaterialProgram(const std::shared_ptr<Renderer::RenderDevice>& renderDevice, const MaterialProgramCreateInfo& createInfo) :
		mName(createInfo.Name)
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
				shaderCreateInfo.CodeLength = binary.GetSize();
				shaderCreateInfo.Code = reinterpret_cast<const Uint32*>(binary.GetData());
				shaderProgramCreateInfo.Shaders[shaderCount] = renderDevice->CreateShader(shaderCreateInfo);
				++shaderCount;
			}

			assert(shaderCount > 0);
			shaderProgramCreateInfo.ShaderCount = shaderCount;

			int staticSamplerCount = 0;
			Renderer::StaticSamplerInfo* staticSamplers = COCKTAIL_STACK_ALLOC(Renderer::StaticSamplerInfo, Enum<Material::TextureType>::ValueCount);
			for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
			{
				if (createInfo.Interface.Textures[textureType].Name.empty())
					continue;

				staticSamplers[staticSamplerCount].Member = createInfo.Interface.Textures[textureType].Name;
				staticSamplers[staticSamplerCount].Sampler = createInfo.Interface.Textures[textureType].Sampler;
				++staticSamplerCount;
			}

			shaderProgramCreateInfo.StaticSamplerCount = staticSamplerCount;
			shaderProgramCreateInfo.StaticSamplers = staticSamplers;

			std::shared_ptr<Renderer::ShaderProgram> shaderProgram = renderDevice->CreateShaderProgram(shaderProgramCreateInfo);
			std::shared_ptr<MaterialProgramVariant> variant = std::make_shared<MaterialProgramVariant>(createInfo.Interface, std::move(shaderProgram));

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
				if (variant->GetMaterialTextureSlot(textureType) == nullptr)
					continue;

				materialTextures |= textureType;
			}

			mVariants.insert_or_assign(VariantKey(vertexAttributes, materialTextures), std::move(variant));
		}
	}

	std::shared_ptr<MaterialProgramVariant> MaterialProgram::GetVariant(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const
	{
		/// First try to find the best program
		for (const auto& [key, materialProgram] : mVariants)
		{
			if (key == VariantKey(vertexAttributes, materialTextures))
				return materialProgram;
		}

		/// Otherwise fallback on one that can just partially support our material
		unsigned int bestScore = 0;
		std::shared_ptr<MaterialProgramVariant> bestVariant;
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
				bestVariant = variant;
			}
		}

		return bestVariant;
	}

	const std::string& MaterialProgram::GetName() const
	{
		return mName;
	}
}
