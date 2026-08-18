#include <CocktailEngine/Core/Utility/PropertyTree/Yml/YmlNullNode.hpp>

namespace Ck
{
    bool YmlNullNode::IsNullNode(const YmlProperties::NodeType& node)
    {
        if (node.GetType() != YmlProperties::NodeType::Type::Custom)
            return false;

        return dynamic_cast<const YmlNullNode*>(&node) != nullptr;
    }

    YmlNullNode::YmlNullNode(YmlProperties::NodeType* parent, const String& name) :
        YmlProperties::CustomNodeType(parent, name)
    {
        /// Nothing
    }

    UniquePtr<YmlProperties::NodeType> YmlNullNode::Clone(YmlProperties::NodeType* parent, const String& name) const
    {
        return UniquePtr<YmlProperties::NodeType>(
            new YmlNullNode(parent, name)
        );
    }
}
