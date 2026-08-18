#ifndef COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
#define COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP

#include <CocktailEngine/Core/System/FileSystem/Path.hpp>
#include <CocktailEngine/Core/Utility/PropertyTree/PropertyTree.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION_FROM(XmlParseException, RuntimeException);

	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API XmlProperties : public Properties
	{
	public:
		/**
		 * \brief 
		 * \param path 
		 */
		explicit XmlProperties(const Path& path);
	};
}

#endif // COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_XML_XMLPROPERTIES_HPP
