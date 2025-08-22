#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_HPP

#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeCustomNode.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeElement.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreePath.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeSequence.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeValue.hpp>

namespace Ck
{
	/**
     * \brief 
     * \tparam Key 
     * \tparam Data 
     */
    template <typename Key, typename Data>
    class PropertyTree
    {
    public:

        using CustomNode = PropertyTreeCustomNode<Key, Data>;
        using Element = PropertyTreeElement<Key, Data>;
        using Node = PropertyTreeNode<Key, Data>;
        using Path = typename PropertyTreePathOf<Key>::Type;
    	using Sequence = PropertyTreeSequence<Key, Data>;
        using Value = PropertyTreeValue<Key, Data>;

        /**
         * \brief 
         */
        PropertyTree() = default;

        /**
         * \brief 
         * \param other 
         */
        PropertyTree(const PropertyTree& other) = delete;

        /**
         * \brief 
         * \param other 
         */
        PropertyTree(PropertyTree&& other) noexcept = default;

        /**
         * \brief 
         * \param other 
         * \return 
         */
        PropertyTree& operator=(const PropertyTree& other) = delete;

        /**
    	 * \brief 
    	 * \param other 
    	 * \return 
    	 */
    	PropertyTree& operator=(PropertyTree&& other) noexcept = default;

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
         * \param path 
         * \return 
         */
        Node& GetChild(const Path& path) const
        {
            Path p(path);
            Key key = p.Reduce();

            if (mRoot->GetName() != key)
                throw InvalidPropertyPathException();

            return mRoot->GetChild(p);
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
                throw InvalidPropertyPathException("Path target node {} is not an element", path.ToString());

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
                throw InvalidPropertyPathException("Path target node {} is not a sequence", path.ToString());

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
                throw InvalidPropertyPathException("Path target node {} is not a value", path.ToString());

            return static_cast<Value&>(node);
        }

        /**
         * \brief Get the root element of the tree property
         * \return The root element
         */
        Element* GetRoot()
        {
            return mRoot.get();
        }

        /**
         * \brief Get the root element of the tree property
         * \return The root element
         */
        const Element* GetRoot() const
        {
            return mRoot.get();
        }

    protected:

        /**
         * \brief
         */
        void ResetRoot(const Key& name, const Element& rootElement)
        {
            mRoot = std::unique_ptr<Element>(
                static_cast<Element*>(
                    rootElement.Clone(nullptr, name).release()
                )
            );
        }

    private:

        std::unique_ptr<Element> mRoot;
    };

    using Properties = PropertyTree<std::string, std::string>;
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_HPP
