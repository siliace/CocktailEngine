#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMLIBRARY_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMLIBRARY_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Utility/ByteArray.hpp>

#include <CocktailEngine/Graphic/Material/Material.hpp>
#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramInterface.hpp>
#include <CocktailEngine/Graphic/Rendering/RenderableType.hpp>

#include <CocktailEngine/Renderer/Shader/ShaderType.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class MaterialProgramLibrary
	{
	public:

		struct Entry
		{
			/**
			 * \brief Specifies the name of the program
			 */
			String Name;

			/**
			 * \brief Specifies the type of object renderable with this program
			 */
			RenderableType Renderable;

			/**
			 * \brief Specifies the shading mode used by the program
			 */
			Material::ShadingMode ShadingMode;

			/**
			 * \brief Specifies the interface of the program
			 */
			SharedPtr<MaterialProgramInterface> Interface;

			/**
			 * \brief An array of binaries per shader stage
			 */
			Array<EnumMap<Renderer::ShaderType, ByteArray>> VariantsBinaries;
		};

		void Register(const Entry& entry);

		/**
		 * \brief 
		 * \return 
		 */
		const Array<Entry>& GetEntries() const;

	private:

		Array<Entry> mEntries;
	};
}
#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMLIBRARY_HPP
