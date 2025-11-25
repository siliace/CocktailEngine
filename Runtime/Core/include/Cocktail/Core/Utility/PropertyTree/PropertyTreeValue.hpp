#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEVALUE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEVALUE_HPP

#include <Cocktail/Core/Utility/TranslatorCast.hpp>
#include <Cocktail/Core/Utility/PropertyTree/PropertyTreeNode.hpp>

namespace Ck
{
    /** 
     * \brief
     */
    template <typename Data>
    class PropertyTreeValue : public PropertyTreeNode<Data>
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
        template <typename T, typename Tr = typename TranslatorBetween<T, Data>::Type>
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
        typename PropertyTreeNode<Data>::Type GetType() const override
        {
            return PropertyTreeNode<Data>::Type::Value;
        }

    protected:

        /**
         * \brief Constructor
         * \param parent The parent of the node into the tree
         * \param name The name of the node into the tree
         * \param value The initial value to hold
         */
        PropertyTreeValue(PropertyTreeNode<Data>* parent, const String& name, const Data& value) :
            PropertyTreeNode<Data>(parent, name),
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
        UniquePtr<PropertyTreeNode<Data>> Clone(PropertyTreeNode<Data>* parent, const String& name) const override
        {
            return UniquePtr<PropertyTreeNode<Data>>(
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
