#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXATTRIBUTE_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXATTRIBUTE_HPP

#include <Cocktail/Core/DataType.hpp>

#include <Cocktail/Graphic/Export.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexAttributeSemantic.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class COCKTAIL_GRAPHIC_API VertexAttribute
	{
	public:

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		bool IsCompatibleWith(const VertexAttribute& other) const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetStride() const;

		/**
		 * \brief 
		 * \return 
		 */
		VertexAttributeSemantic GetSemantic() const;

		/**
		 * \brief 
		 * \return 
		 */
		DataType GetType() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetOffset() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetElementCount() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetArrayLength() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsNormalized() const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator==(const VertexAttribute& rhs) const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator!=(const VertexAttribute& rhs) const;

	private:

		friend class VertexLayout;

		/**
		 * \brief 
		 * \param semantic 
		 * \param type 
		 * \param offset 
		 * \param elementCount 
		 * \param arrayLength
		 * \param normalized 
		 */
		VertexAttribute(VertexAttributeSemantic semantic, DataType type, unsigned int offset, unsigned int elementCount, unsigned int arrayLength, bool normalized);

		VertexAttributeSemantic mSemantic;
		DataType mType;
		unsigned int mOffset;
		unsigned int mElementCount;
		unsigned int mArrayLength;
		bool mNormalized;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXATTRIBUTE_HPP
