#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXREF_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXREF_HPP

#include <cstring>

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Math/Vector/Vector4.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexAttributeSemantic.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API VertexRef
	{
	public:

		/**
		 * \brief 
		 * \tparam T 
		 * \param semantic 
		 * \return 
		 */
		template <typename T>
		const T* Get(VertexAttributeSemantic semantic) const
		{
			const VertexAttribute* vertexAttribute = mVertexLayout->FindAttribute(semantic);
			if (!vertexAttribute)
				return nullptr;

			if (vertexAttribute->GetType() != DataType::Of<T>())
				return nullptr;

			std::size_t offset;
			if (mInterlaced)
			{
				offset = mIndex * mVertexLayout->GetStride() + vertexAttribute->GetOffset();
			}
			else
			{
				offset = vertexAttribute->GetOffset() * mVertexCount + vertexAttribute->GetStride() * mIndex;
			}

			return reinterpret_cast<T*>(mStorage + offset);
		}

		/**
		 * \brief 
		 * \tparam T 
		 * \param semantic 
		 * \param value
		 * \param elementCount 
		 */
		template <typename T>
		void Set(VertexAttributeSemantic semantic, const T* value, unsigned int elementCount)
		{
			const VertexAttribute* vertexAttribute = mVertexLayout->FindAttribute(semantic);
			if (!vertexAttribute)
				return;

			if (vertexAttribute->GetType() != DataType::Of<T>())
				return;

			if (vertexAttribute->GetElementCount() * vertexAttribute->GetArrayLength() != elementCount)
				return;

			std::size_t offset;
			if (mInterlaced)
			{
				offset = mIndex * mVertexLayout->GetStride() + vertexAttribute->GetOffset();
			}
			else
			{
				offset = vertexAttribute->GetOffset() * mVertexCount + vertexAttribute->GetStride() * mIndex;
			}

			std::memcpy(reinterpret_cast<T*>(mStorage + offset), value, sizeof(T) * elementCount);
		}

		/**
		 * \brief
		 * \tparam T
		 * \param semantic
		 * \param vector
		 */
		template <typename T>
		void Set(VertexAttributeSemantic semantic, const Vector2<T>& vector)
		{
			Set<T>(semantic, static_cast<const T*>(vector), 2);
		}

		/**
		 * \brief
		 * \tparam T
		 * \param semantic
		 * \param vector
		 */
		template <typename T>
		void Set(VertexAttributeSemantic semantic, const Vector3<T>& vector)
		{
			Set<T>(semantic, static_cast<const T*>(vector), 3);
		}

		/**
		 * \brief
		 * \tparam T
		 * \param semantic
		 * \param vector
		 */
		template <typename T>
		void Set(VertexAttributeSemantic semantic, const Vector4<T>& vector)
		{
			Set<T>(semantic, static_cast<const T*>(vector), 4);
		}

		/**
		 * \brief
		 * \tparam T
		 * \param semantic
		 * \param color
		 */
		void Set(VertexAttributeSemantic semantic, const LinearColor& color);

	private:

		friend class VertexArray;

		/**
		 * \brief
		 * \param vertexLayout
		 * \param storage
		 * \param index
		 * \param vertexCount
		 * \param interlaced
		 */
		VertexRef(VertexLayout* vertexLayout, unsigned char* storage, std::size_t index, std::size_t vertexCount, bool interlaced);

		VertexLayout* mVertexLayout;
		unsigned char* mStorage;
		std::size_t mIndex;
		std::size_t mVertexCount;
		bool mInterlaced;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXREF_HPP
