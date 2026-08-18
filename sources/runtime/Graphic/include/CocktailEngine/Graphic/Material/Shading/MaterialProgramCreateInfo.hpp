#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP

#include <CocktailEngine/Core/Utility/ByteArray.hpp>

#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramInterface.hpp>

#include <CocktailEngine/Renderer/Shader/ShaderType.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	struct MaterialProgramCreateInfo
	{
		/**
		 * \brief 
		 */
		String Name;

	    /**
	     * \brief 
	     */
	    Material::ShadingMode ShadingMode;

		/**
		 * \brief 
		 */
		SharedPtr<MaterialProgramInterface> Interface;

		/**
		 * \brief 
		 */
		Array<EnumMap<Renderer::ShaderType, ByteArray>> VariantsBinaries;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP
