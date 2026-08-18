#ifndef COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_YML_YMLPROPERTIES_HPP
#define COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_YML_YMLPROPERTIES_HPP

#include <CocktailEngine/Core/System/FileSystem/Path.hpp>
#include <CocktailEngine/Core/Utility/PropertyTree/PropertyTree.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_EXCEPTION_FROM(YmlParseException, RuntimeException);

    class COCKTAILENGINE_CORE_API YmlProperties : public Properties
    {
    public:

        explicit YmlProperties(const Path& path);
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_YML_YMLPROPERTIES_HPP
