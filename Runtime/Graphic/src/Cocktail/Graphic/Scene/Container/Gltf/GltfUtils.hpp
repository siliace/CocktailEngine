#ifndef COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFUTILS_HPP
#define COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFUTILS_HPP

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/DataType.hpp>
#include <Cocktail/Core/Math/Quaternion.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexAttributeSemantic.hpp>

#include <Cocktail/Renderer/Renderer.hpp>
#include <Cocktail/Renderer/Buffer/IndexType.hpp>

namespace Ck
{
	/**
	 * \brief Utility class containing conversion function to Cocktail data types
	 */
	class GltfUtils
	{
	public:

		static DataType ConvertComponentType(int componentType);
		static VertexAttributeSemantic ConvertAttributeName(std::string_view name);
		static Renderer::PrimitiveTopology ConvertPrimitiveTopology(int primitiveMode);
		static Renderer::IndexType ConvertIndexType(int componentType);
	    static Vector3<float> ConvertVector3(const std::vector<double>& values);
		static Quaternion<float> ConvertQuaternion(const std::vector<double>& values);
		static LinearColor ConvertLinearColor(const std::vector<double>& values);
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFUTILS_HPP
