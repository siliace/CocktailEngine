#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREENODE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREENODE_HPP

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>

namespace Ck
{
    /**
     * \brief
     */
    template <typename Data>
    class PropertyTreeNode
    {
    public:
    
        /**
         * \brief
         */
        enum class Type
        {
            /**
             * \brief
             */
            Element,

            /**
             * \brief
             */
            Value,

            /**
             * \brief
             */
            Sequence,

            /**
             * \brief
             */
            Custom,
        };

        /**
         * \brief Default constructor
         */
        PropertyTreeNode() :
            mParent(nullptr),
            mPreviousSibling(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Default destructor
         */
        virtual ~PropertyTreeNode() = default;

        /**
         * \brief 
         * \param other 
         */
        PropertyTreeNode(const PropertyTreeNode& other) = delete;

        /**
         * \brief 
         * \param other 
         */
        PropertyTreeNode(PropertyTreeNode&& other) noexcept = default;

        /**
    	 * \brief 
    	 * \param other 
    	 * \return 
    	 */
    	PropertyTreeNode& operator=(const PropertyTreeNode& other) = delete;

        /**
    	 * \brief 
    	 * \param other 
    	 * \return 
    	 */
    	PropertyTreeNode& operator=(PropertyTreeNode&& other) noexcept = default;

        /**
         * \brief 
         * \return 
         */
        UniquePtr<PropertyTreeNode<Data>> DropNextSibling()
        {
            // Disconnect ourselves form the next sibling
            if (mNextSibling)
				mNextSibling->mPreviousSibling = nullptr;

            return std::move(mNextSibling);
        }

        /**
         * \brief
         * \return
         */
        const String& GetName() const
        {
            return mName;
        }

        /**
         * \brief
         * \return
         */
        virtual PropertyTreeNode<Data>* GetPreviousSibling()
        {
            return mPreviousSibling;
        }

        /**
         * \brief
         * \return
         */
        virtual const PropertyTreeNode<Data>* GetPreviousSibling() const
        {
            return mPreviousSibling;
        }

        /**
         * \brief
         * \return
         */
        virtual PropertyTreeNode<Data>* GetNextSibling()
        {
            return mNextSibling.Get();
        }

        /**
         * \brief
         * \return
         */
        virtual const PropertyTreeNode<Data>* GetNextSibling() const
        {
            return mNextSibling.Get();
        }

        /**
		 * \brief
		 * \param name
		 * \param node
		 * \return
		 */
        PropertyTreeNode<Data>* SetNextSibling(const String& name, const PropertyTreeNode<Data>& node)
        {
            return SetNextSibling(node.Clone(mParent, name));
        }

        /**
         * \brief
         * \param node
         * \return
         */
        PropertyTreeNode<Data>* SetNextSibling(UniquePtr<PropertyTreeNode<Data>> node)
        {
            if (mNextSibling)
            {
                mNextSibling->mPreviousSibling = node.Get();
                if (node)
					node->mNextSibling = std::move(mNextSibling);
            }

            if (node)
				node->mPreviousSibling = this;

            mNextSibling = std::move(node);

            return mNextSibling.Get();
        }

        /**
         * \brief
         * \return
         */
        virtual PropertyTreeNode<Data>* GetParent()
        {
            return mParent;
        }

        /**
         * \brief
         * \return
         */
        virtual const PropertyTreeNode<Data>* GetParent() const
        {
            return mParent;
        }

        /**
         * \brief
         * \return
         */
        virtual Type GetType() const = 0;

    protected:

        template <typename>
        friend class PropertyTreeElement;

        template <typename>
        friend class PropertyTreeSequence;

        template <typename>
        friend class PropertyTreeValue;

        /**
         * \brief Constructor
         * \param parent The parent node
         * \param name The name of this node into \p parent node
         */
        PropertyTreeNode(PropertyTreeNode<Data>* parent, const String& name) :
            mParent(parent),
            mName(name),
            mPreviousSibling(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief
         * \param parent
         * \param name
         * \return
         */
        virtual UniquePtr<PropertyTreeNode<Data>> Clone(PropertyTreeNode<Data>* parent, const String& name) const = 0;

        PropertyTreeNode<Data>* mParent;
        String mName;
        PropertyTreeNode<Data>* mPreviousSibling;
        UniquePtr<PropertyTreeNode<Data>> mNextSibling;
    };
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREENODE_HPP
