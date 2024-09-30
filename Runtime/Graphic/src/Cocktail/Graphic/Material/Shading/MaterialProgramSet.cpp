#include <Cocktail/Core/Application/App.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp>

namespace Ck
{
	MaterialProgramSet::MaterialProgramSet(Ref<MaterialProgramManager> materialProgramManager, RenderableType renderableType, Material::ShadingMode shadingMode) :
		mMaterialProgramManager(std::move(materialProgramManager))
	{
		mMaterialProgramManager->SelectMaterialPrograms(renderableType, shadingMode, [this](auto attributes, auto textures, auto materialProgram) {
			Register(attributes, textures, std::move(materialProgram));
		});
	}

	void MaterialProgramSet::Register(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures, Ref<MaterialProgram> materialProgram)
	{
		mMaterialPrograms.insert_or_assign(MaterialProgramKey(vertexAttributes, materialTextures), std::move(materialProgram));
	}

	Ref<MaterialProgram> MaterialProgramSet::FindMaterialProgram(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const
	{
		/// First try to find the best program
		for (const auto& [key, materialProgram] : mMaterialPrograms)
		{
			if (key == MaterialProgramKey(vertexAttributes, materialTextures))
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
		Ref<MaterialProgram> bestProgram;
		for (const auto& [key, materialProgram] : mMaterialPrograms)
		{
			const unsigned int vertexAttributeScore = computeVertexScore(std::get<0>(key), vertexAttributes);
			const unsigned int materialTextureScore = computeMaterialTextureScore(std::get<1>(key), materialTextures);
			const unsigned int score = vertexAttributeScore + materialTextureScore;
			if (score > bestScore)
			{
				bestScore = score;
				bestProgram = materialProgram;
			}
		}

		return bestProgram;
	}
}
