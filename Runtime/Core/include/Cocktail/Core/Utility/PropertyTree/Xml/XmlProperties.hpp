#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP

#include <filesystem>

#include <Cocktail/Core/Utility/PropertyTree/PropertyTree.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API XmlProperties : public Properties
	{
	public:
		/**
		 * \brief 
		 * \param path 
		 */
		explicit XmlProperties(const std::filesystem::path& path);
	};
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
