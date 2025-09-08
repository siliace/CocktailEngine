#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP

#include <Cocktail/Core/System/FileSystem/Path.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTree.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION_FROM(XmlParseException, RuntimeException);

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
		explicit XmlProperties(const Path& path);
	};
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
