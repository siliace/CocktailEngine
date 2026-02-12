#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP

#include <Cocktail/Core/Utility/ByteArray.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>

#include <Cocktail/Renderer/Shader/ShaderType.hpp>

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
		MaterialProgramInterface Interface;

		/**
		 * \brief 
		 */
		Array<EnumMap<Renderer::ShaderType, ByteArray>> VariantsBinaries;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP
