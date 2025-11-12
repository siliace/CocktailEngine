#include <Cocktail/Graphic/Material/Shading/MaterialProgramVariant.hpp>

#include <Cocktail/Renderer/Shader/Shader.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
	MaterialProgramVariant::MaterialProgramVariant(const MaterialProgramInterface& interface, std::shared_ptr<Renderer::ShaderProgram> shaderProgram) :
		mShaderProgram(std::move(shaderProgram))
	{
		assert(mShaderProgram);

		for (VertexAttributeSemantic vertexAttribute : Enum<VertexAttributeSemantic>::Values)
		{
			const AnsiString& vertexAttributeName = interface.VertexAttributes[vertexAttribute];
			if (vertexAttributeName.IsEmpty())
				continue;

			mVertexAttributeLocations[vertexAttribute] = mShaderProgram->GetStage(Renderer::ShaderType::Vertex)->FindInputAttribute(vertexAttributeName);
		}

		for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
		{
			const AnsiString& textureSlotName = interface.Textures[textureType].Name;
			if (textureSlotName.IsEmpty())
				continue;

			mTextureUniformSlots[textureType] = mShaderProgram->FindUniformSlot(textureSlotName);
		}
	}

	Renderer::VertexAttributeLocation* MaterialProgramVariant::GetVertexAttributeLocation(VertexAttributeSemantic semantic) const
	{
		return mVertexAttributeLocations[semantic];
	}

	Renderer::UniformSlot* MaterialProgramVariant::GetMaterialTextureSlot(Material::TextureType textureType) const
	{
		return mTextureUniformSlots[textureType];
	}

	const std::shared_ptr<Renderer::ShaderProgram>& MaterialProgramVariant::GetShaderProgram() const
	{
		return mShaderProgram;
	}
}
