#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Geometry/Index/IndexRef.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>
#include <Cocktail/Graphic/Mesh/MeshFactory.hpp>

#include <Cocktail/Renderer/Buffer/IndexType.hpp>

namespace Ck
{
	std::shared_ptr<Mesh> MeshFactory::CreateCube(float size, const LinearColor& color)
	{
		float halfExtent = size / 2.f;
		Vector3<float> minPoint(-halfExtent), maxPoint(halfExtent);
		Box<float> box(minPoint, maxPoint);
		return CreateBox(box, color);
	}

	std::shared_ptr<Mesh> MeshFactory::CreateBox(const Box<float>& box, const LinearColor& color)
	{
		std::shared_ptr<VertexLayout> vertexLayout = VertexLayout::Builder()
		                                 .AddAttribute(VertexAttributeSemantic::Position, DataType::Float32, 3)
		                                 .AddAttribute(VertexAttributeSemantic::Normal, DataType::Float32, 3)
		                                 .AddAttribute(VertexAttributeSemantic::Color, DataType::Float32, 4)
		                                 .Get();

		Vector3<float> minPoint = box.GetMinPoint();
		Vector3<float> maxPoint = box.GetMaxPoint();

		Vector3<float> positions[] = {
			/// front
			Vector3<float>(maxPoint.X(), maxPoint.Y(), maxPoint.Z()),
			Vector3<float>(minPoint.X(), maxPoint.Y(), maxPoint.Z()),
			Vector3<float>(maxPoint.X(), minPoint.Y(), maxPoint.Z()),
			Vector3<float>(minPoint.X(), minPoint.Y(), maxPoint.Z()),

			/// back
			Vector3<float>(maxPoint.X(), maxPoint.Y(), minPoint.Z()),
			Vector3<float>(minPoint.X(), maxPoint.Y(), minPoint.Z()),
			Vector3<float>(maxPoint.X(), minPoint.Y(), minPoint.Z()),
			Vector3<float>(minPoint.X(), minPoint.Y(), minPoint.Z()),

			/// right
			Vector3<float>(maxPoint.X(), maxPoint.Y(), maxPoint.Z()),
			Vector3<float>(maxPoint.X(), minPoint.Y(), maxPoint.Z()),
			Vector3<float>(maxPoint.X(), maxPoint.Y(), minPoint.Z()),
			Vector3<float>(maxPoint.X(), minPoint.Y(), minPoint.Z()),

			/// left
			Vector3<float>(minPoint.X(), maxPoint.Y(), maxPoint.Z()),
			Vector3<float>(minPoint.X(), minPoint.Y(), maxPoint.Z()),
			Vector3<float>(minPoint.X(), maxPoint.Y(), minPoint.Z()),
			Vector3<float>(minPoint.X(), minPoint.Y(), minPoint.Z()),

			/// top
			Vector3<float>(minPoint.X(), maxPoint.Y(), maxPoint.Z()),
			Vector3<float>(maxPoint.X(), maxPoint.Y(), maxPoint.Z()),
			Vector3<float>(minPoint.X(), maxPoint.Y(), minPoint.Z()),
			Vector3<float>(maxPoint.X(), maxPoint.Y(), minPoint.Z()),

			/// bottom
			Vector3<float>(maxPoint.X(), minPoint.Y(), maxPoint.Z()),
			Vector3<float>(minPoint.X(), minPoint.Y(), maxPoint.Z()),
			Vector3<float>(maxPoint.X(), minPoint.Y(), minPoint.Z()),
			Vector3<float>(minPoint.X(), minPoint.Y(), minPoint.Z()),
		};

		Vector3<float> normals[] = {
			Vector3<float>::Forward(),
			Vector3<float>::Backward(),
			Vector3<float>::Right(),
			Vector3<float>::Left(),
			Vector3<float>::Up(),
			Vector3<float>::Down(),
		};

		std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>(std::move(vertexLayout), 24);
		for (std::size_t i = 0; i < vertexArray->GetVertexCount(); i++)
		{
			VertexRef vertex = vertexArray->At(i);
			vertex.SetVector(VertexAttributeSemantic::Position, positions[i]);
			vertex.SetVector(VertexAttributeSemantic::Normal, normals[i % 4]);
			vertex.SetColor(VertexAttributeSemantic::Color, color);
		}

		unsigned short indices[] = {
			0, 1, 2,
			1, 2, 3,

			4, 5, 6,
			5, 6, 7,

			8, 9, 10,
			9, 10, 11,

			12, 13, 14,
			13, 14, 15,

			16, 17, 18,
			17, 18, 19,

			20, 21, 22,
			21, 22, 23
		};

		std::shared_ptr<IndexArray> indexArray = std::make_shared<IndexArray>(Renderer::IndexType::Short, 36);
		for (std::size_t i = 0; i < indexArray->GetIndexCount(); i++)
		{
			IndexRef index = indexArray->At(i);
			index.Set(indices[i]);
		}

		return std::make_shared<Mesh>(std::move(vertexArray), std::move(indexArray));
	}
}
