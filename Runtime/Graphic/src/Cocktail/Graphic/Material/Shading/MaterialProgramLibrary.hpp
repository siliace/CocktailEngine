#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMLIBRARY_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMLIBRARY_HPP

#include <filesystem>

#include <Cocktail/Graphic/Material/Material.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>
#include <Cocktail/Graphic/Rendering/RenderableType.hpp>

#include <Cocktail/Renderer/Shader/ShaderType.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class MaterialProgramLibrary : public Extends<MaterialProgramLibrary, Object>
	{
	public:

		struct Entry
		{
			std::string Name;
			RenderableType RenderableType;
			Material::ShadingMode ShadingMode;
			MaterialProgramInterface Interface;
			std::vector<EnumMap<Renderer::ShaderType, std::string>> VariantsBinaries;
		};

		void Register(const Entry& entry);

		/**
		 * \brief 
		 * \return 
		 */
		const std::vector<Entry>& GetEntries() const;

	private:

		std::vector<Entry> mEntries;
	};
}
#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMLIBRARY_HPP
