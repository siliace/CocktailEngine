#ifndef COCKTAILENGINE_CORE_UTILITY_ACYCLICGRAPH_HPP
#define COCKTAILENGINE_CORE_UTILITY_ACYCLICGRAPH_HPP

#include <cassert>
#include <cstddef>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Utility/ObjectPool.hpp>

namespace Ck
{
    template <typename T>
    class AcyclicGraph;

    /**
     * \class AcyclicGraphNode
     *
     * \brief Base of a node in an AcyclicGraph, holding its place in the hierarchy
     *
     * Carries the one parent and the children of a node, and nothing else: what a node
     * means is entirely the business of the type deriving from it. Inherited through
     * CRTP, so every pointer handed out and every pointer taken in is a \p T and a
     * caller never has to cast back down to the type it actually works with.
     *
     * A node has at most one parent, which makes the hierarchy a tree rather than the
     * general directed acyclic graph the name suggests. Nothing here supports a node
     * reachable by two different paths, and Visit has no record of what it has already
     * seen, so a shared subtree would be walked once per path leading to it.
     *
     * Invariants maintained by this class, relied upon by AcyclicGraph:
     *  - A node appears in the children of its parent, and in no other node's children.
     *    The two directions are only ever written together, so one can always be trusted
     *    to agree with the other.
     *  - No cycle is reachable by following parents. InsertChild is the only way to
     *    create a link and it refuses, in a build with assertions, to make a node its
     *    own descendant.
     *  - A node is unlinked before it is destroyed. AcyclicGraph unlinks whole subtrees
     *    ahead of releasing any of their storage, which is what lets a node destructor
     *    read its neighbours, and what the destructor asserts.
     *
     * \tparam T Type deriving from this class, which must do so publicly: the graph
     *         reaches this base through a \p T pointer to unlink a node
     *
     * \remark Not thread safe, and neither is the graph holding it. Two threads
     *         reparenting nodes of the same hierarchy is a data race on the children
     *         arrays involved.
     *
     * \remark The accessors hand out mutable \p T pointers even from a const node, so
     *         constness stops at the node it was asked of and does not reach the
     *         hierarchy below. Visit is the exception, and the way to walk a hierarchy
     *         one may not modify.
     */
    template <typename T>
    class AcyclicGraphNode
    {
    public:

        /**
         * \brief Destructor
         *
         * \remark Asserts that the node is no longer linked to anything. A failing
         *         assert means a node was destroyed while its parent or its children
         *         still pointed at it, which leaves those pointers dangling. Going
         *         through AcyclicGraph::DestroyNode or AcyclicGraph::Detach cannot
         *         produce that, since both unlink an entire subtree before releasing
         *         any of it.
         *
         * \remark Virtual because OnParentChanged is: a node is reparented through the
         *         base, and the type deriving from it has to be the one told. The
         *         vtable that costs is the same one the hook needs.
         */
        virtual ~AcyclicGraphNode()
        {
            assert(mParent == nullptr && mChildren.IsEmpty());
        }

        /**
         * \brief Makes \p child a child of this node
         *
         * Detaches \p child from the parent it had, if any, so a node is never left in
         * two children arrays at once. Reparenting is therefore a single call rather
         * than a remove followed by an insert, and OnParentChanged fires once for it
         * instead of twice.
         *
         * \param child Node to insert, must not be null
         *
         * \remark Does nothing at all when \p child is already a child of this node,
         *         rather than moving it to the end of the children. Inserting twice is
         *         therefore not observable, and no notification is sent for it.
         *
         * \warning \p child must not be this node nor one of its ancestors, which is
         *          what the asserts check: either would close a cycle, and a cycle
         *          makes Visit recurse until the stack runs out. The ancestor check
         *          walks up to the root, so it costs the depth of this node and only
         *          in a build with assertions.
         */
        void InsertChild(T* child)
        {
            assert(child != nullptr);
            assert(child != static_cast<T*>(this));
            assert(!child->IsAncestorOf(static_cast<T*>(this)));

            T* previousParent = child->mParent;
            if (previousParent == static_cast<T*>(this))
                return;

            // Unlinked without notifying, so that the single notification sent below
            // reports the whole move and not the halfway state where the node had no
            // parent at all.
            child->UnlinkFromParent();

            child->mParent = static_cast<T*>(this);
            mChildren.Add(child);

            NotifyParentChanged(child, previousParent);
        }

        /**
         * \brief Detaches \p child from this node
         *
         * Clears the parent of \p child as well as removing it from the children, so
         * the node is left genuinely detached rather than pointing back at a parent
         * that no longer lists it.
         *
         * \param child Node to remove, must not be null
         *
         * \return Whether \p child was a child of this node, and was detached
         *
         * \remark Returns false, and changes nothing, for a node parented elsewhere.
         *         Without that guard a caller naming the wrong parent would clear a
         *         link this node never owned.
         *
         * \remark The node keeps its own children. Detaching is not destroying: the
         *         subtree stays intact, unparented, and stays alive as long as the
         *         graph that created it, until it is inserted somewhere else or handed
         *         to AcyclicGraph::DestroyNode.
         */
        bool RemoveChild(T* child)
        {
            assert(child != nullptr);

            if (child->mParent != static_cast<T*>(this))
                return false;

            mChildren.Remove(child);
            child->mParent = nullptr;

            NotifyParentChanged(child, static_cast<T*>(this));

            return true;
        }

        /**
         * \brief Reports whether this node is an ancestor of \p node
         *
         * \param node Node whose ancestry is walked, may be null
         *
         * \return Whether this node is reachable by following the parents of \p node
         *
         * \remark Costs the depth of \p node. A node is not its own ancestor, so this
         *         returns false when \p node is this node.
         *
         * \remark Terminates because the hierarchy holds no cycle. It is what
         *         InsertChild checks that invariant with, and it relies on it having
         *         held up to that point.
         */
        bool IsAncestorOf(const T* node) const
        {
            for (const T* current = node ? node->mParent : nullptr; current; current = current->mParent)
            {
                if (current == static_cast<const T*>(this))
                    return true;
            }

            return false;
        }

        /**
         * \brief Calls \p callable on this node and on every node below it
         *
         * Pre-order: a node is visited before its children, and children in the order
         * they were inserted.
         *
         * \tparam TCallable Type of the callable, invocable with a \p T pointer
         *
         * \param callable Callable invoked once per node of the subtree
         *
         * \remark The callable is passed down by reference rather than copied at each
         *         level, so one that accumulates into itself holds every node of the
         *         subtree when the call returns, not just the ones below the last child.
         *
         * \warning Recursive, and unbounded: the depth of the hierarchy is what bounds
         *          the stack it uses. A hierarchy deep enough to matter needs a
         *          traversal of its own rather than this one.
         *
         * \warning \p callable must not reparent nor destroy the nodes it is handed.
         *          The traversal holds the children array of each level while walking
         *          it, and both operations write to one.
         */
        template <typename TCallable>
        void Visit(TCallable&& callable)
        {
            VisitSubtree(callable);
        }

        /**
         * \brief Calls \p callable on this node and on every node below it
         *
         * The const half of Visit, handing the callable const \p T pointers. It is the
         * only thing here that keeps constness while descending, and therefore the way
         * to read a hierarchy through a const reference.
         *
         * \tparam TCallable Type of the callable, invocable with a const \p T pointer
         *
         * \param callable Callable invoked once per node of the subtree
         */
        template <typename TCallable>
        void Visit(TCallable&& callable) const
        {
            VisitSubtree(callable);
        }

        /**
         * \brief Returns the parent of this node
         *
         * \return Parent of this node, null when it is a root
         */
        T* GetParent() const
        {
            return mParent;
        }

        /**
         * \brief Returns the children of this node
         *
         * \return Children in the order they were inserted, empty when it is a leaf
         *
         * \warning The reference is invalidated by anything that changes the children
         *          of this node, which includes a call to InsertChild or RemoveChild on
         *          another node naming one of them.
         */
        const Array<T*>& GetChildren() const
        {
            return mChildren;
        }

    protected:

        /**
         * \brief Called on a node whose parent has just changed
         *
         * The one seam a deriving type gets into reparenting, and the reason there is
         * one: a node caching anything computed from its ancestors has no other way to
         * learn that the answer moved. TransformationNode is the case in point, its
         * world transformation being its local one composed with its parent's.
         *
         * Fires once per change, after both directions of the link have been written,
         * so an implementation may already walk its new surroundings. It does not fire
         * for a node inserted where it already was, nor for a node being destroyed:
         * neither is a change anything can usefully react to.
         *
         * \param previousParent Parent the node had, null when it had none
         *
         * \remark Does nothing by default. A node that derives nothing from its
         *         ancestors has nothing to do here.
         *
         * \warning Called while the hierarchy is consistent but mid-operation. An
         *          implementation may read the hierarchy and write itself and its own
         *          subtree, and must not reparent nor destroy anything: it would be
         *          changing the very links the call that is still running just wrote.
         */
        virtual void OnParentChanged(T* previousParent)
        {
            (void) previousParent;
        }

    private:

        template <typename>
        friend class AcyclicGraph;

        /**
         * \brief Tells \p node that its parent changed, reaching its override
         *
         * Goes through the base rather than through \p T, and has to: an override of a
         * protected hook is normally protected itself, and a base class has no access
         * to the protected members of a class deriving from it. Casting to the base
         * makes AcyclicGraphNode the class the access is made through, which is the one
         * class allowed to reach its own protected members.
         *
         * The call stays unqualified, so it still dispatches virtually and lands on the
         * override rather than on the empty body here.
         *
         * \param node Node whose parent changed, never null
         * \param previousParent Parent it had, null when it had none
         */
        static void NotifyParentChanged(T* node, T* previousParent)
        {
            static_cast<AcyclicGraphNode*>(node)->OnParentChanged(previousParent);
        }

        /**
         * \brief Removes this node from its parent without notifying it
         *
         * The half of RemoveChild that only rewrites the links. Used where a
         * notification would be wrong rather than merely unnecessary: by InsertChild,
         * which sends one of its own once the move is complete, and by
         * AcyclicGraph::DestroyNode, whose node is about to stop existing.
         */
        void UnlinkFromParent()
        {
            if (mParent == nullptr)
                return;

            mParent->mChildren.Remove(static_cast<T*>(this));
            mParent = nullptr;
        }

        /**
         * \brief Forgets the parent and the children of this node, notifying no one
         *
         * Only ever called on every node of a subtree in turn, by AcyclicGraph, and
         * only because the whole of that subtree is about to be released. Called on one
         * node alone it would break the invariant that the two directions of a link
         * agree, leaving the parent listing a child that no longer claims it.
         */
        void ClearLinks()
        {
            mParent = nullptr;
            mChildren.Clear();
        }

        /**
         * \brief Body of the traversal, taking the callable by reference
         *
         * Visit takes its callable by forwarding reference and hands it to this, which
         * passes the same object all the way down. Recursing through Visit itself would
         * copy it once per child, and a callable accumulating state would lose
         * everything but the last branch.
         */
        template <typename TCallable>
        void VisitSubtree(TCallable& callable)
        {
            callable(static_cast<T*>(this));

            for (T* child : mChildren)
                child->VisitSubtree(callable);
        }

        /**
         * \brief Body of the const traversal, taking the callable by reference
         */
        template <typename TCallable>
        void VisitSubtree(TCallable& callable) const
        {
            callable(static_cast<const T*>(this));

            for (const T* child : mChildren)
                child->VisitSubtree(callable);
        }

        T* mParent = nullptr; /*!< Parent of this node, null when it is a root */
        Array<T*> mChildren; /*!< Children of this node, each of which names it back */
    };

    /**
     * \class AcyclicGraph
     *
     * \brief Owner of a hierarchy of AcyclicGraphNode, pooling their storage
     *
     * Nodes are carved out of an ObjectPool, so a hierarchy that churns costs a free
     * list pop per node rather than a heap allocation, and nodes of one graph sit near
     * one another in memory. The graph owns every node it creates and hands out raw
     * pointers to them: a node lives until it is destroyed through this class, never
     * because a caller dropped the last reference to it.
     *
     * Creating a node and placing it in the hierarchy are two separate steps. A node
     * comes out of CreateNode parented to nothing, and stays that way until something
     * calls InsertChild on it. The root is the one node a graph knows by name, and it
     * is for the deriving class to create and to designate.
     *
     * The graph is neither copyable nor movable. Nodes point at each other and callers
     * hold raw pointers into it, so relocating one would mean rewriting every pointer
     * naming a node of the hierarchy, and its pool cannot be moved either.
     *
     * \tparam T Node type, which must publicly derive from AcyclicGraphNode<T>
     *
     * \remark Not thread safe. Its pool serializes access to the storage it hands out,
     *         but the node list, the root and the links between nodes are not
     *         protected by anything.
     */
    template <typename T>
    class AcyclicGraph
    {
    public:

        /**
         * \brief Constructor
         *
         * Creates an empty graph, with no node and no root. Its pool allocates its
         * first page when the first node is created.
         */
        AcyclicGraph() = default;

        /**
         * \brief Deleted copy constructor
         */
        AcyclicGraph(const AcyclicGraph& other) = delete;

        /**
         * \brief Deleted move constructor
         *
         * \remark Deleted rather than defaulted, and deliberately. ObjectPool is not
         *         movable, so a defaulted move constructor would be defined as deleted
         *         anyway, and the declaration would promise a graph could be moved
         *         while every attempt failed to compile.
         */
        AcyclicGraph(AcyclicGraph&& other) noexcept = delete;

        /**
         * \brief Deleted copy assignment operator
         */
        AcyclicGraph& operator=(const AcyclicGraph& other) = delete;

        /**
         * \brief Deleted move assignment operator
         */
        AcyclicGraph& operator=(AcyclicGraph&& other) noexcept = delete;

        /**
         * \brief Destructor
         *
         * Destroys every node the graph still holds, through Detach.
         *
         * \remark Not virtual, and a graph must therefore be destroyed through its own
         *         type. A deriving class is what designates the root, which makes it
         *         part of how a graph is built rather than a handle anything would hold
         *         polymorphically.
         */
        ~AcyclicGraph()
        {
            Detach();
        }

        /**
         * \brief Creates a node owned by this graph
         *
         * The node comes back parented to nothing and with no children. Placing it in
         * the hierarchy is a separate call to InsertChild on the node meant to hold it,
         * and a node never inserted anywhere is still owned, and still destroyed with
         * the graph.
         *
         * \tparam Args Types of the arguments forwarded to \p T's constructor
         *
         * \param args Arguments forwarded to \p T's constructor
         *
         * \return Pointer to the new node, never null, owned by this graph
         *
         * \throw OutOfMemory If the pool had to grow and could not allocate
         *
         * \remark The pointer stays valid until the node is destroyed through
         *         DestroyNode or Detach. Nothing else invalidates it: the pool never
         *         moves a node it has handed out.
         */
        template <typename... Args>
        T* CreateNode(Args&&... args)
        {
            ObjectPoolUniquePtr<T> node = mNodePool.Allocate(Forward<Args>(args)...);
            T* nodePtr = node.Get();

            mNodes.Add(Move(node));

            return nodePtr;
        }

        /**
         * \brief Destroys a node and everything below it
         *
         * A subtree is destroyed as a whole rather than leaving children behind: they
         * are only reachable through the node being destroyed, so orphaning them would
         * keep storage alive that nothing can name. Detach a node first if its children
         * are meant to survive it.
         *
         * \param node Root of the subtree to destroy, must not be null and must belong
         *        to this graph
         *
         * \remark Clears the root of the graph when it falls inside the destroyed
         *         subtree, so the graph never names a node it has released.
         *
         * \remark Costs a pass over every node the graph owns, times the size of the
         *         subtree: the owning handles are held in a flat array and are found by
         *         searching it. Destroying a leaf, which is the common case, is one
         *         pass. A graph destroying large subtrees every frame wants an index
         *         from node to handle rather than this.
         *
         * \warning Every pointer into the destroyed subtree is dangling afterwards,
         *          including \p node itself.
         *
         * \warning Do not call from inside a traversal of the same graph. Visit holds
         *          the children array of each level as it walks it, and this rewrites
         *          them.
         */
        void DestroyNode(T* node)
        {
            assert(node != nullptr);

            // Collected before anything is unlinked, since the links are what describe
            // the subtree in the first place.
            Array<T*> doomed;
            node->Visit([&doomed](T* current) {
                doomed.Add(current);
            });

            // The parent of the subtree survives it, so it is the one node that has to
            // be told. Told by rewriting its children rather than through RemoveChild:
            // a node about to stop existing has no use for the notification.
            node->UnlinkFromParent();

            // Every link inside the subtree goes before any of its storage does. A node
            // destructor reading its parent or its children has to find nodes, not
            // slots the pool has already taken back.
            for (T* current : doomed)
                current->ClearLinks();

            if (doomed.Contains(mRoot))
                mRoot = nullptr;

            // The handles are released when this array goes out of scope, which is what
            // recycles the nodes.
            Array<ObjectPoolUniquePtr<T>> released = mNodes.RemoveIf([&doomed](const ObjectPoolUniquePtr<T>& owned) {
                return doomed.Contains(owned.Get());
            });

            // Short means a node of the subtree was not owned here, so \p node came
            // from another graph and this call has just torn up its hierarchy.
            assert(released.GetSize() == doomed.GetSize());
        }

        /**
         * \brief Destroys every node of the graph
         *
         * Leaves the graph as it was constructed: no node, no root, and a pool holding
         * the pages it has grown to. It can be filled again afterwards.
         *
         * \remark Unlinks every node before releasing any of them, for the reason
         *         DestroyNode does: a node destructor must not find a neighbour whose
         *         slot has already gone back to the pool.
         *
         * \remark Called by the destructor, so a graph does not have to be detached by
         *         hand. Calling it early is how a holder of several graphs controls the
         *         order they come down in, which matters when nodes of one point at
         *         nodes of another.
         */
        void Detach()
        {
            for (const ObjectPoolUniquePtr<T>& node : mNodes)
                node->ClearLinks();

            mRoot = nullptr;
            mNodes.Clear();
        }

        /**
         * \brief Calls \p callable on the root and on every node below it
         *
         * \tparam TCallable Type of the callable, invocable with a \p T pointer
         *
         * \param callable Callable invoked once per node reachable from the root
         *
         * \remark Does nothing on a graph with no root, rather than dereferencing one
         *         that is not there.
         *
         * \remark Visits what the root reaches, which is not necessarily every node the
         *         graph owns: a node created and never inserted is owned, and not
         *         reachable. GetNodeCount counts those, this does not reach them.
         */
        template <typename TCallable>
        void Visit(TCallable&& callable)
        {
            if (mRoot == nullptr)
                return;

            mRoot->Visit(Forward<TCallable>(callable));
        }

        /**
         * \brief Calls \p callable on the root and on every node below it
         *
         * \tparam TCallable Type of the callable, invocable with a const \p T pointer
         *
         * \param callable Callable invoked once per node reachable from the root
         */
        template <typename TCallable>
        void Visit(TCallable&& callable) const
        {
            if (mRoot == nullptr)
                return;

            // Cast so the const traversal is the one selected: mRoot is a pointer to
            // non-const T even here, constness of the graph saying nothing about the
            // nodes it points at.
            static_cast<const T*>(mRoot)->Visit(Forward<TCallable>(callable));
        }

        /**
         * \brief Returns the root of the graph
         *
         * \return Root of the graph, null when nothing has been designated as one
         */
        T* GetRoot() const
        {
            return mRoot;
        }

        /**
         * \brief Returns how many nodes the graph owns
         *
         * \return Number of live nodes, reachable from the root or not
         *
         * \remark Worth watching for what it says about churn: a count that grows
         *         while the hierarchy does not is nodes created and never destroyed.
         */
        std::size_t GetNodeCount() const
        {
            return static_cast<std::size_t>(mNodes.GetSize());
        }

    protected:

        /**
         * \brief Root of the graph, designated by the deriving class
         *
         * Written directly by the deriving class, usually from its constructor and out
         * of a call to CreateNode. It is not the graph that decides what the root is:
         * a hierarchy of transformations starts at an identity, a scene at a node
         * carrying the scene itself, and only they know which.
         *
         * \warning Must name a node this graph owns, or null. DestroyNode clears it
         *          when the node it names is destroyed, so it does not go stale on its
         *          own, but nothing checks what is written here in the first place.
         */
        T* mRoot = nullptr;

    private:

        // Declared before the node list so that it is destroyed after it. The order is
        // not load bearing, an ObjectPool outliving its own destruction through the
        // references its handles hold, but the dependency reads the right way round.
        ObjectPool<T> mNodePool; /*!< Storage every node of this graph is carved out of */
        Array<ObjectPoolUniquePtr<T>> mNodes; /*!< Owning handle over every live node */
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_ACYCLICGRAPH_HPP
