#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXREF_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXREF_HPP

#include <cstring>

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Math/Vector/Vector4.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexAttributeSemantic.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API VertexConstRef
	{
	public:

	    template <typename TVector>
	    TVector GetVector(VertexAttributeSemantic semantic) const
	    {
            using VectorElementType = typename TVector::ElementType;

	        TVector vector;

	        const VertexAttribute* vertexAttribute = mVertexLayout->FindAttribute(semantic);
	        assert(vertexAttribute);
	        assert(vertexAttribute->GetType() == DataType::Of<VectorElementType>());
	        assert(vertexAttribute->GetElementCount() * vertexAttribute->GetArrayLength() == TVector::Size);

	        std::size_t offset;
	        if (mInterlaced)
	        {
	            offset = mIndex * mVertexLayout->GetStride() + vertexAttribute->GetOffset();
	        }
	        else
	        {
	            offset = vertexAttribute->GetOffset() * mVertexCount + vertexAttribute->GetStride() * mIndex;
	        }

	        const auto* data = reinterpret_cast<const VectorElementType*>(mVertices->GetData() + offset);
	        for (std::size_t i = 0; i < TVector::Size; i++)
	            vector.At(i) = data[i];

	        return vector;
	    }

		/**
		 * \brief
		 * \param semantic
		 * \return
		 */
		const void* Get(VertexAttributeSemantic semantic) const;

	protected:

		friend class VertexArray;

		/**
		 * \brief
		 * \param vertexLayout
		 * \param vertices
		 * \param index
		 * \param vertexCount
		 * \param interlaced
		 */
		VertexConstRef(const VertexLayout* vertexLayout, const ByteArray* vertices, std::size_t index, std::size_t vertexCount, bool interlaced);

		const VertexLayout* mVertexLayout;
		std::size_t mIndex;
		std::size_t mVertexCount;
		bool mInterlaced;

	private:

		const ByteArray* mVertices;
	};

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API VertexRef : public VertexConstRef
	{
	public:

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
			static_assert(!std::is_void_v<T>, "Cannot set vertex ref value from non-typed memory without data type");

			const VertexAttribute* vertexAttribute = mVertexLayout->FindAttribute(semantic);
			assert(vertexAttribute);
			assert(vertexAttribute->GetType() == DataType::Of<T>());
			assert(vertexAttribute->GetElementCount() * vertexAttribute->GetArrayLength() == elementCount);

			std::size_t offset;
			if (mInterlaced)
			{
				offset = mIndex * mVertexLayout->GetStride() + vertexAttribute->GetOffset();
			}
			else
			{
				offset = vertexAttribute->GetOffset() * mVertexCount + vertexAttribute->GetStride() * mIndex;
			}

			Memory::Copy(reinterpret_cast<T*>(mVertices->GetData() + offset), value, sizeof(T) * elementCount);
		}

		/**
		 * \brief 
		 * \param semantic 
		 * \param value 
		 * \param dataType 
		 * \param elementCount 
		 */
		void Set(VertexAttributeSemantic semantic, const void* value, DataType dataType, unsigned int elementCount);

		/**
		 * \brief
		 * \tparam T
		 * \param semantic
		 * \param vector
		 */
		template <template <class> typename TVector, typename T, std::size_t S>
		void SetVector(VertexAttributeSemantic semantic, const Vector<TVector, T, S>& vector)
		{
			Set<T>(semantic, static_cast<const T*>(vector), S);
		}

		/**
		 * \brief
		 * \tparam T
		 * \param semantic
		 * \param color
		 */
		void SetColor(VertexAttributeSemantic semantic, const LinearColor& color);

	private:

		friend class VertexArray;

		/**
		 * \brief
		 * \param vertexLayout
		 * \param vertices
		 * \param index
		 * \param vertexCount
		 * \param interlaced
		 */
		VertexRef(const VertexLayout* vertexLayout, ByteArray* vertices, std::size_t index, std::size_t vertexCount, bool interlaced);

		ByteArray* mVertices;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXREF_HPP
