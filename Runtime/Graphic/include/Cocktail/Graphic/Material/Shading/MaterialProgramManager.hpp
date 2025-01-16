#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMMANAGER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMMANAGER_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Graphic/Rendering/RenderableType.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgram.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API MaterialProgramManager : public Extends<MaterialProgramManager, Object>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit MaterialProgramManager(Ref<Renderer::RenderDevice> renderDevice);

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param shadingMode 
		 * \param callable 
		 */
		template <typename Callable>
		void SelectMaterialPrograms(Material::ShadingMode shadingMode, Callable callable)
		{
			for (const auto&[key, materialProgram] : mMaterialPrograms)
			{
				if (std::get<1>(key) == shadingMode)
					std::invoke(callable, std::get<0>(key), materialProgram);
			}
		}

	private:

		/**
		 * \brief 
		 * \param renderableType 
		 * \param shadingMode 
		 * \param materialProgram 
		 */
		void Register(RenderableType renderableType, Material::ShadingMode shadingMode, Ref<MaterialProgram> materialProgram);

		using MaterialProgramKey = CompositeKey<RenderableType, Material::ShadingMode>;

		Ref<Renderer::RenderDevice> mRenderDevice;
		std::unordered_map<MaterialProgramKey, Ref<MaterialProgram>> mMaterialPrograms;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMMANAGER_HPP
