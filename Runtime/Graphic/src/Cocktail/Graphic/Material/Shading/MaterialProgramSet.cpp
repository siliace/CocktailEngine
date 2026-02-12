#include <Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp>

namespace Ck
{
	MaterialProgramSet::MaterialProgramSet(std::shared_ptr<MaterialProgramManager> materialProgramManager) :
		mMaterialProgramManager(std::move(materialProgramManager))
	{
		mMaterialProgramManager->SelectMaterialPrograms([&](RenderableType renderableType, MaterialProgram* materialProgram) {
			mMaterialPrograms[renderableType][materialProgram->GetShadingMode()] = materialProgram;
		});
	}

    MaterialProgram* MaterialProgramSet::GetMaterialProgram(RenderableType renderableType, Material::ShadingMode shadingMode) const
    {
		return mMaterialPrograms[renderableType][shadingMode];
    }
}
