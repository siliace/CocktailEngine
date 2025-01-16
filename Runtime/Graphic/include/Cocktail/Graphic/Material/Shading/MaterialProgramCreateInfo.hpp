#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP

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
		std::string Name;

		/**
		 * \brief 
		 */
		MaterialProgramInterface Interface;

		/**
		 * \brief 
		 */
		std::vector<EnumMap<Renderer::ShaderType, std::string>> VariantsBinaries;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMCREATEINFO_HPP
