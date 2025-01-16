#include <Cocktail/Core/Application/App.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramManager.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck
{
	MaterialProgramManager::MaterialProgramManager(Ref<Renderer::RenderDevice> renderDevice) :
		mRenderDevice(std::move(renderDevice))
	{
		for (const MaterialProgramLibrary::Entry& entry : App::Resolve<MaterialProgramLibrary>()->GetEntries())
		{
			MaterialProgramCreateInfo materialProgramCreateInfo;
			materialProgramCreateInfo.Name = entry.Name;
			materialProgramCreateInfo.Interface = entry.Interface;
			materialProgramCreateInfo.VariantsBinaries = entry.VariantsBinaries;

			Register(entry.RenderableType, entry.ShadingMode, MaterialProgram::New(mRenderDevice, materialProgramCreateInfo));
		}
	}

	void MaterialProgramManager::Register(RenderableType renderableType, Material::ShadingMode shadingMode, Ref<MaterialProgram> materialProgram)
	{
		MaterialProgramKey key(renderableType, shadingMode);
		mMaterialPrograms[key] = std::move(materialProgram);
	}
}
