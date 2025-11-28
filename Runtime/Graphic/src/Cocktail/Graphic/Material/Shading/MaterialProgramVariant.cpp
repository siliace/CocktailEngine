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
			const AsciiString& vertexAttributeName = interface.VertexAttributes[vertexAttribute];
			if (vertexAttributeName.IsEmpty())
				continue;

			mVertexAttributeLocations[vertexAttribute] = mShaderProgram->GetStage(Renderer::ShaderType::Vertex)->FindInputAttribute(vertexAttributeName);
		}

		for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
		{
			const AsciiString& textureSlotName = interface.Textures[textureType].Name;
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

    Flags<VertexAttributeSemantic> MaterialProgramVariant::GetVertexAttributeUsage() const
	{
	    Flags<VertexAttributeSemantic> flags;
	    for (VertexAttributeSemantic vertexAttribute : Enum<VertexAttributeSemantic>::Values)
	    {
	        if (mVertexAttributeLocations[vertexAttribute])
	            flags |= vertexAttribute;
	    }

	    return flags;
	}

    Flags<Material::TextureType> MaterialProgramVariant::GetMaterialTextureUsage() const
	{
	    Flags<Material::TextureType> flags;
	    for (Material::TextureType texture : Enum<Material::TextureType>::Values)
	    {
	        if (mTextureUniformSlots[texture])
	            flags |= texture;
	    }

	    return flags;
    }
}
