#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREECUSTOMNODE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREECUSTOMNODE_HPP

#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeNode.hpp>

namespace Ck
{
    /**
     * \brief Specialization of PropertyTreeNode implementing custom node type
     */
    template <typename Key, typename Data>
    class PropertyTreeCustomNode : public PropertyTreeNode<Key, Data>
    {
    public:

	    /**
         * \brief 
         */
        PropertyTreeCustomNode() = default;
    
        /**
         * \brief
         * \return
         */
        typename PropertyTreeCustomNode::Type GetType() const override
        {
            return PropertyTreeNode<Key, Data>::Type::Custom;
        }

    protected:

        /**
         * \brief Constructor
         * \param parent The parent node
         * \param name The name of this node into \p parent node
         */
        PropertyTreeCustomNode(PropertyTreeNode<Key, Data>* parent, const Key& name) :
            PropertyTreeNode<Key, Data>(parent, name)
        {
            /// Nothing
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREECUSTOMNODE_HPP
