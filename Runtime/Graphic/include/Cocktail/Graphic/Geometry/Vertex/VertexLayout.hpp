#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/DataType.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexAttribute.hpp>

#include "Cocktail/Core/Flags.hpp"
#include "Cocktail/Core/Utility/LazyValue.hpp"

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API VertexLayout : public Extends<VertexLayout, Object>
	{
	public:

		/**
		 * \brief
		 * \param lhs
		 * \param rhs
		 * \return
		 */
		static Ref<VertexLayout> Merge(const VertexLayout& lhs, const VertexLayout& rhs);

		/**
		 * \brief
		 * \param instanced
		 * \param divisor
		 */
		explicit VertexLayout(bool instanced = false, unsigned int divisor = 0);

		/**
		 * \brief
		 * \param semantic
		 * \param dataType
		 * \param elementCount
		 * \param arrayLength
		 * \param normalized
		 */
		const VertexAttribute& CreateAttribute(VertexAttributeSemantic semantic, const DataType& dataType, unsigned int elementCount, unsigned int arrayLength = 1, bool normalized = false);

		/**
		 * \brief
		 * \param semantic
		 * \return
		 */
		const VertexAttribute* FindAttribute(VertexAttributeSemantic semantic) const;

		/**
		 * \brief
		 * \return
		 */
		unsigned int GetStride() const;

		/**
		 * \brief 
		 * \return 
		 */
		Flags<VertexAttributeSemantic> GetUsageMask() const;

		/**
		 * \brief
		 * \return
		 */
		bool IsInstanced() const;

		/**
		 * \brief
		 * \return
		 */
		unsigned int GetDivisor() const;

		/**
		 * \brief
		 * \return
		 */
		const std::vector<VertexAttribute>& GetAttributes() const;

	private:

		bool mInstanced;
		unsigned int mDivisor;
		LazyValue<unsigned int> mStride;
		LazyValue<Flags<VertexAttributeSemantic>> mUsageMask;
		std::vector<VertexAttribute> mAttributes;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP
