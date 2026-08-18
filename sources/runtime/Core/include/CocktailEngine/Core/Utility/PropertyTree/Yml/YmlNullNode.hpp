#ifndef COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_YML_YMLNULLNODE_HPP
#define COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_YML_YMLNULLNODE_HPP

#include <CocktailEngine/Core/Utility/PropertyTree/Yml/YmlProperties.hpp>

namespace Ck
{
    /**
     * \brief Node holding the null value of the Yml format
     *
     * Yml tells a key holding no value at all ("key:", "key: ~", "key: null") from a key holding an
     * empty string ("key: \"\""). A value node cannot express that difference, as both would hold an
     * empty String, so a key of the first kind is bound to a node of this type instead.
     *
     * Such a node holds nothing: reading it is asking whether it is there.
     */
    class COCKTAILENGINE_CORE_API YmlNullNode : public YmlProperties::CustomNodeType
    {
    public:

        /**
         * \brief Check whether a node is a null node
         *
         * \param node The node to check
         *
         * \return True if \p node holds a null value, false otherwise
         */
        static bool IsNullNode(const YmlProperties::NodeType& node);

        /**
         * \brief Default constructor
         */
        YmlNullNode() = default;

    protected:

        /**
         * \brief Constructor
         *
         * \param parent The parent node
         * \param name The name of this node into \p parent node
         */
        YmlNullNode(YmlProperties::NodeType* parent, const String& name);

        /**
         * \brief Clone this node
         *
         * \param parent The parent of the clone
         * \param name The name of the clone into \p parent node
         *
         * \return The clone
         */
        UniquePtr<YmlProperties::NodeType> Clone(YmlProperties::NodeType* parent, const String& name) const override;
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_PROPERTYTREE_YML_YMLNULLNODE_HPP
