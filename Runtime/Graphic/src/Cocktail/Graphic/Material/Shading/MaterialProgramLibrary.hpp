#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMLIBRARY_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMLIBRARY_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

#include <Cocktail/Graphic/Material/Material.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>
#include <Cocktail/Graphic/Rendering/RenderableType.hpp>

#include <Cocktail/Renderer/Shader/ShaderType.hpp>

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
			RenderableType RenderableType;

			/**
			 * \brief Specifies the shading mode used by the program
			 */
			Material::ShadingMode ShadingMode;

			/**
			 * \brief Specifies the interface of the program
			 */
			MaterialProgramInterface Interface;

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
