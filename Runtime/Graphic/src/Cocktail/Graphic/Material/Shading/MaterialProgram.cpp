#include <Cocktail/Graphic/Material/Shading/MaterialProgram.hpp>

namespace Ck
{
	MaterialProgram::MaterialProgram(std::string name, const MaterialProgramInterface& interface, Ref<Renderer::ShaderProgram> shaderProgram) :
		mName(std::move(name)),
		mShaderProgram(std::move(shaderProgram))
	{
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

	Renderer::VertexAttributeLocation* MaterialProgram::FindVertexAttributeLocation(Renderer::ShaderType stage, std::string_view name) const
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

	Renderer::UniformSlot* MaterialProgram::FindUniformSlot(std::string_view name) const
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

	Renderer::VertexAttributeLocation* MaterialProgram::GetVertexAttributeLocation(VertexAttributeSemantic semantic) const
	{
		return mVertexAttributeLocations[semantic];
	}

	Renderer::UniformSlot* MaterialProgram::GetMaterialTextureSlot(Material::TextureType textureType) const
	{
		return mTextureUniformSlots[textureType];
	}

	const std::string& MaterialProgram::GetName() const
	{
		return mName;
	}

	const Ref<Renderer::ShaderProgram>& MaterialProgram::GetShaderProgram() const
	{
		return mShaderProgram;
	}
}
