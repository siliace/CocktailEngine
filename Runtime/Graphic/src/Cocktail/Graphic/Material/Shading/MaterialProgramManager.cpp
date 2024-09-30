#include <Cocktail/Core/Application/App.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramManager.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp>

namespace Ck
{
	MaterialProgramManager::MaterialProgramManager(Ref<Renderer::RenderDevice> renderDevice) :
		mRenderDevice(std::move(renderDevice))
	{
		for (const auto& entry : App::Resolve<MaterialProgramLibrary>()->GetEntries())
		{
			Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
			for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
			{
				if (!Renderer::ProgramSupportShader(Renderer::ShaderProgramType::Graphic, shaderType))
					continue;

				const std::string& binary = entry.Binaries[shaderType];
				if (binary.empty())
					continue;

				Renderer::ShaderCreateInfo shaderCreateInfo;
				shaderCreateInfo.Type = shaderType;
				shaderCreateInfo.CodeLength = binary.size();
				shaderCreateInfo.Code = reinterpret_cast<const Uint32*>(binary.data());
				shaderProgramCreateInfo.Shaders[shaderProgramCreateInfo.ShaderCount++] = mRenderDevice->CreateShader(shaderCreateInfo);
			}

			Ref<Renderer::ShaderProgram> shaderProgram = mRenderDevice->CreateShaderProgram(shaderProgramCreateInfo);
			Register(entry.RenderableType, entry.ShadingMode, MaterialProgram::New(entry.Name, entry.Interface, std::move(shaderProgram)));
		}
	}

	void MaterialProgramManager::Register(RenderableType renderableType, Material::ShadingMode shadingMode, Ref<MaterialProgram> materialProgram)
	{
		Flags<VertexAttributeSemantic> vertexAttributes;
		for (VertexAttributeSemantic vertexAttribute : Enum<VertexAttributeSemantic>::Values)
		{
			if (!materialProgram->GetVertexAttributeLocation(vertexAttribute))
				continue;

			vertexAttributes |= vertexAttribute;
		}

		Flags<Material::TextureType> materialTextures;
		for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
		{
			if (!materialProgram->GetMaterialTextureSlot(textureType))
				continue;

			materialTextures |= textureType;
		}

		MaterialProgramKey materialProgramKey(renderableType, shadingMode, vertexAttributes, materialTextures);
		mMaterialPrograms.insert_or_assign(materialProgramKey, std::move(materialProgram));
	}
}
