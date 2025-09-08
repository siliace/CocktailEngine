#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREESEQUENCE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREESEQUENCE_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/TranslatorCast.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeNode.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreePath.hpp>

namespace Ck
{
    template <typename Data>
    class PropertyTreeElement;

    template <typename Data>
    class PropertyTreeValue;

    template <typename Data>
    class PropertyTreeSequence : public PropertyTreeNode<Data>
    {
    public:

        using Element = PropertyTreeElement<Data>;
        using Node = PropertyTreeNode<Data>;
        using Sequence = PropertyTreeSequence;
        using Value = PropertyTreeValue<Data>;

	    /**
         * \brief 
         */
        PropertyTreeSequence() = default;

        /**
         * \brief 
         * \tparam NodeType 
         * \param node 
         * \return 
         */
        template <template<class> class NodeType, typename = std::enable_if_t<std::is_base_of_v<Node, NodeType<Data>>>>
        NodeType<Data>* Add(const NodeType<Data>& node)
        {
            unsigned int index = GetSize();
            String keyIndex = TranslatorCast<String>(index);
            std::unique_ptr<Node> clone = static_cast<const Node&>(node).Clone(this, keyIndex);
            Node* inserted = mChildren.Emplace(std::move(clone)).get();
            return static_cast<NodeType<Data>*>(inserted);
        }

        /**
         * \brief
         * \tparam T
         * \tparam Tr
         * \param path
         * \param translator
         * \return
         */
        template <typename T, typename Tr = typename TranslatorBetween<Data, T>::Type>
        T Get(const PropertyTreePath& path, const Tr& translator = Tr()) const
        {
            return GetValue(path).template As<T, Tr>(translator);
        }

        /**
         * \brief
         * \tparam T
         * \tparam Tr
         * \param path
         * \param translator
         * \return
         */
        template <typename T, typename Tr = typename TranslatorBetween<Data, T>::Type>
        T Get(unsigned int index, const PropertyTreePath& path, const Tr& translator = Tr()) const
        {
            return At(index)->template GetValue<T, Tr>(path, translator);
        }

        /**
         * \brief 
         * \param path 
         * \return 
         */
        Node& GetChild(const PropertyTreePath& path) const
        {
            PropertyTreePath p(path);
            unsigned int index = p.Reduce<unsigned int>();
            if (index >= mChildren.GetSize())
                throw InvalidPropertyPathException(CK_TEXT("PropertyTreePath target sequence %s does not contains child with index %u"), this->mName, index);

            const Node* child = mChildren[index].get();
            if (child->GetType() == Node::Type::Element)
                return static_cast<const PropertyTreeElement<Data>*>(child)->GetChild(p);

            if (child->GetType() == Node::Type::Sequence)
                return static_cast<const PropertyTreeSequence*>(child)->GetChild(p);

            throw InvalidPropertyPathException(CK_TEXT("Child with index %u in node %s is terminal"), index, this->mName);
        }
        /**
         * \brief
         * \param path
         * \return
         */
        Element& GetElement(const PropertyTreePath& path) const
        {
            Node& node = GetChild(path);
            if (node.GetType() != Node::Type::Element)
                throw InvalidPropertyPathException(CK_TEXT("PropertyTreePath target node {} is not an element"), path.ToString());

            return static_cast<Element&>(node);
        }

        /**
         * \brief
         * \param path
         * \return
         */
        Sequence& GetSequence(const PropertyTreePath& path) const
        {
            Node& node = GetChild(path);
            if (node.GetType() != Node::Type::Sequence)
                throw InvalidPropertyPathException(CK_TEXT("PropertyTreePath target node {} is not a sequence"), path.ToString());

            return static_cast<Sequence&>(node);
        }

        /**
         * \brief
         * \param path
         * \return
         */
        Value& GetValue(const PropertyTreePath& path) const
        {
            Node& node = GetChild(path);
            if (node.GetType() != Node::Type::Value)
                throw InvalidPropertyPathException(CK_TEXT("PropertyTreePath target node {} is not a value"), path.ToString());

            return static_cast<Value&>(node);
        }

	    /**
         * \brief 
         * \return 
         */
        bool IsEmpty() const
        {
            return mChildren.IsEmpty();
        }

	    /**
         * \brief 
         * \return 
         */
        unsigned int GetSize() const
        {
            return mChildren.GetSize();
        }

        /**
         * \brief
         * \param index
         * \return
         */
        Node& At(unsigned int index)
        {
            return *mChildren.At(index);
        }

        /**
         * \brief
         * \param index
         * \return
         */
        const Node& At(unsigned int index) const
        {
            return *mChildren.At(index);
        }

        /**
         * \brief
         * \param index
         * \return
         */
        Element& ElementAt(unsigned int index)
        {
            Node& child = At(index);
            if (child.GetType() != Node::Type::Element)
                throw InvalidPropertyPathException("Sequence {} child node at index {} is not an Element", this->mName, index);

            return static_cast<Element&>(child);
        }

	    /**
         * \brief 
         * \param index 
         * \return 
         */
        const Element& ElementAt(unsigned int index) const
        {
            const Node& child = At(index);
            if (child.GetType() != Node::Type::Element)
                throw InvalidPropertyPathException("Sequence {} child node at index {} is not an Element", this->mName, index);

            return static_cast<const Element&>(child);
        }
            
        /**
         * \brief
         * \return
         */
        typename PropertyTreeSequence::Type GetType() const override
        {
            return PropertyTreeSequence::Type::Sequence;
        }

    protected:

        /**
         * \brief Constructor
         * \param parent The parent node
         * \param name The name of this node into \p parent node
         */
        PropertyTreeSequence(Node* parent, const String& name) :
            Node(parent, name)
        {
            /// Nothing
        }

	    /**
         * \brief 
         * \param parent 
         * \param name 
         * \return 
         */
        std::unique_ptr<Node> Clone(Node* parent, const String& name) const override
        {
            std::unique_ptr<PropertyTreeSequence<Data>> clone(new PropertyTreeSequence<Data>(parent, name)); /// TODO: find a way to use std::make_unique
            for (const std::unique_ptr<Node>& child : mChildren)
                clone->Add(*child);

            return clone;
        }

    private:

        Array64<std::unique_ptr<Node>> mChildren;
    };
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREESEQUENCE_HPP
