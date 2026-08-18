#ifndef COCKTAILENGINE_CORE_UTILITY_DETAIL_REDBLACKTREE_HPP
#define COCKTAILENGINE_CORE_UTILITY_DETAIL_REDBLACKTREE_HPP

#include <CocktailEngine/Core/Memory/Allocator/AllocatorUtils.hpp>
#include <CocktailEngine/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

namespace Ck::Detail
{
    template <typename, typename>
    class RedBlackTreeIterator;

    template <typename, typename>
    class RedBlackTreeConstIterator;

    /**
     * \brief Colour carried by every node of a red-black tree
     *
     * A missing child is treated as black, which is what makes the black-height
     * invariant work without storing explicit leaf sentinels.
     */
    enum class TreeNodeColor
    {
        /**
         * \brief A red node may not have a red child
         */
        Red,

        /**
         * \brief Every path from a node to its leaves holds the same number of black nodes
         */
        Black,
    };

    /**
     * \brief Low-level red-black tree implementation shared by TreeMap and TreeSet
     *
     * This class owns the node graph and implements all structural operations:
     * ordered insertion, lookup, removal, rebalancing and in-order navigation. It
     * is parameterized by a \p TEntryPolicy that controls what is stored per entry
     * and how the lookup key is extracted from a stored entry, exactly like
     * \c HashTableBase does for the hashed containers.
     *
     * \par Entry Policy contract
     * \p TEntryPolicy must provide:
     * \code
     * // The type stored in each node
     * using EntryType = ...;
     *
     * // The type used to look up an entry
     * using KeyType = ...;
     *
     * // Extract the lookup key from a stored entry
     * static const KeyType& GetKey(const EntryType&);
     *
     * // Update the non-key portion of an existing entry (no-op for sets)
     * template <typename... TArgs>
     * static void ReplaceValue(EntryType&, TArgs&&...);
     * \endcode
     *
     * \par Ordering
     * Keys are ordered by \p TComparator, which must be a strict weak ordering
     * (the same contract as \c std::less). Two keys \c a and \c b are considered
     * equivalent when neither \c comparator(a, b) nor \c comparator(b, a) holds,
     * so no separate equality predicate is needed.
     *
     * \par Complexity
     * Lookup, insertion and removal are O(log n) in the worst case; in-order
     * traversal of the whole tree is O(n). The tree height is bounded by
     * 2 * log2(n + 1), which is what the colour invariants buy over a plain
     * binary search tree.
     *
     * \par Ownership and iterator stability
     * Each node owns its children through an allocator-aware \c UniquePtr, so
     * releasing the root releases the whole tree. Removal splices nodes rather
     * than moving entries between them, therefore references, pointers and
     * iterators to entries other than the removed one stay valid across an
     * insertion or a removal. Only the removed entry is invalidated.
     *
     * \par Thread safety
     * This class is not thread-safe. External synchronisation is required when
     * concurrent reads and writes are needed.
     *
     * \tparam TEntryPolicy Policy describing the entry layout and key extraction
     * \tparam TComparator  Strict weak ordering over \c KeyType
     * \tparam TAllocator   Allocator used to allocate nodes
     */
    template <typename TEntryPolicy, typename TComparator, typename TAllocator>
    class RedBlackTree
    {
    public:

        /**
         * \brief The type stored in each node, as defined by \p TEntryPolicy
         */
        using EntryType = typename TEntryPolicy::EntryType;

        /**
         * \brief The type used to look up entries, as defined by \p TEntryPolicy
         */
        using KeyType = typename TEntryPolicy::KeyType;

        /**
         * \brief Allocator type used by this tree
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for sizes and counts
         */
        using SizeType = typename TAllocator::SizeType;

    protected:

        template <typename, typename>
        friend class RedBlackTreeIterator;

        template <typename, typename>
        friend class RedBlackTreeConstIterator;

        struct Node;

        /**
         * \brief Owning handle over a node
         *
         * The deleter carries the allocator that produced the node, so a subtree
         * can be released without reaching back to the owning tree.
         */
        using NodeHandle = UniquePtr<Node, AllocatorAwareDeleter<Node, TAllocator>>;

        // ---------------------------------------------------------------
        // Internal node
        // ---------------------------------------------------------------

        /**
         * \brief Internal node type holding one entry and its position in the tree
         *
         * A \c Node inherits from \c EntryType so that the stored entry and the
         * tree links live in a single allocation. The parent link is a borrowed
         * pointer while both child links are owning handles, which makes the
         * node graph a strict ownership tree.
         */
        struct Node : EntryType
        {
            /**
             * \brief Forwarding constructor
             *
             * Constructs the \c EntryType portion by forwarding \p args. A fresh
             * node always starts red and detached: the insertion path links it and
             * the rebalancing pass recolours it.
             *
             * \param args Arguments forwarded to the \c EntryType constructor
             */
            template <typename... TArgs>
            explicit Node(TArgs&&... args) :
                EntryType(Forward<TArgs>(args)...),
                Parent(nullptr),
                Color(TreeNodeColor::Red)
            {
                /// Nothing
            }

            /**
             * \brief Borrowed pointer to the parent node, \c nullptr for the root
             */
            Node* Parent;

            /**
             * \brief Owned left child, holding the keys that order before this one
             */
            NodeHandle Left;

            /**
             * \brief Owned right child, holding the keys that order after this one
             */
            NodeHandle Right;

            /**
             * \brief Colour used to keep the tree balanced
             */
            TreeNodeColor Color;
        };

        /**
         * \brief Allocator rebound to the node type
         */
        using NodeAllocatorType = typename TAllocator::template ForType<Node>;

        // ---------------------------------------------------------------
        // Constructors / assignment
        // ---------------------------------------------------------------

        /**
         * \brief Constructs an empty tree
         *
         * \param allocator Allocator used to produce nodes
         */
        explicit RedBlackTree(const NodeAllocatorType& allocator = NodeAllocatorType()) :
            mSize(0),
            mAllocator(AllocatorUtils::CopyPropagateAllocator(allocator))
        {
            /// Nothing
        }

        /**
         * \brief Copy constructor
         *
         * The shape and the colouring of \p other are cloned as they are, so no
         * rebalancing is needed and the copy is O(n).
         *
         * \param other Tree to copy from
         */
        RedBlackTree(const RedBlackTree& other) :
            mSize(other.mSize),
            mComparator(other.mComparator),
            mAllocator(AllocatorUtils::CopyPropagateAllocator(other.mAllocator))
        {
            mRoot = CloneSubtree(other.mRoot.Get(), nullptr);
        }

        /**
         * \brief Move constructor
         *
         * Ownership of the node graph is transferred whenever the allocator allows
         * it. When it does not — an inline allocator owns storage that cannot be
         * handed over — the entries are moved one by one instead.
         * \p other is left empty after the operation.
         *
         * \param other Tree to move from
         */
        RedBlackTree(RedBlackTree&& other) noexcept :
            mSize(0),
            mComparator(Move(other.mComparator)),
            mAllocator(AllocatorUtils::MovePropagateAllocator(other.mAllocator))
        {
            AdoptNodes(other);
        }

        /**
         * \brief Destructor
         *
         * Releasing the root releases the whole node graph.
         */
        ~RedBlackTree() = default;

        /**
         * \brief Copy assignment operator
         *
         * Clears this tree, then clones all entries from \p other.
         *
         * \param other Tree to copy from
         *
         * \return Reference to \c *this
         */
        RedBlackTree& operator=(const RedBlackTree& other)
        {
            if (this != &other)
            {
                Clear();

                if constexpr (NodeAllocatorType::PropagateOnContainerCopy)
                    mAllocator = other.mAllocator;

                mComparator = other.mComparator;
                mRoot = CloneSubtree(other.mRoot.Get(), nullptr);
                mSize = other.mSize;
            }

            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * \p other is left empty after the operation.
         *
         * \param other Tree to move from
         *
         * \return Reference to \c *this
         */
        RedBlackTree& operator=(RedBlackTree&& other) noexcept
        {
            if (this != &other)
            {
                Clear();

                mComparator = Move(other.mComparator);
                if constexpr (NodeAllocatorType::PropagateOnContainerMove)
                    mAllocator = Move(other.mAllocator);

                AdoptNodes(other);
            }

            return *this;
        }

        // ---------------------------------------------------------------
        // Core operations
        // ---------------------------------------------------------------

        /**
         * \brief Insert a new entry or replace the value of an existing one
         *
         * The insertion point is located by descending from the root with
         * \c TComparator. If a node with an equivalent key already exists its
         * value is updated via \c TEntryPolicy::ReplaceValue; otherwise a new red
         * node is linked in and the tree is rebalanced.
         *
         * \tparam TLookupKey Type of the key (may be a reference to avoid copying)
         * \tparam TArgs      Types forwarded to \c EntryType on insertion or to
         *                    \c TEntryPolicy::ReplaceValue on update
         *
         * \param lookupKey Key used to locate the insertion point
         * \param args      Constructor / replacement arguments
         *
         * \return \c true if a new entry was inserted, \c false if an existing
         *         entry was updated
         */
        template <typename TLookupKey, typename... TArgs>
        bool PutEntry(TLookupKey&& lookupKey, TArgs&&... args)
        {
            const KeyType& canonicalKey = static_cast<const KeyType&>(lookupKey);

            Node* parent = nullptr;
            bool insertLeft = false;
            Node* current = mRoot.Get();
            while (current != nullptr)
            {
                parent = current;
                if (mComparator(canonicalKey, GetKey(*current)))
                {
                    insertLeft = true;
                    current = current->Left.Get();
                }
                else if (mComparator(GetKey(*current), canonicalKey))
                {
                    insertLeft = false;
                    current = current->Right.Get();
                }
                else
                {
                    TEntryPolicy::ReplaceValue(*current, Forward<TArgs>(args)...);
                    return false;
                }
            }

            LinkNode(parent, insertLeft, CreateNode(Forward<TArgs>(args)...));
            return true;
        }

        /**
         * \brief Insert a new entry only if the key is not already present
         *
         * Unlike \c PutEntry, this method never calls \c ReplaceValue. If a node
         * with an equivalent key already exists, the operation is a no-op and no
         * entry is constructed.
         *
         * \tparam TLookupKey Type of the key
         * \tparam TArgs      Types forwarded to \c EntryType on a new insertion
         *
         * \param lookupKey Key to check and potentially insert
         * \param args      Constructor arguments used only when inserting
         *
         * \return \c true if a new entry was inserted, \c false if the key was
         *         already present
         */
        template <typename TLookupKey, typename... TArgs>
        bool PutEntryIfMissing(TLookupKey&& lookupKey, TArgs&&... args)
        {
            const KeyType& canonicalKey = static_cast<const KeyType&>(lookupKey);

            Node* parent = nullptr;
            bool insertLeft = false;
            Node* current = mRoot.Get();
            while (current != nullptr)
            {
                parent = current;
                if (mComparator(canonicalKey, GetKey(*current)))
                {
                    insertLeft = true;
                    current = current->Left.Get();
                }
                else if (mComparator(GetKey(*current), canonicalKey))
                {
                    insertLeft = false;
                    current = current->Right.Get();
                }
                else
                {
                    return false;
                }
            }

            LinkNode(parent, insertLeft, CreateNode(Forward<TArgs>(args)...));
            return true;
        }

        /**
         * \brief Insert a new entry at an already located position
         *
         * This is the back half of \c PutEntry, exposed so that a caller that has
         * just established the absence of \p lookupKey does not have to descend
         * the tree a second time.
         *
         * \tparam TLookupKey Type of the key
         * \tparam TArgs      Types forwarded to \c EntryType
         *
         * \param lookupKey Key of the entry being inserted
         * \param args      Constructor arguments
         *
         * \return Reference to the newly inserted node
         *
         * \pre No entry with a key equivalent to \p lookupKey exists in the tree.
         */
        template <typename TLookupKey, typename... TArgs>
        Node& EmplaceEntry(TLookupKey&& lookupKey, TArgs&&... args)
        {
            const KeyType& canonicalKey = static_cast<const KeyType&>(lookupKey);

            Node* parent = nullptr;
            bool insertLeft = false;
            Node* current = mRoot.Get();
            while (current != nullptr)
            {
                parent = current;
                if (mComparator(canonicalKey, GetKey(*current)))
                {
                    insertLeft = true;
                    current = current->Left.Get();
                }
                else
                {
                    assert(mComparator(GetKey(*current), canonicalKey) && "EmplaceEntry called with an already present key");
                    insertLeft = false;
                    current = current->Right.Get();
                }
            }

            NodeHandle node = CreateNode(Forward<TArgs>(args)...);
            Node& inserted = *node;
            LinkNode(parent, insertLeft, Move(node));

            return inserted;
        }

        /**
         * \brief Find the internal \c Node holding \p key
         *
         * \param key Key to search for
         *
         * \return An \c Optional mutable reference to the matching \c Node,
         *         or empty if not found
         */
        Optional<Node&> FindNode(const KeyType& key)
        {
            Node* node = LocateNode(key);
            return node != nullptr ? Optional<Node&>::Of(*node) : Optional<Node&>::Empty();
        }

        /**
         * \brief Find the internal \c Node holding \p key (const overload)
         *
         * \param key Key to search for
         *
         * \return An \c Optional const reference to the matching \c Node,
         *         or empty if not found
         */
        Optional<const Node&> FindNode(const KeyType& key) const
        {
            const Node* node = LocateNode(key);
            return node != nullptr ? Optional<const Node&>::Of(*node) : Optional<const Node&>::Empty();
        }

        /**
         * \brief Remove the entry whose key is equivalent to \p key
         *
         * \param key Key of the entry to remove
         *
         * \return \c true if an entry was found and removed, \c false otherwise
         */
        bool RemoveEntry(const KeyType& key)
        {
            Node* node = LocateNode(key);
            if (node == nullptr)
                return false;

            RemoveNode(node);
            return true;
        }

        /**
         * \brief Unlink and destroy a node, then restore the colour invariants
         *
         * A node with two children is replaced by its in-order successor, which is
         * spliced into its position instead of having its entry moved. Every node
         * other than \p node therefore keeps its address, and outstanding
         * iterators keep pointing at the entries they pointed at.
         *
         * \param node Node to remove, which must belong to this tree
         */
        void RemoveNode(Node* node)
        {
            assert(node != nullptr);

            Node* fixupNode = nullptr;
            Node* fixupParent = nullptr;
            bool fixupLeft = false;
            TreeNodeColor removedColor;

            if (node->Left == nullptr || node->Right == nullptr)
            {
                // At most one child: the child, possibly none, takes the node's slot.
                removedColor = node->Color;
                fixupParent = node->Parent;
                fixupLeft = IsLeftChild(node);

                NodeHandle child = node->Left != nullptr ? Move(node->Left) : Move(node->Right);
                fixupNode = child.Get();

                NodeHandle removed = Detach(node);
                Attach(fixupParent, fixupLeft, Move(child));
            }
            else
            {
                // Two children: the in-order successor leaves its own position and
                // takes over the node's, keeping the node's colour so that only the
                // successor's former colour has to be accounted for.
                Node* successor = MinimumNode(node->Right.Get());
                Node* successorParent = successor->Parent;
                const bool successorLeft = IsLeftChild(successor);
                removedColor = successor->Color;

                NodeHandle successorRight = Move(successor->Right);
                fixupNode = successorRight.Get();

                NodeHandle successorOwned = Detach(successor);

                if (successorParent == node)
                {
                    // The successor is the node's own right child: it stays where it
                    // is relative to its former right subtree.
                    fixupParent = successor;
                    fixupLeft = false;
                }
                else
                {
                    fixupParent = successorParent;
                    fixupLeft = successorLeft;
                    Attach(successorParent, successorLeft, Move(successorRight));
                }

                Node* parent = node->Parent;
                const bool nodeLeft = IsLeftChild(node);
                NodeHandle removed = Detach(node);

                successor->Left = Move(node->Left);
                if (successor->Left != nullptr)
                    successor->Left->Parent = successor;

                successor->Right = successorParent == node ? Move(successorRight) : Move(node->Right);
                if (successor->Right != nullptr)
                    successor->Right->Parent = successor;

                successor->Color = node->Color;
                Attach(parent, nodeLeft, Move(successorOwned));
            }

            --mSize;

            // Removing a red node cannot change any black-height, so only the loss
            // of a black node has to be repaired.
            if (removedColor == TreeNodeColor::Black)
                RemoveFixup(fixupNode, fixupParent, fixupLeft);
        }

        /**
         * \brief Remove all entries from the tree
         */
        void Clear()
        {
            mRoot.Reset();
            mSize = 0;
        }

        /**
         * \brief Returns the number of entries currently stored in the tree
         */
        SizeType GetSize() const
        {
            return mSize;
        }

        /**
         * \brief Returns \c true if the tree contains no entries
         */
        bool IsEmpty() const
        {
            return mSize == 0;
        }

        // ---------------------------------------------------------------
        // Ordered navigation
        // ---------------------------------------------------------------

        /**
         * \brief Returns the node holding the smallest key, or \c nullptr when empty
         */
        Node* GetFirstNode() const
        {
            return MinimumNode(mRoot.Get());
        }

        /**
         * \brief Returns the node holding the largest key, or \c nullptr when empty
         */
        Node* GetLastNode() const
        {
            return MaximumNode(mRoot.Get());
        }

        /**
         * \brief Returns the in-order successor of \p node
         *
         * \param node Node to step from
         *
         * \return The next node in key order, or \c nullptr past the last entry
         */
        static Node* NextNode(Node* node)
        {
            if (node == nullptr)
                return nullptr;

            if (node->Right != nullptr)
                return MinimumNode(node->Right.Get());

            Node* parent = node->Parent;
            while (parent != nullptr && node == parent->Right.Get())
            {
                node = parent;
                parent = parent->Parent;
            }

            return parent;
        }

        /**
         * \brief Returns the in-order predecessor of \p node
         *
         * \param node Node to step from
         *
         * \return The previous node in key order, or \c nullptr before the first entry
         */
        static Node* PreviousNode(Node* node)
        {
            if (node == nullptr)
                return nullptr;

            if (node->Left != nullptr)
                return MaximumNode(node->Left.Get());

            Node* parent = node->Parent;
            while (parent != nullptr && node == parent->Left.Get())
            {
                node = parent;
                parent = parent->Parent;
            }

            return parent;
        }

        /**
         * \brief Returns the first node whose key does not order before \p key
         *
         * \param key Key to position on
         *
         * \return The matching node, or \c nullptr when every key orders before \p key
         */
        Node* LowerBoundNode(const KeyType& key) const
        {
            Node* result = nullptr;
            Node* current = mRoot.Get();
            while (current != nullptr)
            {
                if (mComparator(GetKey(*current), key))
                {
                    current = current->Right.Get();
                }
                else
                {
                    result = current;
                    current = current->Left.Get();
                }
            }

            return result;
        }

        /**
         * \brief Returns the first node whose key orders after \p key
         *
         * \param key Key to position past
         *
         * \return The matching node, or \c nullptr when no key orders after \p key
         */
        Node* UpperBoundNode(const KeyType& key) const
        {
            Node* result = nullptr;
            Node* current = mRoot.Get();
            while (current != nullptr)
            {
                if (mComparator(key, GetKey(*current)))
                {
                    result = current;
                    current = current->Left.Get();
                }
                else
                {
                    current = current->Right.Get();
                }
            }

            return result;
        }

        /**
         * \brief Returns the node holding the largest key that does not order after \p key
         *
         * \param key Key to floor
         *
         * \return The matching node, or \c nullptr when every key orders after \p key
         */
        Node* FloorNode(const KeyType& key) const
        {
            Node* result = nullptr;
            Node* current = mRoot.Get();
            while (current != nullptr)
            {
                if (mComparator(key, GetKey(*current)))
                {
                    current = current->Left.Get();
                }
                else
                {
                    result = current;
                    current = current->Right.Get();
                }
            }

            return result;
        }

        /**
         * \brief Extracts the lookup key of a node
         *
         * \param node Node to read
         *
         * \return Const reference to the node's key
         */
        static const KeyType& GetKey(const Node& node)
        {
            return TEntryPolicy::GetKey(node);
        }

        /**
         * \brief Returns the comparator used to order keys
         */
        const TComparator& GetComparator() const
        {
            return mComparator;
        }

    private:

        /**
         * \brief Returns the colour of a possibly missing node
         *
         * Missing children count as black, which keeps the black-height rules
         * free of leaf sentinels.
         *
         * \param node Node to read, may be \c nullptr
         *
         * \return The node's colour, or \c Black when \p node is \c nullptr
         */
        static TreeNodeColor GetColor(const Node* node)
        {
            return node != nullptr ? node->Color : TreeNodeColor::Black;
        }

        /**
         * \brief Tells whether \p node is the left child of its parent
         *
         * \param node Node to test
         *
         * \return \c false for the root, otherwise whether the node sits on the left
         */
        static bool IsLeftChild(const Node* node)
        {
            const Node* parent = node->Parent;
            return parent != nullptr && parent->Left.Get() == node;
        }

        /**
         * \brief Returns the leftmost node of a subtree
         *
         * \param node Subtree root, may be \c nullptr
         *
         * \return The node holding the smallest key of the subtree
         */
        static Node* MinimumNode(Node* node)
        {
            while (node != nullptr && node->Left != nullptr)
                node = node->Left.Get();

            return node;
        }

        /**
         * \brief Returns the rightmost node of a subtree
         *
         * \param node Subtree root, may be \c nullptr
         *
         * \return The node holding the largest key of the subtree
         */
        static Node* MaximumNode(Node* node)
        {
            while (node != nullptr && node->Right != nullptr)
                node = node->Right.Get();

            return node;
        }

        /**
         * \brief Locates the node holding \p key
         *
         * \param key Key to search for
         *
         * \return The matching node, or \c nullptr when the key is absent
         */
        Node* LocateNode(const KeyType& key) const
        {
            Node* current = mRoot.Get();
            while (current != nullptr)
            {
                if (mComparator(key, GetKey(*current)))
                    current = current->Left.Get();
                else if (mComparator(GetKey(*current), key))
                    current = current->Right.Get();
                else
                    return current;
            }

            return nullptr;
        }

        /**
         * \brief Allocates and constructs a detached node
         *
         * \tparam TArgs Types forwarded to the \c EntryType constructor
         *
         * \param args Arguments forwarded to the \c EntryType constructor
         *
         * \return Owning handle over the new node
         */
        template <typename... TArgs>
        NodeHandle CreateNode(TArgs&&... args)
        {
            return MakeUniqueWithAllocator<Node, TAllocator>(mAllocator, Forward<TArgs>(args)...);
        }

        /**
         * \brief Returns the handle that owns \p node
         *
         * The owning handle is the root handle for the root, and the matching
         * child handle of the parent otherwise.
         *
         * \param node Node to look up
         *
         * \return Reference to the handle currently holding \p node
         */
        NodeHandle& GetSlot(Node* node)
        {
            Node* parent = node->Parent;
            if (parent == nullptr)
                return mRoot;

            return parent->Left.Get() == node ? parent->Left : parent->Right;
        }

        /**
         * \brief Takes ownership of \p node out of the tree
         *
         * The slot that held \p node is left empty and the node keeps its links,
         * which the caller is expected to rewrite.
         *
         * \param node Node to detach
         *
         * \return Owning handle over \p node
         */
        NodeHandle Detach(Node* node)
        {
            return Move(GetSlot(node));
        }

        /**
         * \brief Places \p child into an empty slot and fixes its parent link
         *
         * \param parent Parent to attach under, \c nullptr for the root slot
         * \param left   Whether to attach as the left child
         * \param child  Node to attach, may be empty
         *
         * \pre The target slot must be empty, otherwise the node it holds would be
         *      destroyed instead of unlinked.
         */
        void Attach(Node* parent, bool left, NodeHandle child)
        {
            Node* attached = child.Get();

            if (parent == nullptr)
            {
                assert(mRoot == nullptr);
                mRoot = Move(child);
            }
            else if (left)
            {
                assert(parent->Left == nullptr);
                parent->Left = Move(child);
            }
            else
            {
                assert(parent->Right == nullptr);
                parent->Right = Move(child);
            }

            if (attached != nullptr)
                attached->Parent = parent;
        }

        /**
         * \brief Links a freshly created node into the tree and rebalances
         *
         * \param parent     Parent to link under, \c nullptr when the tree is empty
         * \param insertLeft Whether the node belongs to the parent's left slot
         * \param node       Node to link
         */
        void LinkNode(Node* parent, bool insertLeft, NodeHandle node)
        {
            Node* inserted = node.Get();
            Attach(parent, insertLeft, Move(node));
            ++mSize;

            InsertFixup(inserted);
        }

        /**
         * \brief Rotates \p node to the left, promoting its right child
         *
         * \param node Node to rotate, which must have a right child
         */
        void RotateLeft(Node* node)
        {
            assert(node != nullptr && node->Right != nullptr);

            Node* parent = node->Parent;
            NodeHandle& slot = GetSlot(node);
            NodeHandle owned = Move(slot);

            NodeHandle pivotOwned = Move(node->Right);
            Node* pivot = pivotOwned.Get();

            node->Right = Move(pivot->Left);
            if (node->Right != nullptr)
                node->Right->Parent = node;

            node->Parent = pivot;
            pivot->Left = Move(owned);
            pivot->Parent = parent;

            slot = Move(pivotOwned);
        }

        /**
         * \brief Rotates \p node to the right, promoting its left child
         *
         * \param node Node to rotate, which must have a left child
         */
        void RotateRight(Node* node)
        {
            assert(node != nullptr && node->Left != nullptr);

            Node* parent = node->Parent;
            NodeHandle& slot = GetSlot(node);
            NodeHandle owned = Move(slot);

            NodeHandle pivotOwned = Move(node->Left);
            Node* pivot = pivotOwned.Get();

            node->Left = Move(pivot->Right);
            if (node->Left != nullptr)
                node->Left->Parent = node;

            node->Parent = pivot;
            pivot->Right = Move(owned);
            pivot->Parent = parent;

            slot = Move(pivotOwned);
        }

        /**
         * \brief Restores the colour invariants after an insertion
         *
         * A freshly linked node is red, so the only invariant that can be broken
         * is the one forbidding a red node under a red parent. The violation is
         * either recoloured away — when the uncle is red — or rotated away, which
         * terminates the pass.
         *
         * \param node Newly linked node
         */
        void InsertFixup(Node* node)
        {
            while (node->Parent != nullptr && node->Parent->Color == TreeNodeColor::Red)
            {
                Node* parent = node->Parent;
                Node* grandParent = parent->Parent;

                // A red parent is never the root, so the grandparent exists.
                assert(grandParent != nullptr);

                if (parent == grandParent->Left.Get())
                {
                    Node* uncle = grandParent->Right.Get();
                    if (GetColor(uncle) == TreeNodeColor::Red)
                    {
                        parent->Color = TreeNodeColor::Black;
                        uncle->Color = TreeNodeColor::Black;
                        grandParent->Color = TreeNodeColor::Red;
                        node = grandParent;
                    }
                    else
                    {
                        if (node == parent->Right.Get())
                        {
                            node = parent;
                            RotateLeft(node);
                            parent = node->Parent;
                            grandParent = parent->Parent;
                        }

                        parent->Color = TreeNodeColor::Black;
                        grandParent->Color = TreeNodeColor::Red;
                        RotateRight(grandParent);
                    }
                }
                else
                {
                    Node* uncle = grandParent->Left.Get();
                    if (GetColor(uncle) == TreeNodeColor::Red)
                    {
                        parent->Color = TreeNodeColor::Black;
                        uncle->Color = TreeNodeColor::Black;
                        grandParent->Color = TreeNodeColor::Red;
                        node = grandParent;
                    }
                    else
                    {
                        if (node == parent->Left.Get())
                        {
                            node = parent;
                            RotateRight(node);
                            parent = node->Parent;
                            grandParent = parent->Parent;
                        }

                        parent->Color = TreeNodeColor::Black;
                        grandParent->Color = TreeNodeColor::Red;
                        RotateLeft(grandParent);
                    }
                }
            }

            mRoot->Color = TreeNodeColor::Black;
        }

        /**
         * \brief Restores the colour invariants after a black node was spliced out
         *
         * \p node carries one unit of missing black height. The pass either borrows
         * black from a sibling, which ends it, or pushes the deficit one level up
         * towards the root, where it vanishes.
         *
         * \param node   Node that took the removed node's place, may be \c nullptr
         * \param parent Parent of \p node after the splice, \c nullptr at the root
         * \param isLeft Whether \p node sits in the left slot of \p parent
         */
        void RemoveFixup(Node* node, Node* parent, bool isLeft)
        {
            while (parent != nullptr && GetColor(node) == TreeNodeColor::Black)
            {
                if (isLeft)
                {
                    Node* sibling = parent->Right.Get();

                    // A black deficit on one side implies the other side carries at
                    // least one black node, hence a sibling.
                    assert(sibling != nullptr);
                    if (sibling == nullptr)
                        break;

                    if (sibling->Color == TreeNodeColor::Red)
                    {
                        sibling->Color = TreeNodeColor::Black;
                        parent->Color = TreeNodeColor::Red;
                        RotateLeft(parent);
                        sibling = parent->Right.Get();

                        assert(sibling != nullptr);
                        if (sibling == nullptr)
                            break;
                    }

                    if (GetColor(sibling->Left.Get()) == TreeNodeColor::Black && GetColor(sibling->Right.Get()) == TreeNodeColor::Black)
                    {
                        // Nothing to borrow: drop one black from the sibling and move
                        // the deficit to the parent.
                        sibling->Color = TreeNodeColor::Red;
                        node = parent;
                        parent = node->Parent;
                        isLeft = IsLeftChild(node);
                    }
                    else
                    {
                        if (GetColor(sibling->Right.Get()) == TreeNodeColor::Black)
                        {
                            assert(sibling->Left != nullptr);
                            sibling->Left->Color = TreeNodeColor::Black;
                            sibling->Color = TreeNodeColor::Red;
                            RotateRight(sibling);
                            sibling = parent->Right.Get();
                        }

                        sibling->Color = parent->Color;
                        parent->Color = TreeNodeColor::Black;
                        if (sibling->Right != nullptr)
                            sibling->Right->Color = TreeNodeColor::Black;

                        RotateLeft(parent);
                        node = mRoot.Get();
                        parent = nullptr;
                    }
                }
                else
                {
                    Node* sibling = parent->Left.Get();

                    assert(sibling != nullptr);
                    if (sibling == nullptr)
                        break;

                    if (sibling->Color == TreeNodeColor::Red)
                    {
                        sibling->Color = TreeNodeColor::Black;
                        parent->Color = TreeNodeColor::Red;
                        RotateRight(parent);
                        sibling = parent->Left.Get();

                        assert(sibling != nullptr);
                        if (sibling == nullptr)
                            break;
                    }

                    if (GetColor(sibling->Left.Get()) == TreeNodeColor::Black && GetColor(sibling->Right.Get()) == TreeNodeColor::Black)
                    {
                        sibling->Color = TreeNodeColor::Red;
                        node = parent;
                        parent = node->Parent;
                        isLeft = IsLeftChild(node);
                    }
                    else
                    {
                        if (GetColor(sibling->Left.Get()) == TreeNodeColor::Black)
                        {
                            assert(sibling->Right != nullptr);
                            sibling->Right->Color = TreeNodeColor::Black;
                            sibling->Color = TreeNodeColor::Red;
                            RotateLeft(sibling);
                            sibling = parent->Left.Get();
                        }

                        sibling->Color = parent->Color;
                        parent->Color = TreeNodeColor::Black;
                        if (sibling->Left != nullptr)
                            sibling->Left->Color = TreeNodeColor::Black;

                        RotateRight(parent);
                        node = mRoot.Get();
                        parent = nullptr;
                    }
                }
            }

            if (node != nullptr)
                node->Color = TreeNodeColor::Black;
        }

        /**
         * \brief Clones a subtree, preserving both its shape and its colouring
         *
         * \param source Subtree to clone, may be \c nullptr
         * \param parent Parent to assign to the clone
         *
         * \return Owning handle over the cloned subtree
         */
        NodeHandle CloneSubtree(const Node* source, Node* parent)
        {
            if (source == nullptr)
                return NodeHandle();

            NodeHandle node = CreateNode(static_cast<const EntryType&>(*source));
            Node* clone = node.Get();
            clone->Parent = parent;
            clone->Color = source->Color;
            clone->Left = CloneSubtree(source->Left.Get(), clone);
            clone->Right = CloneSubtree(source->Right.Get(), clone);

            return node;
        }

        /**
         * \brief Takes over the node graph of \p other, or moves its entries over
         *
         * Stealing the graph is only sound when both trees allocate from
         * interchangeable allocators. An inline allocator owns storage tied to its
         * container, so in that case the entries are moved one by one.
         *
         * \param other Tree to take from, left empty afterwards
         */
        void AdoptNodes(RedBlackTree& other)
        {
            if constexpr (NodeAllocatorType::PropagateOnContainerMove || NodeAllocatorType::AlwaysEqual)
            {
                mRoot = Move(other.mRoot);
                mSize = std::exchange(other.mSize, static_cast<SizeType>(0));
            }
            else
            {
                if (mAllocator == other.mAllocator)
                {
                    mRoot = Move(other.mRoot);
                    mSize = std::exchange(other.mSize, static_cast<SizeType>(0));
                }
                else
                {
                    // The source holds unique keys, so no entry can collide here and
                    // the policy never has to replace a value.
                    for (Node* node = other.GetFirstNode(); node != nullptr; node = NextNode(node))
                        PutEntryIfMissing(GetKey(*node), Move(static_cast<EntryType&>(*node)));
                }

                other.Clear();
            }
        }

        /**
         * \brief Returns the height of a subtree
         *
         * \param node Subtree root, may be \c nullptr
         *
         * \return Number of nodes on the subtree's longest downward path
         */
        static SizeType SubtreeHeight(const Node* node)
        {
            if (node == nullptr)
                return 0;

            return static_cast<SizeType>(1) + std::max(SubtreeHeight(node->Left.Get()), SubtreeHeight(node->Right.Get()));
        }

        /**
         * \brief Recursively verifies the invariants of a subtree
         *
         * \param node        Subtree root, may be \c nullptr
         * \param parent      Expected parent of \p node
         * \param count       Accumulator incremented once per visited node
         * \param blackHeight Receives the black height of the subtree
         *
         * \return \c true when the subtree is well-formed
         */
        static bool CheckSubtree(const Node* node, const Node* parent, SizeType& count, SizeType& blackHeight)
        {
            if (node == nullptr)
            {
                blackHeight = 1;
                return true;
            }

            if (node->Parent != parent)
                return false;

            if (node->Color == TreeNodeColor::Red &&
                (GetColor(node->Left.Get()) == TreeNodeColor::Red || GetColor(node->Right.Get()) == TreeNodeColor::Red))
                return false;

            SizeType leftHeight = 0;
            SizeType rightHeight = 0;
            if (!CheckSubtree(node->Left.Get(), node, count, leftHeight))
                return false;
            if (!CheckSubtree(node->Right.Get(), node, count, rightHeight))
                return false;

            if (leftHeight != rightHeight)
                return false;

            ++count;
            blackHeight = leftHeight + (node->Color == TreeNodeColor::Black ? 1 : 0);

            return true;
        }

        NodeHandle mRoot; ///< Owning handle over the root node
        SizeType mSize; ///< Number of entries currently stored
        TComparator mComparator; ///< Strict weak ordering over keys
        NodeAllocatorType mAllocator; ///< Allocator producing nodes
    };

    // -------------------------------------------------------------------
    // Iterator helpers shared by TreeMap and TreeSet
    // -------------------------------------------------------------------

    /**
     * \brief Mutable bidirectional iterator over a \c RedBlackTree
     *
     * Entries are visited in ascending key order. A past-the-end iterator holds no
     * node and compares equal to any other past-the-end iterator over the same
     * tree; stepping it backwards lands on the last entry.
     *
     * \par Invalidation
     * Only the removal of the entry an iterator points at invalidates it. Any other
     * insertion or removal leaves it usable, because nodes keep their address.
     *
     * \tparam TTree      The concrete tree type being iterated
     * \tparam TValueType The type exposed through \c GetValue() and \c operator*
     *                    (\c MapEntry<K,V> for TreeMap, \c EntryType for TreeSet)
     */
    template <typename TTree, typename TValueType>
    class RedBlackTreeIterator
    {
        using NodeType = typename TTree::Node;

    public:

        /**
         * \brief Type used for step counts
         */
        using SizeType = typename TTree::SizeType;

        /**
         * \brief Construct an iterator positioned on the smallest key
         *
         * \param owner The tree to iterate
         */
        explicit RedBlackTreeIterator(TTree& owner) :
            mOwner(&owner),
            mNode(owner.GetFirstNode())
        {
            /// Nothing
        }

        /**
         * \brief Construct an iterator positioned on a given node
         *
         * \param owner The tree to iterate
         * \param node  Node to point at, \c nullptr for a past-the-end iterator
         */
        RedBlackTreeIterator(TTree& owner, NodeType* node) :
            mOwner(&owner),
            mNode(node)
        {
            /// Nothing
        }

        /**
         * \brief Move the iterator backward by \p count entries
         *
         * Stepping back from a past-the-end iterator lands on the last entry.
         *
         * \param count Number of entries to step backward
         *
         * \return Reference to \c *this
         */
        RedBlackTreeIterator& Rewind(SizeType count = 1)
        {
            for (SizeType i = 0; i < count; i++)
                mNode = mNode != nullptr ? TTree::PreviousNode(mNode) : mOwner->GetLastNode();

            return *this;
        }

        /**
         * \brief Move the iterator forward by \p count entries
         *
         * \param count Number of entries to step forward
         *
         * \return Reference to \c *this
         */
        RedBlackTreeIterator& Advance(SizeType count = 1)
        {
            for (SizeType i = 0; i < count; i++)
                mNode = TTree::NextNode(mNode);

            return *this;
        }

        /**
         * \brief Returns a copy positioned on the previous entry
         */
        RedBlackTreeIterator Previous() const
        {
            RedBlackTreeIterator previous(*this);
            previous.Rewind();

            return previous;
        }

        /**
         * \brief Returns a copy positioned on the next entry
         */
        RedBlackTreeIterator Next() const
        {
            RedBlackTreeIterator next(*this);
            next.Advance();

            return next;
        }

        /**
         * \brief Check whether the iterator points to a valid entry
         *
         * \return \c true if the iterator is positioned on an entry, \c false
         *         once all entries have been visited
         */
        bool IsValid() const
        {
            return mNode != nullptr;
        }

        /**
         * \brief Returns the node the iterator points at
         *
         * This is an implementation detail exposed so that the owning container
         * can turn a position back into a node without walking the tree again.
         *
         * \return The current node, \c nullptr past the end
         */
        NodeType* GetNode() const
        {
            return mNode;
        }

        /**
         * \brief Get a mutable reference to the current entry
         */
        TValueType& GetValue()
        {
            return static_cast<TValueType&>(*mNode);
        }

        /**
         * \brief Get a const reference to the current entry
         */
        const TValueType& GetValue() const
        {
            return static_cast<const TValueType&>(*mNode);
        }

        /**
         * \brief Dereferences the iterator
         *
         * \return Reference to the current entry
         */
        TValueType& operator*() const
        {
            return static_cast<TValueType&>(*mNode);
        }

        /**
         * \brief Member-access through the iterator
         *
         * \return Pointer to the current entry
         */
        TValueType* operator->() const
        {
            return &static_cast<TValueType&>(*mNode);
        }

        /**
         * \brief Pre-increment — advance to the next entry
         *
         * \return Reference to \c *this after advancing
         */
        RedBlackTreeIterator& operator++()
        {
            return Advance();
        }

        /**
         * \brief Pre-decrement — retreat to the previous entry
         *
         * \return Reference to \c *this after retreating
         */
        RedBlackTreeIterator& operator--()
        {
            return Rewind();
        }

        /**
         * \brief Equality comparison
         *
         * Two iterators are equal when they point to the same node, which makes
         * all past-the-end iterators over a tree compare equal.
         */
        bool operator==(const RedBlackTreeIterator& other) const
        {
            return mNode == other.mNode;
        }

        /**
         * \brief Inequality comparison
         */
        bool operator!=(const RedBlackTreeIterator& other) const
        {
            return !(*this == other);
        }

    private:

        template <typename, typename>
        friend class RedBlackTreeConstIterator;

        TTree* mOwner; ///< Pointer to the iterated tree
        NodeType* mNode; ///< Node currently pointed at, \c nullptr past the end
    };

    /**
     * \brief Read-only bidirectional iterator over a \c RedBlackTree
     *
     * Semantically identical to \c RedBlackTreeIterator but provides only const
     * access to the stored entries. Can be implicitly constructed from a mutable
     * iterator.
     *
     * \tparam TTree      The concrete tree type being iterated
     * \tparam TValueType The type exposed through \c GetValue() and \c operator*
     */
    template <typename TTree, typename TValueType>
    class RedBlackTreeConstIterator
    {
        using NodeType = typename TTree::Node;

    public:

        /**
         * \brief Type used for step counts
         */
        using SizeType = typename TTree::SizeType;

        /**
         * \brief Construct a const iterator positioned on the smallest key
         *
         * \param owner The tree to iterate
         */
        explicit RedBlackTreeConstIterator(const TTree& owner) :
            mOwner(&owner),
            mNode(owner.GetFirstNode())
        {
            /// Nothing
        }

        /**
         * \brief Construct a const iterator positioned on a given node
         *
         * \param owner The tree to iterate
         * \param node  Node to point at, \c nullptr for a past-the-end iterator
         */
        RedBlackTreeConstIterator(const TTree& owner, NodeType* node) :
            mOwner(&owner),
            mNode(node)
        {
            /// Nothing
        }

        /**
         * \brief Implicit conversion from a mutable iterator
         *
         * \param other The mutable iterator to copy state from
         */
        RedBlackTreeConstIterator(const RedBlackTreeIterator<TTree, TValueType>& other) :
            mOwner(other.mOwner),
            mNode(other.mNode)
        {
            /// Nothing
        }

        /**
         * \brief Move the iterator backward by \p count entries
         *
         * \param count Number of entries to step backward
         *
         * \return Reference to \c *this
         */
        RedBlackTreeConstIterator& Rewind(SizeType count = 1)
        {
            for (SizeType i = 0; i < count; i++)
                mNode = mNode != nullptr ? TTree::PreviousNode(mNode) : mOwner->GetLastNode();

            return *this;
        }

        /**
         * \brief Move the iterator forward by \p count entries
         *
         * \param count Number of entries to step forward
         *
         * \return Reference to \c *this
         */
        RedBlackTreeConstIterator& Advance(SizeType count = 1)
        {
            for (SizeType i = 0; i < count; i++)
                mNode = TTree::NextNode(mNode);

            return *this;
        }

        /**
         * \brief Returns a copy positioned on the previous entry
         */
        RedBlackTreeConstIterator Previous() const
        {
            RedBlackTreeConstIterator previous(*this);
            previous.Rewind();

            return previous;
        }

        /**
         * \brief Returns a copy positioned on the next entry
         */
        RedBlackTreeConstIterator Next() const
        {
            RedBlackTreeConstIterator next(*this);
            next.Advance();

            return next;
        }

        /**
         * \brief Check whether the iterator points to a valid entry
         *
         * \return \c true if positioned on an entry, \c false when exhausted
         */
        bool IsValid() const
        {
            return mNode != nullptr;
        }

        /**
         * \brief Returns the node the iterator points at
         *
         * This is an implementation detail exposed so that the owning container
         * can turn a position back into a node without walking the tree again.
         *
         * \return The current node, \c nullptr past the end
         */
        NodeType* GetNode() const
        {
            return mNode;
        }

        /**
         * \brief Get a const reference to the current entry
         */
        const TValueType& GetValue() const
        {
            return static_cast<const TValueType&>(*mNode);
        }

        /**
         * \brief Dereferences the iterator
         *
         * \return Const reference to the current entry
         */
        const TValueType& operator*() const
        {
            return static_cast<const TValueType&>(*mNode);
        }

        /**
         * \brief Member-access through the iterator
         *
         * \return Const pointer to the current entry
         */
        const TValueType* operator->() const
        {
            return &static_cast<const TValueType&>(*mNode);
        }

        /**
         * \brief Pre-increment — advance to the next entry
         *
         * \return Reference to \c *this after advancing
         */
        RedBlackTreeConstIterator& operator++()
        {
            return Advance();
        }

        /**
         * \brief Pre-decrement — retreat to the previous entry
         *
         * \return Reference to \c *this after retreating
         */
        RedBlackTreeConstIterator& operator--()
        {
            return Rewind();
        }

        /**
         * \brief Equality comparison
         */
        bool operator==(const RedBlackTreeConstIterator& other) const
        {
            return mNode == other.mNode;
        }

        /**
         * \brief Inequality comparison
         */
        bool operator!=(const RedBlackTreeConstIterator& other) const
        {
            return !(*this == other);
        }

    private:

        const TTree* mOwner; ///< Pointer to the iterated tree (const)
        NodeType* mNode; ///< Node currently pointed at, \c nullptr past the end
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_DETAIL_REDBLACKTREE_HPP
