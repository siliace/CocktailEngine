#ifndef COCKTAIL_GRAPHIC_MESH_MESH_HPP
#define COCKTAIL_GRAPHIC_MESH_MESH_HPP

#include <unordered_set>

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Export.hpp>

#include <Cocktail/Renderer/Renderer.hpp>

namespace Ck
{
	class GraphicEngine;
	class IndexArray;
	class VertexArray;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API Mesh : public Extends<Mesh, Object>
	{
	public:

		/**
		 * \brief 
		 */
		struct SubMesh
		{
			/**
			 * \brief Specifies the number of index or vertex of the sub-mesh
			 */
			unsigned int Count = 0;

			/**
			 * \brief Specifies the index of the first vertex of the sub-mesh in the global mesh's vertex array
			 */
			unsigned int FirstVertex = 0;

			/**
			 * \brief Specifies the index of the first index of the sub-mesh in the global mesh's index array
			 * If the mesh is not indexed, this value should be ignored
			 */
			unsigned int FirstIndex = 0;

			/**
			 * \brief The primitive used to compose the sub-mesh's geometry
			 */
			Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;

			/**
			 * \brief Specifies the index of material to use to draw the sub-mesh
			 */
			unsigned int MaterialIndex = 0;
		};

		/**
		 * \brief 
		 * \param vertices 
		 * \param indices 
		 * \param subMeshes 
		 */
		Mesh(Ref<VertexArray> vertices, Ref<IndexArray> indices, std::vector<SubMesh> subMeshes);

		/**
		 * \brief
		 * \param vertices
		 * \param indices
		 * \param primitiveTopology
		 */
		Mesh(Ref<VertexArray> vertices, Ref<IndexArray> indices, Renderer::PrimitiveTopology primitiveTopology = Renderer::PrimitiveTopology::Triangle);

		/**
		 * \brief 
		 * \return 
		 */
		bool IsIndexed() const;

		/**
		 * \brief 
		 * \return 
		 */
		const Ref<VertexArray>& GetVertices() const;

		/**
		 * \brief 
		 * \return 
		 */
		const Ref<IndexArray>& GetIndices() const;

		/**
		 * \brief 
		 * \return 
		 */
		const std::vector<SubMesh>& GetSubMeshes() const;

		/**
		 * \brief 
		 * \return 
		 */
		const std::unordered_set<unsigned int>& GetMaterialIndices() const;

	private:

		Ref<VertexArray> mVertices;
		Ref<IndexArray> mIndices;
		std::vector<SubMesh> mSubMeshes;
		std::unordered_set<unsigned int> mMaterialIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_MESH_MESH_HPP
