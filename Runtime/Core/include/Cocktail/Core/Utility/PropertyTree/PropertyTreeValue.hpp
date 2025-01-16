#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEVALUE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEVALUE_HPP

#include <Cocktail/Core/Utility/TranslatorCast.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeNode.hpp>

namespace Ck
{
    /** 
     * \brief
     */
    template <typename Key, typename Data>
    class PropertyTreeValue : public PropertyTreeNode<Key, Data>
    {
    public:

        /**
         * \brief Default constructor
         */
        template <typename = std::enable_if_t<std::is_default_constructible_v<Data>>>
        PropertyTreeValue() :
            mValue{}
        {
            /// Nothing
        }

        /** 
         * \brief Constructor
         * \tparam T The type of the initial value to hold
         * \tparam Tr The type of the translator to use to convert T type to Data type
         * \param value The initial value to hold
         * \param translator The translator
         */
        template <typename T, typename Tr = typename TranslatorBetween<Data, T>::Type>
        explicit PropertyTreeValue(const T& value, const Tr& translator = Tr()) :
			mValue(translator(value).Get())
        {
            /// Nothing
        }

        /**
         * \brief 
         * \tparam T 
         * \tparam Tr The type of the translator to use to convert T type to Data type
         * \param translator The translator
         * \return 
         */
        template <typename T, typename Tr = typename TranslatorBetween<Data, T>::Type>
        T As(const Tr& translator = Tr()) const
        {
            return translator(mValue).Get();
        }

        /**
         * \brief
         * \return
         */
        typename PropertyTreeNode<Key, Data>::Type GetType() const override
        {
            return PropertyTreeNode<Key, Data>::Type::Value;
        }

    protected:

        /**
         * \brief Constructor
         * \param parent The parent of the node into the tree
         * \param name The name of the node into the tree
         * \param value The initial value to hold
         */
        PropertyTreeValue(PropertyTreeNode<Key, Data>* parent, const Key& name, const Data& value) :
            PropertyTreeNode<Key, Data>(parent, name),
            mValue(value)
        {
            /// Nothing
        }

        /**
         * \brief
         * \param parent
         * \param name
         * \return
         */
        std::unique_ptr<PropertyTreeNode<Key, Data>> Clone(PropertyTreeNode<Key, Data>* parent, const Key& name) const override
        {
            return std::unique_ptr<PropertyTreeNode<Key, Data>>(
                new PropertyTreeValue(parent, name, mValue)
            );
        }

    private:

        /** 
         * \brief The value held by the this node of the tree
         */
        Data mValue;
    };
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEVALUE_HPP
