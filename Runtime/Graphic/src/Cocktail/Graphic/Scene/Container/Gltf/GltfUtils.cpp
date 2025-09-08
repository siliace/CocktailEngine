#include <tiny_gltf.h>

#include <Cocktail/Core/Utility/StringUtils.hpp>

#include <Cocktail/Graphic/Scene/Container/Gltf/GltfUtils.hpp>

namespace Ck
{
	DataType GltfUtils::ConvertComponentType(int componentType)
	{
        switch (componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_BYTE:
            return DataType::Int8;

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            return DataType::UnsignedInt8;

        case TINYGLTF_COMPONENT_TYPE_SHORT:
            return DataType::Int16;

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            return DataType::UnsignedInt16;

        case TINYGLTF_COMPONENT_TYPE_INT:
            return DataType::Int32;

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            return DataType::UnsignedInt32;

        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            return DataType::Float32;

        case TINYGLTF_COMPONENT_TYPE_DOUBLE:
            return DataType::Float64;
        }

        COCKTAIL_UNREACHABLE();
	}

	VertexAttributeSemantic GltfUtils::ConvertAttributeName(const AnsiChar* name)
	{
        if (StringUtils<AnsiChar>::Equal(name, "POSITION"))
            return VertexAttributeSemantic::Position;

		if (StringUtils<AnsiChar>::Equal(name, "NORMAL"))
            return VertexAttributeSemantic::Normal;
        
        if (StringUtils<AnsiChar>::Equal(name, "TEXCOORD_0"))
            return VertexAttributeSemantic::TexCoord;
        
        if (StringUtils<AnsiChar>::Equal(name, "TANGENT"))
            return VertexAttributeSemantic::Tangent;

        COCKTAIL_UNREACHABLE();
	}

	Renderer::PrimitiveTopology GltfUtils::ConvertPrimitiveTopology(int primitiveMode)
	{
        switch (primitiveMode)
        {
        case TINYGLTF_MODE_POINTS:
            return Renderer::PrimitiveTopology::Point;

        case TINYGLTF_MODE_LINE:
            return Renderer::PrimitiveTopology::Line;

        case TINYGLTF_MODE_LINE_STRIP:
            return Renderer::PrimitiveTopology::LineStrip;

        case TINYGLTF_MODE_TRIANGLES:
            return Renderer::PrimitiveTopology::Triangle;

        case TINYGLTF_MODE_TRIANGLE_STRIP:
            return Renderer::PrimitiveTopology::TriangleStrip;

        case TINYGLTF_MODE_TRIANGLE_FAN:
            return Renderer::PrimitiveTopology::TriangleFan;
        }

        COCKTAIL_UNREACHABLE();
	}

	Renderer::IndexType GltfUtils::ConvertIndexType(int componentType)
	{
        switch (componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            return Renderer::IndexType::Byte;

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            return Renderer::IndexType::Short;

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            return Renderer::IndexType::Integer;
        }

        COCKTAIL_UNREACHABLE();
	}

	Vector3<float> GltfUtils::ConvertVector3(const std::vector<double>& values)
	{
		Vector3<float> vector;
		assert(values.size() == 3);
		for (std::size_t i = 0; i < values.size(); i++)
			vector.At(i) = static_cast<float>(values[i]);

		return vector;
	}

	Quaternion<float> GltfUtils::ConvertQuaternion(const std::vector<double>& values)
	{
		Quaternion<float> quaternion;
		assert(values.size() == 4);
		quaternion.X = static_cast<float>(values[0]);
		quaternion.Y = static_cast<float>(values[1]);
		quaternion.Z = static_cast<float>(values[2]);
		quaternion.W = static_cast<float>(values[3]);

		return quaternion;
	}

	LinearColor GltfUtils::ConvertLinearColor(const std::vector<double>& values)
	{
		LinearColor color;
		assert(values.size() == 3 || values.size() == 4);
		color.R = static_cast<float>(values[0]);
		color.G = static_cast<float>(values[1]);
		color.B = static_cast<float>(values[2]);
		color.A = values.size() == 4 ? static_cast<float>(values[3]) : 1.f;

		return color;
	}

	Material::AlphaMode GltfUtils::ConvertAlphaMode(const AnsiChar* value)
	{
        if (StringUtils<AnsiChar>::Equal(value, "OPAQUE"))
			return Material::AlphaMode::Opaque;

		if (StringUtils<AnsiChar>::Equal(value, "BLEND"))
			return Material::AlphaMode::Blend;

		if (StringUtils<AnsiChar>::Equal(value, "MASK"))
			return Material::AlphaMode::Mask;

		COCKTAIL_UNREACHABLE();
	}
}
