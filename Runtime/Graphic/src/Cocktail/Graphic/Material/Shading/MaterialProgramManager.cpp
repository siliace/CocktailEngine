#include <Cocktail/Core/Application/App.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramManager.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>

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

            Register(entry.RenderableType, MakeUnique<MaterialProgram>(mRenderDevice, materialProgramCreateInfo));
        }
    }

    void MaterialProgramManager::Register(RenderableType renderableType, UniquePtr<MaterialProgram> materialProgram)
    {
        mMaterialPrograms[renderableType].Emplace(std::move(materialProgram));
    }
}
