#include <Cocktail/Core/Application/App.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp>

namespace Ck
{
	MaterialProgramSet::MaterialProgramSet(std::shared_ptr<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode) :
		mMaterialProgramManager(std::move(materialProgramManager))
	{
		mMaterialProgramManager->SelectMaterialPrograms(shadingMode, [&](RenderableType renderableType, std::shared_ptr<MaterialProgram> materialProgram) {
			mMaterialPrograms[renderableType] = std::move(materialProgram);
		});
	}

	std::shared_ptr<MaterialProgram> MaterialProgramSet::GetMaterialProgram(RenderableType renderableType) const
	{
		return mMaterialPrograms[renderableType];
	}
}
