#include <Cocktail/Graphic/Material/Shading/MaterialProgram.hpp>

namespace Ck
{
	MaterialProgram::MaterialProgram(std::string name, Ref<Renderer::ShaderProgram> shaderProgram, Ref<VertexInterface> vertexInterface) :
		mName(std::move(name)),
		mShaderProgram(std::move(shaderProgram)),
		mVertexInterface(std::move(vertexInterface))
	{
		for (VertexAttributeSemantic semantic : Enum<VertexAttributeSemantic>::Values)
			mVertexAttributeLocations[semantic] = mVertexInterface->GetAttributeLocation(*mShaderProgram, semantic);
	}

	Renderer::VertexAttributeLocation* MaterialProgram::GetAttributeLocation(VertexAttributeSemantic semantic) const
	{
		return mVertexAttributeLocations[semantic];
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
