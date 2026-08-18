#ifndef COCKTAILENGINE_VULKAN_SHADER_VERTEXATTRIBUTELOCATION_HPP
#define COCKTAILENGINE_VULKAN_SHADER_VERTEXATTRIBUTELOCATION_HPP

#include <CocktailEngine/Core/String.hpp>

#include <CocktailEngine/Renderer/Shader/VertexAttributeLocation.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class VertexAttributeLocation : public Renderer::VertexAttributeLocation
	{
	public:

		/**
		 * \brief 
		 * \param location 
		 * \param dataType 
		 * \param elementCount 
		 * \param name 
		 */	
		VertexAttributeLocation(unsigned int location, const DataType& dataType, unsigned int elementCount, AsciiString name);

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetLocation() const override;

		/**
		 * \brief 
		 * \return 
		 */
		const DataType& GetDataType() const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetElementCount() const override;

		/**
		 * \brief 
		 * \return 
		 */
		const AsciiString& GetName() const override;

	private:

		unsigned int mLocation;
		DataType mDataType;
		unsigned int mElementCount;
		AsciiString mName;
	};
}

#endif // COCKTAILENGINE_VULKAN_SHADER_VERTEXATTRIBUTELOCATION_HPP
