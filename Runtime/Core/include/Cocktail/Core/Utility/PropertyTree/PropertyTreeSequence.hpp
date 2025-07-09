#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREESEQUENCE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREESEQUENCE_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/TranslatorCast.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeNode.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreePath.hpp>

namespace Ck
{
    template <typename Key, typename Data>
    class PropertyTreeElement;

    template <typename Key, typename Data>
    class PropertyTreeValue;

    template <typename Key, typename Data>
    class PropertyTreeSequence : public PropertyTreeNode<Key, Data>
    {
    public:

        using Element = PropertyTreeElement<Key, Data>;
        using Node = PropertyTreeNode<Key, Data>;
        using Path = typename PropertyTreePathOf<Key>::Type;
        using Sequence = PropertyTreeSequence;
        using Value = PropertyTreeValue<Key, Data>;

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
        template <template<class, class> class NodeType, typename = std::enable_if_t<std::is_base_of_v<Node, NodeType<Key, Data>>>>
        NodeType<Key, Data>* Add(const NodeType<Key, Data>& node)
        {
            std::size_t index = GetSize();
            Key keyIndex = TranslatorCast<Key>(index);
            std::unique_ptr<Node> clone = static_cast<const Node&>(node).Clone(this, keyIndex);
            Node* inserted = mChildren.Emplace(std::move(clone)).get();
            return static_cast<NodeType<Key, Data>*>(inserted);
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
        T Get(const Path& path, const Tr& translator = Tr()) const
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
        T Get(std::size_t index, const Path& path, const Tr& translator = Tr()) const
        {
            return At(index)->template GetValue<T, Tr>(path, translator);
        }

        /**
         * \brief 
         * \param path 
         * \return 
         */
        Node& GetChild(const Path& path) const
        {
            Path p(path);
            std::size_t index = p.template Reduce<std::size_t>();
            if (index >= mChildren.GetSize())
                throw std::runtime_error("");

            const Node* child = mChildren[index].get();
            if (child->GetType() == Node::Type::Element)
                return static_cast<const PropertyTreeElement<Key, Data>*>(child)->GetChild(p);

            if (child->GetType() == Node::Type::Sequence)
                return static_cast<const PropertyTreeSequence*>(child)->GetChild(p);

            throw std::runtime_error("");
        }
        /**
         * \brief
         * \param path
         * \return
         */
        Element& GetElement(const Path& path) const
        {
            Node& node = GetChild(path);
            if (node.GetType() != Node::Type::Element)
                throw std::runtime_error("Path target node is not an element");

            return static_cast<Element&>(node);
        }

        /**
         * \brief
         * \param path
         * \return
         */
        Sequence& GetSequence(const Path& path) const
        {
            Node& node = GetChild(path);
            if (node.GetType() != Node::Type::Sequence)
                throw std::runtime_error("Path target node is not a sequence");

            return static_cast<Sequence&>(node);
        }

        /**
         * \brief
         * \param path
         * \return
         */
        Value& GetValue(const Path& path) const
        {
            Node& node = GetChild(path);
            if (node.GetType() != Node::Type::Value)
                throw std::runtime_error("Path target node is not a value");

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
        std::size_t GetSize() const
        {
            return mChildren.GetSize();
        }

        /**
         * \brief
         * \param index
         * \return
         */
        Node& At(std::size_t index)
        {
            return *mChildren.At(index);
        }

        /**
         * \brief
         * \param index
         * \return
         */
        const Node& At(std::size_t index) const
        {
            return *mChildren.At(index);
        }

        /**
         * \brief
         * \param index
         * \return
         */
        Element& ElementAt(std::size_t index)
        {
            Node& child = At(index);
            if (child.GetType() != Node::Type::Element)
                throw std::runtime_error("");

            return static_cast<Element&>(child);
        }

	    /**
         * \brief 
         * \param index 
         * \return 
         */
        const Element& ElementAt(std::size_t index) const
        {
            const Node& child = At(index);
            if (child.GetType() != Node::Type::Element)
                throw std::runtime_error("");

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
        PropertyTreeSequence(Node* parent, const Key& name) :
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
        std::unique_ptr<Node> Clone(Node* parent, const Key& name) const override
        {
            std::unique_ptr<PropertyTreeSequence<Key, Data>> clone(new PropertyTreeSequence<Key, Data>(parent, name)); /// TODO: find a way to use std::make_unique
            for (const std::unique_ptr<Node>& child : mChildren)
                clone->Add(*child);

            return clone;
        }

    private:

        Array<std::unique_ptr<Node>> mChildren;
    };
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREESEQUENCE_HPP
