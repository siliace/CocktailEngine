#ifndef COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFUTILS_HPP
#define COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFUTILS_HPP

#include <CocktailEngine/Core/Color.hpp>
#include <CocktailEngine/Core/DataType.hpp>
#include <CocktailEngine/Core/Math/Quaternion.hpp>
#include <CocktailEngine/Core/Math/Vector/Vector3.hpp>

#include <CocktailEngine/Graphic/Geometry/Vertex/VertexAttributeSemantic.hpp>
#include <CocktailEngine/Graphic/Material/Material.hpp>
#include <CocktailEngine/Graphic/Scene/Light/Light.hpp>

#include <CocktailEngine/Renderer/Buffer/IndexType.hpp>
#include <CocktailEngine/Renderer/Renderer.hpp>

namespace Ck
{
	/**
	 * \brief Utility class containing conversion function to Cocktail data types
	 */
	class GltfUtils
	{
	public:

		static DataType ConvertComponentType(int componentType);
		static Optional<VertexAttributeSemantic> ConvertAttributeName(const AnsiChar* name);
		static Renderer::PrimitiveTopology ConvertPrimitiveTopology(int primitiveMode);
		static Renderer::IndexType ConvertIndexType(int componentType);
	    static Vector3<float> ConvertVector3(const std::vector<double>& values);
		static Quaternion<float> ConvertQuaternion(const std::vector<double>& values);
		static LinearColor ConvertLinearColor(const std::vector<double>& values);
		static Material::AlphaMode ConvertAlphaMode(const AnsiChar* value);
	    static Light::Type ConvertLightType(const AnsiChar* value);
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFUTILS_HPP
