#ifndef COCKTAIL_VULKAN_SHADER_VERTEXATTRIBUTELOCATION_HPP
#define COCKTAIL_VULKAN_SHADER_VERTEXATTRIBUTELOCATION_HPP

#include <Cocktail/Renderer/Shader/VertexAttributeLocation.hpp>

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
		VertexAttributeLocation(unsigned int location, const DataType& dataType, unsigned int elementCount, std::string name);

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
		const std::string& GetName() const override;

	private:

		unsigned int mLocation;
		DataType mDataType;
		unsigned int mElementCount;
		std::string mName;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_VERTEXATTRIBUTELOCATION_HPP
