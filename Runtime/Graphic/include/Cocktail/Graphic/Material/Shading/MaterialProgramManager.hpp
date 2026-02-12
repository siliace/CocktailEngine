#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMMANAGER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMMANAGER_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgram.hpp>
#include <Cocktail/Graphic/Rendering/RenderableType.hpp>

namespace Ck
{
    class COCKTAIL_GRAPHIC_API MaterialProgramManager
    {
    public:

        /**
         * \brief
         * \param renderDevice
         */
        explicit MaterialProgramManager(Renderer::RenderDevice* renderDevice);

        MaterialProgramManager(const MaterialProgramManager& other) = delete;
        MaterialProgramManager(MaterialProgramManager&& other) = default;
        MaterialProgramManager& operator=(const MaterialProgramManager& other) = delete;
        MaterialProgramManager& operator=(MaterialProgramManager&& other) = default;

        /**
         * \brief
         * \tparam Callable
         * \param callable
         */
        template <typename Callable>
        void SelectMaterialPrograms(Callable callable)
        {
            for (const auto& [key, materialProgramList] : mMaterialPrograms)
            {
                for (const auto& materialProgram : materialProgramList)
                {
                    std::invoke(callable, key, materialProgram.Get());
                }
            }
        }

    private:

        /**
         * \brief
         * \param renderableType
         * \param materialProgram
         */
        void Register(RenderableType renderableType, UniquePtr<MaterialProgram> materialProgram);

        Renderer::RenderDevice* mRenderDevice;
        std::unordered_map<RenderableType, Array<UniquePtr<MaterialProgram>>> mMaterialPrograms;
    };
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMMANAGER_HPP
