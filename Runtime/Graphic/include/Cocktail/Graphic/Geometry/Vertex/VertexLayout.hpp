#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP

#include <memory>

#include <Cocktail/Core/DataType.hpp>
#include <Cocktail/Core/Flags.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexAttribute.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	struct VertexLayoutCreateInfo
	{
		/**
		 * \brief 
		 */
		struct VertexLayoutAttributeInfo
		{
			/**
			 * \brief 
			 */
			VertexAttributeSemantic Semantic;

			/**
			 * \brief 
			 */
			DataType DataType;

			/**
			 * \brief 
			 */
			unsigned int ElementCount = 0;

			/**
			 * \brief 
			 */
			unsigned int ArrayLength = 1;

			/**
			 * \brief 
			 */
			bool Normalized = false;
		};

		/**
		 * \brief 
		 */
		unsigned int VertexAttributeCount = 0;

		/**
		 * \brief 
		 */
		VertexLayoutAttributeInfo VertexAttributes[Enum<VertexAttributeSemantic>::ValueCount];

		/**
		 * \brief 
		 */
		bool Instanced = false;

		/**
		 * \brief 
		 */
		unsigned int Divisor = 0;
	};

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API VertexLayout
	{
	public:

		class COCKTAIL_GRAPHIC_API Builder
		{
		public:

			Builder& AddAttribute(VertexAttributeSemantic semantic, const DataType& dataType, unsigned int elementCount, unsigned int arrayLength = 1, bool normalized = false);
			Builder& SetInstanced(bool instanced = true);
			Builder& SetDivisor(unsigned int divisor);
			std::shared_ptr<VertexLayout> Get();
			const VertexLayoutCreateInfo& ToCreateInfo() const;

		private:

			VertexLayoutCreateInfo mCreateInfo;
		};

		/**
		 * \brief 
		 * \param createInfo 
		 */
		explicit VertexLayout(const VertexLayoutCreateInfo& createInfo);

		/**
		 * \brief
		 * \param semantic
		 * \return
		 */
		const VertexAttribute* FindAttribute(VertexAttributeSemantic semantic) const;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		bool IsCompatibleWith(const VertexLayout& other) const;

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

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator==(const VertexLayout& rhs) const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator!=(const VertexLayout& rhs) const;

	private:

		/**
		 * \brief
		 * \param semantic
		 * \param dataType
		 * \param elementCount
		 * \param arrayLength
		 * \param normalized
		 */
		const VertexAttribute& CreateAttribute(VertexAttributeSemantic semantic, const DataType& dataType, unsigned int elementCount, unsigned int arrayLength = 1, bool normalized = false);

		bool mInstanced;
		unsigned int mDivisor;
		unsigned int mStride;
		Flags<VertexAttributeSemantic> mUsageMask;
		std::vector<VertexAttribute> mAttributes;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXLAYOUT_HPP
