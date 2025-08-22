#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEELEMENT_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEELEMENT_HPP

#include <stdexcept>

#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeNode.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreePath.hpp>

namespace Ck
{
    template <typename Key, typename Data>
    class PropertyTreeSequence;

    template <typename Key, typename Data>
    class PropertyTreeValue;

    /**
     * \brief Specialization of PropertyTreeNode implementing recursive tree node
     */
    template <typename Key, typename Data>
    class PropertyTreeElement : public PropertyTreeNode<Key, Data>
    {
    public:

        using Element = PropertyTreeElement;
        using Node = PropertyTreeNode<Key, Data>;
        using Path = typename PropertyTreePathOf<Key>::Type;
        using Sequence = PropertyTreeSequence<Key, Data>;
        using Value = PropertyTreeValue<Key, Data>;

        /**
         * \brief Default constructor
         */
        PropertyTreeElement() :
			mLastChild(nullptr)
        {
	        /// Nothing
        }

        /**
         * \brief 
         * \param other 
         */
        PropertyTreeElement(const PropertyTreeElement& other) = delete;

        /**
         * \brief 
         * \param other 
         */
        PropertyTreeElement(PropertyTreeElement&& other) noexcept = default;

        /**
         * \brief 
         * \param other 
         * \return 
         */
        PropertyTreeElement& operator=(const PropertyTreeElement& other) = delete;

        /**
    	 * \brief 
    	 * \param other 
    	 * \return 
    	 */
    	PropertyTreeElement& operator=(PropertyTreeElement&& other) noexcept = default;

        /**
         * \brief Insert a new node at the end of this element
         * \param name The name of the \p node to be inserted
         * \param node The node to be inserted
         * \return A pointer to the inserted node�
         */
        template <template<class, class> class NodeType, typename = std::enable_if_t<std::is_base_of_v<Node, NodeType<Key, Data>>>>
        NodeType<Key, Data>* Insert(const Key& name, const NodeType<Key, Data>& node)
        {
            Remove(name);
            
            if (!mFirstChild)
                return SetFirstChild(name, node);

            return InsertAfter(mLastChild, name, node);
        }

        /**
         * \brief Insert a new node at the beginning of this element
         * \param name The name of the \p node to be inserted
         * \param node The node to be inserted
         * \return A pointer to the inserted node�
         */
        template <template<class, class> class NodeType, typename = std::enable_if_t<std::is_base_of_v<Node, NodeType<Key, Data>>>>
        NodeType<Key, Data>* FrontInsert(const Key& name, const NodeType<Key, Data>& node)
        {
            Remove(name);

            if (!mFirstChild)
                return SetFirstChild(name, node);

            return InsertBefore<NodeType>(mFirstChild.get(), name, node);
        }

        /**
         * \brief
         * \param where
         * \param name
         * \param node
         * \return
         */
        template <template<class, class> class NodeType, typename = std::enable_if_t<std::is_base_of_v<Node, NodeType<Key, Data>>>>
        NodeType<Key, Data>* InsertBefore(Node* where, const Key& name, const NodeType<Key, Data>& node)
        {
            if (this != where->GetParent())
                throw std::invalid_argument("Cannot insert property node after a node that does not belongs to the same element");

            Remove(name);

            Node* inserted;
            if (Node* before = where->GetPreviousSibling(); before != nullptr) 
            {
                inserted = before->SetNextSibling(name, node);
            }
        	else
            {
                std::unique_ptr<Node> firstChild = static_cast<const Node&>(node).Clone(this, name);
                firstChild->SetNextSibling(std::move(mFirstChild));

                mFirstChild = std::move(firstChild);
                inserted = mFirstChild.get();
            }

            return static_cast<NodeType<Key, Data>*>(inserted);
        }

        /**
         * \brief
         * \param where
         * \param name
         * \param node
         * \return
         */
        template <template<class, class> class NodeType, typename = std::enable_if_t<std::is_base_of_v<Node, NodeType<Key, Data>>>>
        NodeType<Key, Data>* InsertAfter(Node* where, const Key& name, const NodeType<Key, Data>& node)
        {
            if (this != where->GetParent())
                throw std::invalid_argument("Cannot insert property node after a node that does not belongs to the same element");

        	Remove(name);

            Node* inserted = where->SetNextSibling(name, node);

            if (where == mLastChild)
                mLastChild = inserted;

			return static_cast<NodeType<Key, Data>*>(inserted);
        }

        /**
         * \brief 
         * \param key 
         */
        void Remove(const Key& key)
        {
            for (Node* current = mFirstChild.get(); current; current = current->GetNextSibling())
            {
                if (current->GetName() != key)
                    continue;

                Remove(current);
                return;
            }
        }

        /**
         * \brief 
         * \param node 
         */
        void Remove(const Node* node)
        {
            if (this != node->GetParent())
                throw std::invalid_argument("Cannot insert property node after a node that does not belongs to the same element");

            if (mFirstChild.get() == node)
            {
                mFirstChild = mFirstChild->DropNextSibling();
                if (node == mLastChild)
                    mLastChild = mFirstChild.get();
            }
            else if (mLastChild == node)
            {
                Node* beforeLastChild = mLastChild->GetPreviousSibling();
            	beforeLastChild->SetNextSibling(nullptr);

                mLastChild = beforeLastChild;
            }
            else
            {
                for (Node* current = mFirstChild.get(); current != nullptr; current = current->GetNextSibling())
                {
                    if (current != node)
                        continue;

                    if (Node* previous = current->GetPreviousSibling())
                        previous->SetNextSibling(current->DropNextSibling());

                    break;
                }
            }
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
         * \param childName 
         * \return 
         */
        bool HasChild(const Key& childName) const
        {
	        for (auto child = mFirstChild.get(); child; child = child->GetNextSibling())
	        {
                if (child->GetName() == childName)
                    return true;
	        }

            return false;
        }

        /**
         * \brief 
         * \param path 
         * \return 
         */
        Node& GetChild(const Path& path) const
        {
            Path p(path);
            const bool isTerminal = p.IsSingle();
            Key key = p.Reduce();

            for (Node* child = GetFirstChild(); child; child = child->GetNextSibling())
            {
	            if (child->GetName() == key)
	            {
                    if (isTerminal)
                        return *child;

	            	if (child->GetType() == Node::Type::Element)
                        return static_cast<Element*>(child)->GetChild(p);

                    if (child->GetType() == Node::Type::Sequence)
                        return static_cast<PropertyTreeSequence<Key, Data>*>(child)->GetChild(p);

                    throw InvalidPropertyPathException("Child {} in node {} is terminal", key, this->mName);
	            }
            }

            throw InvalidPropertyPathException("Element {} does not has child {}", this->mName, key);
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
        Node* GetFirstChild() const
        {
            return mFirstChild.get();
        }

        /**
         * \brief 
         * \return 
         */
        Node* GetLastChild() const
        {
            return mLastChild;
        }

        /**
         * \brief
         * \return
         */
        typename Node::Type GetType() const override
        {
            return PropertyTreeNode<Key, Data>::Type::Element;
        }

    protected:

        template <typename,typename>
        friend class PropertyTree;

        /**
         * \brief
         * \param parent
         * \param name
         */
        PropertyTreeElement(Node* parent, const Key& name) :
            Node(parent, name),
            mLastChild(nullptr)
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
            std::unique_ptr<PropertyTreeElement<Key, Data>> clone(new PropertyTreeElement<Key, Data>(parent, name)); /// TODO: find a way to use std::make_unique
            for (auto* child = mFirstChild.get(); child != nullptr; child = child->GetNextSibling())
                clone->Insert(child->GetName(), *child);

            return clone;
        }
        
    private:

        template <template<class, class> class NodeType, typename = std::enable_if_t<std::is_base_of_v<Node, NodeType<Key, Data>>>>
        NodeType<Key, Data>* SetFirstChild(const Key& name, const NodeType<Key, Data>& node)
        {
            mFirstChild = static_cast<const Node&>(node).Clone(this, name);
            mLastChild = mFirstChild.get();

            return static_cast<NodeType<Key, Data>*>(mLastChild);
        }

        std::unique_ptr<Node> mFirstChild;
        Node* mLastChild;
    };
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEELEMENT_HPP
