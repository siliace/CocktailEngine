#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP

#include <Cocktail/Core/Utility/ByteArray.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexRef.hpp>

namespace Ck
{
	/**
	 * \brief Container class for attributes of vertices sharing a same format
	 */
	class COCKTAIL_GRAPHIC_API VertexArray
	{
	public:

		/**
		 * \brief Constructor
		 * \param layout The format of vertices stored
		 * \param vertexCount The number of vertices stored
		 * \param interlaced True if vertices attributes must be stored interlaced, false otherwise
		 */
		VertexArray(std::shared_ptr<VertexLayout> layout, std::size_t vertexCount, bool interlaced = true);

		/**
		 * \brief 
		 * \param other 
		 */
		void Merge(const VertexArray& other);

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		VertexRef At(std::size_t index);

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		VertexConstRef At(std::size_t index) const;

		/**
		 * \brief Get the format of vertices stored
		 * \return The format
		 */
		const std::shared_ptr<VertexLayout>& GetVertexLayout() const;

		/**
		 * \brief Get the number of vertices stored
		 * \return The number of vertices
		 */
		std::size_t GetVertexCount() const;

		/**
		 * \brief 
		 * \return 
		 */
		const ByteArray& GetVertices() const;

	private:

		std::shared_ptr<VertexLayout> mVertexLayout;
		std::size_t mVertexCount;
		bool mInterlaced;
		ByteArray mVertices;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP
