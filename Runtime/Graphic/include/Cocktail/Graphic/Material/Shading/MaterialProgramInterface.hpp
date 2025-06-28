#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMINTERFACE_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMINTERFACE_HPP

#include <string>

#include <Cocktail/Graphic/Geometry/Vertex/VertexAttributeSemantic.hpp>
#include <Cocktail/Graphic/Material/Material.hpp>

#include <Cocktail/Renderer/Texture/StaticSampler.hpp>

namespace Ck
{
	/**
	 * \brief Structure describing the interface between a Material and a MaterialProgram
	 */
	struct MaterialProgramInterface
	{
		/**
		 * \brief Names of vertex attributes in the vertex shader
		 */
		EnumMap<VertexAttributeSemantic, std::string> VertexAttributes;

		struct TextureInterface
		{
			/**
			 * \brief 
			 */
			std::string Name;

			/**
			 * \brief 
			 */
			Renderer::StaticSampler Sampler = Renderer::StaticSampler::LinearClamp;
		};

		/**
		 * \brief Names of the slots of the material textures in the program
		 */
		EnumMap<Material::TextureType, TextureInterface> Textures;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMINTERFACE_HPP
