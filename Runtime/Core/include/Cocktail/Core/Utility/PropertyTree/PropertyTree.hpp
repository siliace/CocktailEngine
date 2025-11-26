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
     * \tparam Data
     */
    template <typename Data>
    class PropertyTree
    {
    public:

        using CustomNodeType = PropertyTreeCustomNode<Data>;
        using ElementType = PropertyTreeElement<Data>;
        using NodeType = PropertyTreeNode<Data>;
    	using SequenceType = PropertyTreeSequence<Data>;
        using ValueType = PropertyTreeValue<Data>;

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
    	T Get(const PropertyTreePath& path, const Tr& translator = Tr()) const
        {
            return GetValue(path).template As<T, Tr>(translator);
        }

        /**
         * \brief 
         * \param path 
         * \return 
         */
        NodeType& GetChild(const PropertyTreePath& path) const
        {
            PropertyTreePath p(path);
            String key = p.Reduce();

            if (mRoot->GetName() != key)
                throw InvalidPropertyPathException();

            return mRoot->GetChild(p);
        }

        /**
         * \brief 
         * \param path 
         * \return 
         */
        ElementType& GetElement(const PropertyTreePath& path) const
        {
            NodeType& node = GetChild(path);
            if (node.GetType() != NodeType::Type::Element)
                throw InvalidPropertyPathException(CK_TEXT("PropertyTreePath target node %s is not an element"), path.ToString());

        	return static_cast<ElementType&>(node);
        }

        /**
         * \brief 
         * \param path 
         * \return 
         */
        SequenceType& GetSequence(const PropertyTreePath& path) const
        {
            NodeType& node = GetChild(path);
            if (node.GetType() != NodeType::Type::Sequence)
                throw InvalidPropertyPathException(CK_TEXT("PropertyTreePath target node %s is not a sequence"), path.ToString());

            return static_cast<SequenceType&>(node);
        }

        /**
         * \brief 
         * \param path 
         * \return 
         */
        ValueType& GetValue(const PropertyTreePath& path) const
        {
            NodeType& node = GetChild(path);
            if (node.GetType() != NodeType::Type::Value)
                throw InvalidPropertyPathException(CK_TEXT("PropertyTreePath target node %s is not a value"), path.ToString());

            return static_cast<ValueType&>(node);
        }

        /**
         * \brief Get the root element of the tree property
         * \return The root element
         */
        ElementType* GetRoot()
        {
            return mRoot.Get();
        }

        /**
         * \brief Get the root element of the tree property
         * \return The root element
         */
        const ElementType* GetRoot() const
        {
            return mRoot.Get();
        }

    protected:

        /**
         * \brief
         */
        void ResetRoot(const String& name, const ElementType& rootElement)
        {
            mRoot = UniquePtr<ElementType>(
                static_cast<ElementType*>(
                    rootElement.Clone(nullptr, name).Release()
                )
            );
        }

    private:

        UniquePtr<ElementType> mRoot;
    };

    using Properties = PropertyTree<String>;
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_HPP
