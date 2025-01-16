#include <Cocktail/Core/Application/App.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp>

namespace Ck
{
	MaterialProgramSet::MaterialProgramSet(Ref<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode) :
		mMaterialProgramManager(std::move(materialProgramManager))
	{
		mMaterialProgramManager->SelectMaterialPrograms(shadingMode, [&](RenderableType renderableType, Ref<MaterialProgram> materialProgram) {
			mMaterialPrograms[renderableType] = std::move(materialProgram);
		});
	}

	Ref<MaterialProgram> MaterialProgramSet::GetMaterialProgram(RenderableType renderableType) const
	{
		return mMaterialPrograms[renderableType];
	}
}
