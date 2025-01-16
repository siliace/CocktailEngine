#include <Cocktail/Graphic/Material/Shading/MaterialProgram.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp>

namespace Ck
{
	MaterialProgram::MaterialProgram(const Ref<Renderer::RenderDevice>& renderDevice, const MaterialProgramCreateInfo& createInfo) :
		mName(createInfo.Name)
	{
		for (const EnumMap<Renderer::ShaderType, std::string>& variantBinaries : createInfo.VariantsBinaries)
		{
			unsigned int shaderCount = 0;
			Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
			shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
			for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
			{
				const std::string& binary = variantBinaries[shaderType];
				if (binary.empty())
					continue;

				Renderer::ShaderCreateInfo shaderCreateInfo;
				shaderCreateInfo.Type = shaderType;
				shaderCreateInfo.Code = reinterpret_cast<const Uint32*>(binary.data());
				shaderCreateInfo.CodeLength = binary.length();
				shaderProgramCreateInfo.Shaders[shaderCount] = renderDevice->CreateShader(shaderCreateInfo);
				++shaderCount;
			}

			assert(shaderCount > 0);
			shaderProgramCreateInfo.ShaderCount = shaderCount;

			Ref<Renderer::ShaderProgram> shaderProgram = renderDevice->CreateShaderProgram(shaderProgramCreateInfo);
			Ref<MaterialProgramVariant> variant = MaterialProgramVariant::New(createInfo.Interface, std::move(shaderProgram));

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

			VariantKey key(vertexAttributes, materialTextures);
			mVariants[key] = std::move(variant);
		}
	}

	Ref<MaterialProgramVariant> MaterialProgram::GetVariant(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const
	{
		/// First try to find the best program
		for (const auto& [key, materialProgram] : mVariants)
		{
			if (key == VariantKey(vertexAttributes, materialTextures))
				return materialProgram;
		}

		auto computeVertexScore = [](Flags<VertexAttributeSemantic> supportedVertexAttribute, Flags<VertexAttributeSemantic> requestedVertexAttributes) {
			unsigned int score = 0;
			for (VertexAttributeSemantic vertexAttribute : Enum<VertexAttributeSemantic>::Values)
			{
				if (!(requestedVertexAttributes & vertexAttribute))
					continue;

				if (!(supportedVertexAttribute & vertexAttribute))
					continue;

				++score;
			}

			return score;
		};

		auto computeMaterialTextureScore = [](Flags<Material::TextureType> supportedTextureMaterials, Flags<Material::TextureType> requestedTextureMaterials) {
			unsigned int score = 0;
			for (Material::TextureType materialTexture : Enum<Material::TextureType>::Values)
			{
				if (!(requestedTextureMaterials & materialTexture))
					continue;

				if (!(supportedTextureMaterials & materialTexture))
					continue;

				++score;
			}

			return score;
		};

		/// Otherwise fallback on one that can just partially support our material
		unsigned int bestScore = 0;
		Ref<MaterialProgramVariant> bestVariant;
		for (const auto& [key, variant] : mVariants)
		{
			const unsigned int vertexAttributeScore = computeVertexScore(std::get<0>(key), vertexAttributes);
			const unsigned int materialTextureScore = computeMaterialTextureScore(std::get<1>(key), materialTextures);
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
