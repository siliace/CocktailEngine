#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramSet.hpp>

namespace Ck
{
	MaterialProgramSet::MaterialProgramSet(SharedPtr<MaterialProgramManager> materialProgramManager) :
		mMaterialProgramManager(Move(materialProgramManager))
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
