#include <Cocktail/Graphic/Material/Shading/MaterialProgramVariant.hpp>

#include <Cocktail/Renderer/Shader/Shader.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
	MaterialProgramVariant::MaterialProgramVariant(const MaterialProgramInterface& interface, Ref<Renderer::ShaderProgram> shaderProgram) :
		mShaderProgram(std::move(shaderProgram))
	{
		assert(mShaderProgram);

		for (VertexAttributeSemantic vertexAttribute : Enum<VertexAttributeSemantic>::Values)
		{
			const std::string& vertexAttributeName = interface.VertexAttributes[vertexAttribute];
			if (vertexAttributeName.empty())
				continue;

			mVertexAttributeLocations[vertexAttribute] = FindVertexAttributeLocation(Renderer::ShaderType::Vertex, vertexAttributeName);
		}

		for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
		{
			const std::string& textureSlotName = interface.Textures[textureType];
			if (textureSlotName.empty())
				continue;

			mTextureUniformSlots[textureType] = FindUniformSlot(textureSlotName);
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

	const Ref<Renderer::ShaderProgram>& MaterialProgramVariant::GetShaderProgram() const
	{
		return mShaderProgram;
	}

	Renderer::VertexAttributeLocation* MaterialProgramVariant::FindVertexAttributeLocation(Renderer::ShaderType stage, std::string_view name) const
	{
		Ref<Renderer::Shader> vertexStage = mShaderProgram->GetStage(stage);
		Renderer::VertexAttributeLocation* vertexAttributeLocation;
		for (unsigned int i = 0; i < vertexStage->GetInputAttributeCount(); i++)
		{
			vertexStage->GetInputAttributes(&vertexAttributeLocation, 1, i);
			if (vertexAttributeLocation->GetName() == name)
				return vertexAttributeLocation;
		}

		return nullptr;
	}

	Renderer::UniformSlot* MaterialProgramVariant::FindUniformSlot(std::string_view name) const
	{
		Renderer::UniformSlot* slot;
		for (unsigned int i = 0; i < mShaderProgram->GetUniformSlotCount(); i++)
		{
			mShaderProgram->GetUniformSlots(&slot, 1, i);
			if (slot->GetName() == name)
				return slot;
		}

		return nullptr;
	}
}
