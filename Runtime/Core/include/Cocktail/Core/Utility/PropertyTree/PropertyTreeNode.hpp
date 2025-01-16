#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREENODE_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREENODE_HPP

#include <memory>

namespace Ck
{
    /**
     * \brief
     */
    template <typename Key, typename Data>
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
        std::unique_ptr<PropertyTreeNode<Key, Data>> DropNextSibling()
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
        const Key& GetName() const
        {
            return mName;
        }

        /**
         * \brief
         * \return
         */
        virtual PropertyTreeNode<Key, Data>* GetPreviousSibling()
        {
            return mPreviousSibling;
        }

        /**
         * \brief
         * \return
         */
        virtual const PropertyTreeNode<Key, Data>* GetPreviousSibling() const
        {
            return mPreviousSibling;
        }

        /**
         * \brief
         * \return
         */
        virtual PropertyTreeNode<Key, Data>* GetNextSibling()
        {
            return mNextSibling.get();
        }

        /**
         * \brief
         * \return
         */
        virtual const PropertyTreeNode<Key, Data>* GetNextSibling() const
        {
            return mNextSibling.get();
        }

        /**
		 * \brief
		 * \param name
		 * \param node
		 * \return
		 */
        PropertyTreeNode<Key, Data>* SetNextSibling(const Key& name, const PropertyTreeNode<Key, Data>& node)
        {
            return SetNextSibling(node.Clone(mParent, name));
        }

        /**
         * \brief
         * \param node
         * \return
         */
        PropertyTreeNode<Key, Data>* SetNextSibling(std::unique_ptr<PropertyTreeNode<Key, Data>> node)
        {
            if (mNextSibling)
            {
                mNextSibling->mPreviousSibling = node.get();
                if (node)
					node->mNextSibling = std::move(mNextSibling);
            }

            if (node)
				node->mPreviousSibling = this;

            mNextSibling = std::move(node);

            return mNextSibling.get();
        }

        /**
         * \brief
         * \return
         */
        virtual PropertyTreeNode<Key, Data>* GetParent()
        {
            return mParent;
        }

        /**
         * \brief
         * \return
         */
        virtual const PropertyTreeNode<Key, Data>* GetParent() const
        {
            return mParent;
        }

        /**
         * \brief
         * \return
         */
        virtual Type GetType() const = 0;

    protected:

        template <typename, typename>
        friend class PropertyTreeElement;

        template <typename, typename>
        friend class PropertyTreeSequence;

        template <typename, typename>
        friend class PropertyTreeValue;

        /**
         * \brief Constructor
         * \param parent The parent node
         * \param name The name of this node into \p parent node
         */
        PropertyTreeNode(PropertyTreeNode<Key, Data>* parent, const Key& name) :
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
        virtual std::unique_ptr<PropertyTreeNode<Key, Data>> Clone(PropertyTreeNode<Key, Data>* parent, const Key& name) const = 0;

    private:

        PropertyTreeNode<Key, Data>* mParent;
        Key mName;
        PropertyTreeNode<Key, Data>* mPreviousSibling;
        std::unique_ptr<PropertyTreeNode<Key, Data>> mNextSibling;
    };
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREENODE_HPP
