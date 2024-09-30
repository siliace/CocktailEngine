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
		MaterialProgramManager(Ref<Renderer::RenderDevice> renderDevice);

		/**
		 * \brief 
		 * \param renderableType 
		 * \param shadingMode 
		 * \param materialProgram 
		 */
		void Register(RenderableType renderableType, Material::ShadingMode shadingMode, Ref<MaterialProgram> materialProgram);

		/**
		 * \brief Iterate over a subset of registered MaterialProgram
		 * The \p callback will be invoked with each MaterialProgram matching the \p renderableType and \p shadingMode registered as parameter
		 * \param renderableType 
		 * \param shadingMode 
		 * \param callable 
		 */
		template <typename Callable>
		void SelectMaterialPrograms(RenderableType renderableType, Material::ShadingMode shadingMode, Callable callable) const
		{
			for (const auto& [key, materialProgram] : mMaterialPrograms)
			{
				if (std::get<0>(key) != renderableType || std::get<1>(key) != shadingMode)
					continue;

				std::invoke(callable, std::get<2>(key), std::get<3>(key), materialProgram);
			}
		}

	private:

		using MaterialProgramKey = CompositeKey<RenderableType, Material::ShadingMode, Flags<VertexAttributeSemantic>, Flags<Material::TextureType>>;

		Ref<Renderer::RenderDevice> mRenderDevice;
		std::unordered_map<MaterialProgramKey, Ref<MaterialProgram>> mMaterialPrograms;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMMANAGER_HPP
