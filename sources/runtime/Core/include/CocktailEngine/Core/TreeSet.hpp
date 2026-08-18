#ifndef COCKTAILENGINE_CORE_TREESET_HPP
#define COCKTAILENGINE_CORE_TREESET_HPP

#include <functional>
#include <initializer_list>
#include <utility>

#include <CocktailEngine/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <CocktailEngine/Core/Utility/Detail/RedBlackTree.hpp>
#include <CocktailEngine/Core/Utility/FunctionTraits.hpp>

namespace Ck
{
    namespace Detail
    {
        /**
         * \brief Entry policy where the element itself is the key
         *
         * A TreeSet stores unique elements; the key used for ordering and lookup
         * is the element itself. \c ReplaceValue is a no-op because the "value" is
         * the key — inserting a duplicate is simply ignored.
         *
         * \tparam TElement Type of the stored elements
         */
        template <typename TElement>
        struct TreeSetEntryPolicy
        {
            /**
             * \brief The type stored per entry
             *
             * The element is wrapped in a trivial struct so that \c RedBlackTree can
             * use the same \c EntryType(args...) construction pattern as
             * \c MapEntry, while the iterator still exposes a natural
             * \c operator* over the element.
             */
            struct EntryType
            {
                /**
                 * \brief The type used to look up this entry (the element itself)
                 */
                using KeyType = TElement;

                /**
                 * \brief Default constructor — value-initialises \c Element
                 */
                EntryType() = default;

                /**
                 * \brief Construct from a const lvalue element
                 *
                 * \param element Element to copy into this entry
                 */
                explicit EntryType(const TElement& element) :
                    Element(element)
                {
                    /// Nothing
                }

                /**
                 * \brief Construct from an rvalue element
                 *
                 * \param element Element to move into this entry
                 */
                explicit EntryType(TElement&& element) :
                    Element(Move(element))
                {
                    /// Nothing
                }

                /**
                 * \brief The stored element — also serves as the ordering key
                 */
                TElement Element;
            };

            /**
             * \brief The type used to look up entries (the element type itself)
             */
            using KeyType = TElement;

            /**
             * \brief Extract the lookup key from a stored entry
             *
             * For a set the key is the element itself.
             *
             * \param entry The entry to extract the key from
             *
             * \return Const reference to the stored element
             */
            static const KeyType& GetKey(const EntryType& entry)
            {
                return entry.Element;
            }

            /**
             * \brief No-op: sets do not have a separate value to replace
             */
            template <typename TOtherElement>
            static void ReplaceValue(EntryType& /*entry*/, TOtherElement&& /*element*/)
            {
                // Duplicate key — nothing to update
            }
        };
    }

    /**
     * \brief Ordered set storing unique elements in a red-black tree
     *
     * \c TreeSet keeps its elements sorted by \p TComparator at all times, which
     * is the difference that matters against \c HashSet: iteration is in ascending
     * element order rather than bucket order, and the container answers range
     * queries such as \c Floor, \c Ceiling or \c GetBounds. The price is
     * logarithmic rather than constant-time lookup.
     *
     * \par Complexity
     * \c Add, \c Contains and \c Remove are O(log n) in the worst case. The set
     * operations are O(n log n).
     *
     * \par Iterator stability
     * Elements live in individually allocated nodes, so inserting or removing an
     * element never invalidates references or iterators to the other elements.
     *
     * \par Thread safety
     * This class is not thread-safe. External synchronisation is required when
     * concurrent reads and writes are needed.
     *
     * \tparam TElement    Type of stored elements
     * \tparam TComparator Strict weak ordering over \c TElement
     * \tparam TAllocator  Allocator for internal node storage
     */
    template <typename TElement, typename TComparator = std::less<TElement>, typename TAllocator = HeapAllocator>
    class TreeSet : public Detail::RedBlackTree<Detail::TreeSetEntryPolicy<TElement>, TComparator, TAllocator>
    {
        using Base = Detail::RedBlackTree<Detail::TreeSetEntryPolicy<TElement>, TComparator, TAllocator>;
        using Node = typename Base::Node;
        using EntryType = typename Base::EntryType;

    public:

        /**
         * \brief Type of elements stored in the set
         */
        using ElementType = TElement;

        /**
         * \brief Allocator type used for internal storage
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for sizes and counts
         */
        using SizeType = typename TAllocator::SizeType;

        /**
         * \brief Mutable iterator: dereferences to \c const TElement&
         *
         * Elements of a set are logically immutable — rewriting an element in
         * place would break the ordering invariant — so even the mutable iterator
         * hands out const references. The type exists for range-for compatibility.
         */
        class Iterator : public Detail::RedBlackTreeIterator<Base, EntryType>
        {
            using Inner = Detail::RedBlackTreeIterator<Base, EntryType>;

        public:

            /**
             * \brief Construct an iterator positioned on the smallest element
             *
             * \param owner The set to iterate
             */
            explicit Iterator(TreeSet& owner) :
                Inner(static_cast<Base&>(owner))
            {
                /// Nothing
            }

            /**
             * \brief Construct an iterator positioned on a given node
             *
             * \param owner The set to iterate
             * \param node  Node to point at, \c nullptr for a past-the-end iterator
             */
            Iterator(TreeSet& owner, Node* node) :
                Inner(static_cast<Base&>(owner), node)
            {
                /// Nothing
            }

            /**
             * \brief Dereferences to the stored element
             *
             * \return Const reference to the current element
             */
            const TElement& operator*() const
            {
                return Inner::GetValue().Element;
            }

            /**
             * \brief Member-access through the iterator
             *
             * \return Const pointer to the current element
             */
            const TElement* operator->() const
            {
                return &Inner::GetValue().Element;
            }

            /**
             * \brief Get a const reference to the current element
             *
             * \return Const reference to the current element
             */
            const TElement& GetValue() const
            {
                return Inner::GetValue().Element;
            }

            /**
             * \brief Pre-increment — advance to the next element
             *
             * \return Reference to \c *this after advancing
             */
            Iterator& operator++()
            {
                Inner::operator++();
                return *this;
            }

            /**
             * \brief Pre-decrement — retreat to the previous element
             *
             * \return Reference to \c *this after retreating
             */
            Iterator& operator--()
            {
                Inner::operator--();
                return *this;
            }

            /**
             * \brief Equality comparison
             */
            bool operator==(const Iterator& other) const
            {
                return Inner::operator==(other);
            }

            /**
             * \brief Inequality comparison
             */
            bool operator!=(const Iterator& other) const
            {
                return !(*this == other);
            }
        };

        /**
         * \brief Read-only bidirectional iterator over a \c TreeSet
         *
         * Semantically identical to \c Iterator. Can be implicitly constructed
         * from a mutable \c Iterator.
         */
        class ConstIterator : public Detail::RedBlackTreeConstIterator<Base, EntryType>
        {
            using Inner = Detail::RedBlackTreeConstIterator<Base, EntryType>;

        public:

            /**
             * \brief Construct a const iterator positioned on the smallest element
             *
             * \param owner The set to iterate
             */
            explicit ConstIterator(const TreeSet& owner) :
                Inner(static_cast<const Base&>(owner))
            {
                /// Nothing
            }

            /**
             * \brief Construct a const iterator positioned on a given node
             *
             * \param owner The set to iterate
             * \param node  Node to point at, \c nullptr for a past-the-end iterator
             */
            ConstIterator(const TreeSet& owner, Node* node) :
                Inner(static_cast<const Base&>(owner), node)
            {
                /// Nothing
            }

            /**
             * \brief Implicit conversion from a mutable iterator
             *
             * \param other The mutable iterator to copy state from
             */
            ConstIterator(const Iterator& other) :
                Inner(other)
            {
                /// Nothing
            }

            /**
             * \brief Dereferences to the stored element
             *
             * \return Const reference to the current element
             */
            const TElement& operator*() const
            {
                return Inner::GetValue().Element;
            }

            /**
             * \brief Member-access through the iterator
             *
             * \return Const pointer to the current element
             */
            const TElement* operator->() const
            {
                return &Inner::GetValue().Element;
            }

            /**
             * \brief Get a const reference to the current element
             *
             * \return Const reference to the current element
             */
            const TElement& GetValue() const
            {
                return Inner::GetValue().Element;
            }

            /**
             * \brief Pre-increment — advance to the next element
             *
             * \return Reference to \c *this after advancing
             */
            ConstIterator& operator++()
            {
                Inner::operator++();
                return *this;
            }

            /**
             * \brief Pre-decrement — retreat to the previous element
             *
             * \return Reference to \c *this after retreating
             */
            ConstIterator& operator--()
            {
                Inner::operator--();
                return *this;
            }

            /**
             * \brief Equality comparison
             */
            bool operator==(const ConstIterator& other) const
            {
                return Inner::operator==(other);
            }

            /**
             * \brief Inequality comparison
             */
            bool operator!=(const ConstIterator& other) const
            {
                return !(*this == other);
            }
        };

        struct Bounds
        {
            Iterator Begin;
            Iterator End;
        };

        struct ConstBounds
        {
            Iterator Begin;
            Iterator End;
        };

        // -----------------------------------------------------------------------
        // Construction / assignment
        // -----------------------------------------------------------------------

        /**
         * \brief Constructs an empty set
         */
        TreeSet() :
            Base()
        {
            /// Nothing
        }

        /**
         * \brief Copy constructor
         *
         * Deep-copies all elements from \p other, reusing its tree shape.
         *
         * \param other Set to copy from
         */
        TreeSet(const TreeSet& other) :
            Base(other)
        {
            /// Nothing
        }

        /**
         * \brief Move constructor
         *
         * Transfers ownership of \p other's nodes.
         * \p other is left empty after the operation.
         *
         * \param other Set to move from
         */
        TreeSet(TreeSet&& other) noexcept :
            Base(Move(other))
        {
            /// Nothing
        }

        /**
         * \brief Constructs the set from an initializer list
         *
         * Duplicate values in \p values are silently ignored.
         *
         * \param values Initializer list of elements to insert
         */
        TreeSet(std::initializer_list<TElement> values) :
            Base()
        {
            for (const TElement& element : values)
                Add(element);
        }

        /**
         * \brief Copy assignment operator
         *
         * \param other Set to copy from
         *
         * \return Reference to \c *this
         */
        TreeSet& operator=(const TreeSet& other)
        {
            Base::operator=(other);
            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * \p other is left empty after the operation.
         *
         * \param other Set to move from
         *
         * \return Reference to \c *this
         */
        TreeSet& operator=(TreeSet&& other) noexcept
        {
            Base::operator=(Move(other));
            return *this;
        }

        // -----------------------------------------------------------------------
        // Insertion
        // -----------------------------------------------------------------------

        /**
         * \brief Insert an element into the set
         *
         * If an equivalent element is already present, this is a no-op.
         *
         * \param element The element to insert
         *
         * \return \c true if the element was inserted, \c false if it was already present
         */
        bool Add(const TElement& element)
        {
            return Base::PutEntryIfMissing(element, element);
        }

        /**
         * \brief Insert an element into the set using move semantics
         *
         * \param element The element to insert
         *
         * \return \c true if the element was inserted, \c false if it was already present
         */
        bool Add(TElement&& element)
        {
            // The key is needed to locate the insertion point before the value can
            // be moved, so bind it first and move only once the position is known.
            const TElement& key = element;
            return Base::PutEntryIfMissing(key, Move(element));
        }

        /**
         * \brief Construct an element in-place and insert it
         *
         * The element is constructed from \p args. If the resulting value is
         * already present in the set, the construction is discarded.
         *
         * \tparam TArgs Argument types forwarded to the element constructor
         *
         * \param args Arguments forwarded to the element constructor
         *
         * \return \c true if the element was inserted
         */
        template <typename... TArgs>
        bool Emplace(TArgs&&... args)
        {
            TElement element(Forward<TArgs>(args)...);
            return Add(Move(element));
        }

        // -----------------------------------------------------------------------
        // Lookup
        // -----------------------------------------------------------------------

        /**
         * \brief Check whether \p element exists in the set
         *
         * \param element Element to search for
         *
         * \return \c true if found, \c false otherwise
         */
        bool Contains(const TElement& element) const
        {
            return !Base::FindNode(element).IsEmpty();
        }

        /**
         * \brief Returns the smallest element of the set
         *
         * \return \c Optional const reference to the smallest element, empty when
         *         the set is empty
         */
        Optional<const TElement&> GetFirst() const
        {
            Node* node = Base::GetFirstNode();
            return node != nullptr ? Optional<const TElement&>::Of(node->Element) : Optional<const TElement&>::Empty();
        }

        /**
         * \brief Returns the largest element of the set
         *
         * \return \c Optional const reference to the largest element, empty when
         *         the set is empty
         */
        Optional<const TElement&> GetLast() const
        {
            Node* node = Base::GetLastNode();
            return node != nullptr ? Optional<const TElement&>::Of(node->Element) : Optional<const TElement&>::Empty();
        }

        /**
         * \brief Returns the largest element that does not order after \p element
         *
         * \param element Element to floor
         *
         * \return \c Optional const reference to the matching element, empty when
         *         every element orders after \p element
         */
        Optional<const TElement&> Floor(const TElement& element) const
        {
            Node* node = Base::FloorNode(element);
            return node != nullptr ? Optional<const TElement&>::Of(node->Element) : Optional<const TElement&>::Empty();
        }

        /**
         * \brief Returns the smallest element that does not order before \p element
         *
         * \param element Element to ceil
         *
         * \return \c Optional const reference to the matching element, empty when
         *         every element orders before \p element
         */
        Optional<const TElement&> Ceiling(const TElement& element) const
        {
            Node* node = Base::LowerBoundNode(element);
            return node != nullptr ? Optional<const TElement&>::Of(node->Element) : Optional<const TElement&>::Empty();
        }

        /**
         * \brief Returns an iterator on the first element that does not order before \p element
         *
         * \param element Element to position on
         *
         * \return Iterator on the matching element, or a past-the-end iterator
         */
        Iterator LowerBound(const TElement& element)
        {
            return Iterator(*this, Base::LowerBoundNode(element));
        }

        /**
         * \brief Returns a read-only iterator on the first element that does not
         *        order before \p element
         *
         * \param element Element to position on
         *
         * \return Const iterator on the matching element, or a past-the-end iterator
         */
        ConstIterator LowerBound(const TElement& element) const
        {
            return ConstIterator(*this, Base::LowerBoundNode(element));
        }

        /**
         * \brief Returns an iterator on the first element that orders after \p element
         *
         * \param element Element to position past
         *
         * \return Iterator on the matching element, or a past-the-end iterator
         */
        Iterator UpperBound(const TElement& element)
        {
            return Iterator(*this, Base::UpperBoundNode(element));
        }

        /**
         * \brief Returns a read-only iterator on the first element that orders
         *        after \p element
         *
         * \param element Element to position past
         *
         * \return Const iterator on the matching element, or a past-the-end iterator
         */
        ConstIterator UpperBound(const TElement& element) const
        {
            return ConstIterator(*this, Base::UpperBoundNode(element));
        }

        /**
         * \brief Returns the iterator pair delimiting the inclusive range
         *        [\p first, \p last]
         *
         * \param first Lower bound, included
         * \param last  Upper bound, included
         *
         * \return A half-open iterator range covering every element between
         *         \p first and \p last
         */
        Bounds GetBounds(const TElement& first, const TElement& last)
        {
            return { LowerBound(first), UpperBound(last) };
        }

        /**
         * \brief Returns the read-only iterator pair delimiting the inclusive
         *        range [\p first, \p last]
         *
         * \param first Lower bound, included
         * \param last  Upper bound, included
         *
         * \return A half-open iterator range covering every element between
         *         \p first and \p last
         */
        ConstBounds GetBounds(const TElement& first, const TElement& last) const
        {
            return { LowerBound(first), UpperBound(last) };
        }

        // -----------------------------------------------------------------------
        // Removal
        // -----------------------------------------------------------------------

        /**
         * \brief Remove \p element from the set
         *
         * \param element Element to remove
         *
         * \return \c true if the element was present and removed
         */
        bool Remove(const TElement& element)
        {
            return Base::RemoveEntry(element);
        }

        /**
         * \brief Remove the element an iterator points at
         *
         * \param position Iterator on the element to remove, which must be valid
         *
         * \return Iterator on the element that followed the removed one
         */
        Iterator Remove(const Iterator& position)
        {
            assert(position.IsValid());

            Node* node = position.GetNode();
            Node* next = Base::NextNode(node);
            Base::RemoveNode(node);

            return Iterator(*this, next);
        }

        /**
         * \brief Remove the smallest element of the set
         *
         * \return \c true if an element was removed, \c false when the set is empty
         */
        bool RemoveFirst()
        {
            Node* node = Base::GetFirstNode();
            if (node == nullptr)
                return false;

            Base::RemoveNode(node);
            return true;
        }

        /**
         * \brief Remove the largest element of the set
         *
         * \return \c true if an element was removed, \c false when the set is empty
         */
        bool RemoveLast()
        {
            Node* node = Base::GetLastNode();
            if (node == nullptr)
                return false;

            Base::RemoveNode(node);
            return true;
        }

        // -----------------------------------------------------------------------
        // Set operations
        // -----------------------------------------------------------------------

        /**
         * \brief Returns a new set holding the union of this set and \p other
         *
         * \param other Set to merge with
         *
         * \return New set containing every element of both operands
         */
        TreeSet Union(const TreeSet& other) const
        {
            TreeSet result(*this);
            for (ConstIterator it(other); it.IsValid(); it.Advance())
                result.Add(it.GetValue());

            return result;
        }

        /**
         * \brief Returns a new set holding the intersection of this set and \p other
         *
         * \param other Set to intersect with
         *
         * \return New set containing only the elements present in both operands
         */
        TreeSet Intersection(const TreeSet& other) const
        {
            TreeSet result;
            for (ConstIterator it(*this); it.IsValid(); it.Advance())
            {
                if (other.Contains(it.GetValue()))
                    result.Add(it.GetValue());
            }

            return result;
        }

        /**
         * \brief Returns a new set holding this set minus \p other
         *
         * \param other Set to subtract
         *
         * \return New set containing the elements of this set absent from \p other
         */
        TreeSet Difference(const TreeSet& other) const
        {
            TreeSet result;
            for (ConstIterator it(*this); it.IsValid(); it.Advance())
            {
                if (!other.Contains(it.GetValue()))
                    result.Add(it.GetValue());
            }

            return result;
        }

        /**
         * \brief Applies \p function to every element, in ascending order
         *
         * \tparam TFunction Callable with signature \c void(const TElement&)
         *
         * \param function The function to apply to each element
         *
         * \warning Do not insert or remove elements during iteration.
         */
        template <typename TFunction>
        void ForEach(TFunction function) const
        {
            static_assert(FunctionTraits<TFunction>::Arity == 1, "ForEach function must accept exactly 1 parameter");

            for (Node* node = Base::GetFirstNode(); node != nullptr; node = Base::NextNode(node))
                function(node->Element);
        }

        // -----------------------------------------------------------------------
        // Iteration
        // -----------------------------------------------------------------------

        /**
         * \brief Return a mutable iterator positioned on the smallest element
         *
         * \return Iterator at the beginning of the set
         */
        Iterator GetIterator()
        {
            return Iterator(*this);
        }

        /**
         * \brief Return a read-only iterator positioned on the smallest element
         *
         * \return Const iterator at the beginning of the set
         */
        ConstIterator GetIterator() const
        {
            return ConstIterator(*this);
        }

        /**
         * \brief Return a mutable iterator positioned on the largest element
         *
         * \return Iterator on the last element of the set
         */
        Iterator GetLastIterator()
        {
            return Iterator(*this, Base::GetLastNode());
        }

        /**
         * \brief Return a read-only iterator positioned on the largest element
         *
         * \return Const iterator on the last element of the set
         */
        ConstIterator GetLastIterator() const
        {
            return ConstIterator(*this, Base::GetLastNode());
        }

        /**
         * \brief Return a mutable past-the-end iterator
         *
         * \return Iterator past the last element of the set
         */
        Iterator GetEndIterator()
        {
            return Iterator(*this, nullptr);
        }

        /**
         * \brief Return a read-only past-the-end iterator
         *
         * \return Const iterator past the last element of the set
         */
        ConstIterator GetEndIterator() const
        {
            return ConstIterator(*this, nullptr);
        }

        using Base::Clear;
        using Base::GetSize;
        using Base::IsEmpty;
    };

    /// \name ADL begin/end support for range-based for loops
    /// \{

    /** \brief Returns a mutable iterator to the smallest element of \p s */
    template <typename E, typename C, typename A>
    typename TreeSet<E, C, A>::Iterator begin(TreeSet<E, C, A>& s)
    {
        return s.GetIterator();
    }

    /** \brief Returns a read-only iterator to the smallest element of \p s */
    template <typename E, typename C, typename A>
    typename TreeSet<E, C, A>::ConstIterator begin(const TreeSet<E, C, A>& s)
    {
        return s.GetIterator();
    }

    /** \brief Returns a mutable past-the-end iterator for \p s */
    template <typename E, typename C, typename A>
    typename TreeSet<E, C, A>::Iterator end(TreeSet<E, C, A>& s)
    {
        return s.GetEndIterator();
    }

    /** \brief Returns a read-only past-the-end iterator for \p s */
    template <typename E, typename C, typename A>
    typename TreeSet<E, C, A>::ConstIterator end(const TreeSet<E, C, A>& s)
    {
        return s.GetEndIterator();
    }

    /// \}
}

#endif // COCKTAILENGINE_CORE_TREESET_HPP
