#include <CocktailEngine/Core/Application/App.hpp>

#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramLibrary.hpp>
#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramManager.hpp>

#include <CocktailEngine/Renderer/RenderDevice.hpp>

namespace Ck
{
    MaterialProgramManager::MaterialProgramManager(Renderer::RenderDevice* renderDevice) :
        mRenderDevice(renderDevice)
    {
        for (const MaterialProgramLibrary::Entry& entry : App::Resolve<MaterialProgramLibrary>()->GetEntries())
        {
            MaterialProgramCreateInfo materialProgramCreateInfo;
            materialProgramCreateInfo.Name = entry.Name;
            materialProgramCreateInfo.ShadingMode = entry.ShadingMode;
            materialProgramCreateInfo.Interface = entry.Interface;
            materialProgramCreateInfo.VariantsBinaries = entry.VariantsBinaries;

            Register(entry.Renderable, MakeUnique<MaterialProgram>(mRenderDevice, materialProgramCreateInfo));
        }
    }

    void MaterialProgramManager::Register(RenderableType renderableType, UniquePtr<MaterialProgram> materialProgram)
    {
        mMaterialPrograms[renderableType].Emplace(Move(materialProgram));
    }
}
