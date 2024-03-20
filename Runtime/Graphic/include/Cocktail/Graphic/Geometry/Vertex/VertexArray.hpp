#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP

#include <memory>

#include <Cocktail/Graphic/Geometry/Vertex/VertexRef.hpp>

namespace Ck
{
	/**
	 * \brief Container class for attributes of vertices sharing a same format
	 */
	class COCKTAIL_GRAPHIC_API VertexArray : public Extends<VertexArray, Object>
	{
	public:

		/**
		 * \brief Merge two vertex array
		 * \param lhs 
		 * \param rhs 
		 * \return 
		 */
		static Ref<VertexArray> Merge(const VertexArray& lhs, const VertexArray& rhs);

		/**
		 * \brief Constructor
		 * \param layout The format of vertices stored
		 * \param vertexCount The number of vertices stored
		 * \param interlaced True if vertices attributes must be stored interlaced, false otherwise
		 */
		VertexArray(Ref<VertexLayout> layout, std::size_t vertexCount, bool interlaced = true);

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		VertexRef At(std::size_t index) const;

		/**
		 * \brief Get the format of vertices stored
		 * \return The format
		 */
		const Ref<VertexLayout>& GetVertexLayout() const;

		/**
		 * \brief Get the number of vertices stored
		 * \return The number of vertices
		 */
		std::size_t GetVertexCount() const;

		/**
		 * \brief 
		 * \return 
		 */
		const void* GetPointer() const;

	private:

		Ref<VertexLayout> mVertexLayout;
		std::size_t mVertexCount;
		bool mInterlaced;
		std::unique_ptr<unsigned char[]> mStorage;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP
