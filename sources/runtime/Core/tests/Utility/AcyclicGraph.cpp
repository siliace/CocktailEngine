#include <catch2/catch_all.hpp>

#include <type_traits>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Utility/AcyclicGraph.hpp>
#include <CocktailEngine/Core/Utility/LazyValue.hpp>

using namespace Ck;

namespace
{
    /**
     * \brief Index and size type of every array in this file
     *
     * All of them are plain \c Array, so they index with the size type of
     * \c HeapAllocator rather than with \c std::size_t, and the counts a case states are
     * declared in it instead of being cast at each use.
     */
    using Index = HeapAllocator::SizeType;

    /**
     * \brief Node counting its own lifetime and recording what the reparenting hook saw
     *
     * The counters are what let a case say a subtree was destroyed rather than merely
     * unlinked: the graph hands out raw pointers, so nothing else distinguishes a node
     * that is gone from one that is still owned and no longer reachable.
     *
     * \remark OnParentChanged is overridden as protected, which is not incidental. An
     *         override of a protected hook is normally protected, and a base class has no
     *         access to the protected members of a class deriving from it, so this is the
     *         shape that decides whether the notification reaches an override at all.
     */
    class ProbeNode : public AcyclicGraphNode<ProbeNode>
    {
    public:

        static inline int Constructed = 0;
        static inline int Destroyed = 0;

        static void Reset()
        {
            Constructed = 0;
            Destroyed = 0;
        }

        /**
         * \brief Number of nodes constructed and not yet destroyed
         */
        static int Live()
        {
            return Constructed - Destroyed;
        }

        explicit ProbeNode(int value) :
            mValue(value)
        {
            ++Constructed;
        }

        ~ProbeNode() override
        {
            ++Destroyed;
        }

        ProbeNode(const ProbeNode&) = delete;
        ProbeNode& operator=(const ProbeNode&) = delete;

        int GetValue() const
        {
            return mValue;
        }

        /**
         * \brief Number of times this node was told its parent changed
         */
        int GetParentChangeCount() const
        {
            return mParentChangeCount;
        }

        /**
         * \brief Parent reported by the last notification as the one left behind
         */
        ProbeNode* GetPreviousParent() const
        {
            return mPreviousParent;
        }

        /**
         * \brief Parent this node had at the moment it was notified
         *
         * Read inside the hook rather than after it, which is what shows the notification
         * to be sent once both directions of the link are written and not halfway through
         * the move.
         */
        ProbeNode* GetParentWhenNotified() const
        {
            return mParentWhenNotified;
        }

    protected:

        void OnParentChanged(ProbeNode* previousParent) override
        {
            ++mParentChangeCount;
            mPreviousParent = previousParent;
            mParentWhenNotified = GetParent();
        }

    private:

        int mValue;
        int mParentChangeCount = 0;
        ProbeNode* mPreviousParent = nullptr;
        ProbeNode* mParentWhenNotified = nullptr;
    };

    /**
     * \brief Graph designating its root the way a real one does, from its constructor
     */
    class ProbeGraph : public AcyclicGraph<ProbeNode>
    {
    public:

        ProbeGraph()
        {
            mRoot = CreateNode(0);
        }
    };

    /**
     * \brief Graph letting a case choose which node is the root, and when
     *
     * Needed by the case about the root going stale, which has to designate a root
     * somewhere other than at the top of the hierarchy.
     */
    class OpenGraph : public AcyclicGraph<ProbeNode>
    {
    public:

        void SetRoot(ProbeNode* root)
        {
            mRoot = root;
        }
    };

    /**
     * \brief The shape every traversal case is written against
     *
     * <pre>
     *   root(0)
     *   |-- a(1)
     *   |   |-- c(3)
     *   |   +-- d(4)
     *   +-- b(2)
     *       +-- e(5)
     * </pre>
     *
     * Two levels under two different branches, and a first branch whose children come
     * before the second branch, so a pre-order walk of it is neither its insertion order
     * nor its breadth-first order. A flatter shape would pass under all three.
     */
    struct Hierarchy
    {
        ProbeNode* Root;
        ProbeNode* A;
        ProbeNode* B;
        ProbeNode* C;
        ProbeNode* D;
        ProbeNode* E;
    };

    Hierarchy BuildHierarchy(ProbeGraph& graph)
    {
        Hierarchy hierarchy{};

        hierarchy.Root = graph.GetRoot();
        hierarchy.A = graph.CreateNode(1);
        hierarchy.B = graph.CreateNode(2);
        hierarchy.C = graph.CreateNode(3);
        hierarchy.D = graph.CreateNode(4);
        hierarchy.E = graph.CreateNode(5);

        hierarchy.Root->InsertChild(hierarchy.A);
        hierarchy.Root->InsertChild(hierarchy.B);
        hierarchy.A->InsertChild(hierarchy.C);
        hierarchy.A->InsertChild(hierarchy.D);
        hierarchy.B->InsertChild(hierarchy.E);

        return hierarchy;
    }

    /**
     * \brief Callable accumulating into itself, passed as an lvalue
     *
     * A lambda capturing by reference would pass the traversal cases whether the callable
     * is handed down by reference or copied at each level, since what it writes to lives
     * outside it either way. This does not: a copy per child loses everything but the
     * branch the last copy walked.
     */
    struct Collector
    {
        void operator()(const ProbeNode* node)
        {
            Values.Add(node->GetValue());
        }

        Array<int> Values;
    };

    Array<int> ValuesOf(const Array<ProbeNode*>& nodes)
    {
        Array<int> values;
        values.Reserve(nodes.GetSize());

        for (const ProbeNode* node : nodes)
            values.Add(node->GetValue());

        return values;
    }

    /**
     * \brief Node caching a value derived from its ancestors, behind a LazyValue
     *
     * The shape TransformationNode has, reduced to arithmetic: a total that is this
     * node's own value plus its parent's total, computed on demand and kept until
     * something says it moved. That cache is the reason OnParentChanged exists, so the
     * hook is only really covered by a node that has one.
     *
     * A LazyValue starts dirty and goes clean on the first read, which is what makes the
     * order of a case matter here: the total has to be read before the hierarchy is
     * changed, or there is no stale answer to catch.
     */
    class AccumulatingNode : public AcyclicGraphNode<AccumulatingNode>
    {
    public:

        explicit AccumulatingNode(int local) :
            mLocal(local)
        {
        }

        /**
         * \brief This node's own value plus every value above it
         */
        int GetTotal() const
        {
            return mTotal.Get([this] {
                if (const AccumulatingNode* parent = GetParent())
                    return parent->GetTotal() + mLocal;

                return mLocal;
            });
        }

        void SetLocal(int local)
        {
            mLocal = local;
            Invalidate();
        }

    protected:

        void OnParentChanged(AccumulatingNode* previousParent) override
        {
            (void) previousParent;

            Invalidate();
        }

    private:

        /**
         * \brief Drops the cached total of this node and of everything below it
         *
         * The subtree and not just the node: every total below is composed through this
         * one, so all of them moved with it.
         */
        void Invalidate()
        {
            mTotal.SetDirty();

            for (AccumulatingNode* child : GetChildren())
                child->Invalidate();
        }

        int mLocal;
        LazyValue<int> mTotal;
    };

    class AccumulatingGraph : public AcyclicGraph<AccumulatingNode>
    {
    public:

        AccumulatingGraph()
        {
            mRoot = CreateNode(1);
        }
    };
}

TEST_CASE("A graph starts out empty", "[AcyclicGraph]")
{
    ProbeNode::Reset();

    AcyclicGraph<ProbeNode> graph;

    // The root is a plain pointer member and nothing had written it yet, so reading it
    // used to report whatever the storage happened to hold.
    REQUIRE(graph.GetRoot() == nullptr);
    REQUIRE(graph.GetNodeCount() == 0);

    SECTION("Visiting it reaches nothing rather than dereferencing a root it has not got")
    {
        int visited = 0;
        graph.Visit([&visited](ProbeNode*) {
            ++visited;
        });

        REQUIRE(visited == 0);
    }

    SECTION("Detaching it is allowed and changes nothing")
    {
        graph.Detach();

        REQUIRE(graph.GetRoot() == nullptr);
        REQUIRE(graph.GetNodeCount() == 0);
    }
}

TEST_CASE("A graph is neither copied nor moved", "[AcyclicGraph]")
{
    // Nodes name each other and callers hold raw pointers into the graph, so there is no
    // such thing as relocating one. The move operations used to be declared as defaulted,
    // which the pool being immovable turned into deleted: the declaration promised a
    // graph could be moved while every attempt to do so failed to compile.
    static_assert(!std::is_copy_constructible_v<AcyclicGraph<ProbeNode>>);
    static_assert(!std::is_move_constructible_v<AcyclicGraph<ProbeNode>>);
    static_assert(!std::is_copy_assignable_v<AcyclicGraph<ProbeNode>>);
    static_assert(!std::is_move_assignable_v<AcyclicGraph<ProbeNode>>);

    SUCCEED();
}

TEST_CASE("Inserting a child links it both ways", "[AcyclicGraphNode]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    ProbeNode* root = graph.GetRoot();
    ProbeNode* child = graph.CreateNode(1);

    SECTION("A node created but not inserted is owned and parented to nothing")
    {
        REQUIRE(child->GetParent() == nullptr);
        REQUIRE(child->GetChildren().IsEmpty());
        REQUIRE(graph.GetNodeCount() == 2);
    }

    SECTION("The parent lists the child and the child names the parent")
    {
        root->InsertChild(child);

        REQUIRE(child->GetParent() == root);
        REQUIRE(root->GetChildren().GetSize() == static_cast<Index>(1));
        REQUIRE(root->GetChildren()[0] == child);
    }

    SECTION("Children keep the order they were inserted in")
    {
        ProbeNode* second = graph.CreateNode(2);
        ProbeNode* third = graph.CreateNode(3);

        root->InsertChild(child);
        root->InsertChild(second);
        root->InsertChild(third);

        REQUIRE(root->GetChildren().GetSize() == static_cast<Index>(3));
        REQUIRE(root->GetChildren()[0] == child);
        REQUIRE(root->GetChildren()[1] == second);
        REQUIRE(root->GetChildren()[2] == third);
    }

    SECTION("Inserting a node where it already is changes nothing")
    {
        root->InsertChild(child);
        const int changesAfterFirst = child->GetParentChangeCount();

        root->InsertChild(child);

        REQUIRE(root->GetChildren().GetSize() == static_cast<Index>(1));
        REQUIRE(child->GetParent() == root);

        // Not a change, so not reported as one. Were it handled as a remove followed by
        // an insert, the node would be told twice that nothing had happened.
        REQUIRE(child->GetParentChangeCount() == changesAfterFirst);
    }
}

TEST_CASE("Reparenting a node leaves only one parent claiming it", "[AcyclicGraphNode]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    Hierarchy hierarchy = BuildHierarchy(graph);

    hierarchy.B->InsertChild(hierarchy.C);

    REQUIRE(hierarchy.C->GetParent() == hierarchy.B);
    REQUIRE(hierarchy.B->GetChildren().Contains(hierarchy.C));

    // The previous parent has to stop listing it, or the node sits in two children
    // arrays and a traversal from the root reaches it twice.
    REQUIRE_FALSE(hierarchy.A->GetChildren().Contains(hierarchy.C));
    REQUIRE(hierarchy.A->GetChildren().GetSize() == static_cast<Index>(1));
    REQUIRE(hierarchy.A->GetChildren()[0] == hierarchy.D);

    SECTION("The subtree of the moved node moves with it")
    {
        const Array<int> expected = { 2, 5, 3 };

        Array<int> values;
        hierarchy.B->Visit([&values](const ProbeNode* node) {
            values.Add(node->GetValue());
        });

        REQUIRE(values == expected);
    }
}

TEST_CASE("Detaching a child clears its parent", "[AcyclicGraphNode]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    Hierarchy hierarchy = BuildHierarchy(graph);

    SECTION("Both directions of the link go")
    {
        REQUIRE(hierarchy.Root->RemoveChild(hierarchy.A));

        REQUIRE_FALSE(hierarchy.Root->GetChildren().Contains(hierarchy.A));

        // The parent pointer used to be left in place, so a detached node went on reading
        // a parent that no longer listed it, and that a later teardown could have
        // recycled underneath it.
        REQUIRE(hierarchy.A->GetParent() == nullptr);
    }

    SECTION("The detached node keeps its own children")
    {
        hierarchy.Root->RemoveChild(hierarchy.A);

        REQUIRE(hierarchy.A->GetChildren().GetSize() == static_cast<Index>(2));
        REQUIRE(hierarchy.C->GetParent() == hierarchy.A);
        REQUIRE(hierarchy.D->GetParent() == hierarchy.A);
    }

    SECTION("A detached subtree is no longer reached from the root")
    {
        hierarchy.Root->RemoveChild(hierarchy.A);

        const Array<int> expected = { 0, 2, 5 };

        Array<ProbeNode*> reached;
        graph.Visit([&reached](ProbeNode* node) {
            reached.Add(node);
        });

        REQUIRE(ValuesOf(reached) == expected);

        // Detached, not destroyed: the graph still owns all six.
        REQUIRE(graph.GetNodeCount() == 6);
        REQUIRE(ProbeNode::Live() == 6);
    }

    SECTION("Naming the wrong parent changes nothing")
    {
        // C is a child of A, not of the root. Without the guard this would clear a link
        // the root never held, detaching C from A on A's behalf.
        REQUIRE_FALSE(hierarchy.Root->RemoveChild(hierarchy.C));

        REQUIRE(hierarchy.C->GetParent() == hierarchy.A);
        REQUIRE(hierarchy.A->GetChildren().Contains(hierarchy.C));
    }

    SECTION("Detaching a node that has no parent changes nothing")
    {
        ProbeNode* loose = graph.CreateNode(9);

        REQUIRE_FALSE(hierarchy.Root->RemoveChild(loose));
        REQUIRE(loose->GetParent() == nullptr);
    }
}

TEST_CASE("A node is told when its parent changes", "[AcyclicGraphNode]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    ProbeNode* root = graph.GetRoot();
    ProbeNode* first = graph.CreateNode(1);
    ProbeNode* second = graph.CreateNode(2);
    ProbeNode* child = graph.CreateNode(3);

    root->InsertChild(first);
    root->InsertChild(second);

    SECTION("Being inserted reports having had no parent")
    {
        first->InsertChild(child);

        REQUIRE(child->GetParentChangeCount() == 1);
        REQUIRE(child->GetPreviousParent() == nullptr);

        // Read from inside the hook, so the link was already written when the node was
        // told. A hook firing earlier could not look at where it had landed.
        REQUIRE(child->GetParentWhenNotified() == first);
    }

    SECTION("Being reparented reports one change, not two")
    {
        first->InsertChild(child);
        second->InsertChild(child);

        // Reparenting goes through an unlink internally, and that unlink is deliberately
        // silent: a node told twice would see itself parented to nothing halfway.
        REQUIRE(child->GetParentChangeCount() == 2);
        REQUIRE(child->GetPreviousParent() == first);
        REQUIRE(child->GetParentWhenNotified() == second);
    }

    SECTION("Being detached reports the parent it left")
    {
        first->InsertChild(child);
        first->RemoveChild(child);

        REQUIRE(child->GetParentChangeCount() == 2);
        REQUIRE(child->GetPreviousParent() == first);
        REQUIRE(child->GetParentWhenNotified() == nullptr);
    }

    SECTION("Nothing is reported to a node whose subtree is being destroyed")
    {
        first->InsertChild(child);
        const int changesBefore = first->GetParentChangeCount();

        graph.DestroyNode(child);

        // A node about to stop existing has no use for a notification, and the surviving
        // parent is not the one whose parent moved.
        REQUIRE(first->GetParentChangeCount() == changesBefore);
    }
}

TEST_CASE("Ancestry is walked through the parents", "[AcyclicGraphNode]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    Hierarchy hierarchy = BuildHierarchy(graph);

    REQUIRE(hierarchy.Root->IsAncestorOf(hierarchy.C));
    REQUIRE(hierarchy.A->IsAncestorOf(hierarchy.C));
    REQUIRE(hierarchy.Root->IsAncestorOf(hierarchy.E));

    SECTION("A node is not its own ancestor")
    {
        REQUIRE_FALSE(hierarchy.A->IsAncestorOf(hierarchy.A));
        REQUIRE_FALSE(hierarchy.Root->IsAncestorOf(hierarchy.Root));
    }

    SECTION("Ancestry does not run sideways nor downwards")
    {
        REQUIRE_FALSE(hierarchy.B->IsAncestorOf(hierarchy.C));
        REQUIRE_FALSE(hierarchy.C->IsAncestorOf(hierarchy.A));
    }

    SECTION("Nothing is an ancestor of nothing")
    {
        REQUIRE_FALSE(hierarchy.A->IsAncestorOf(nullptr));
    }

    SECTION("Ancestry follows a node that moves")
    {
        hierarchy.B->InsertChild(hierarchy.A);

        REQUIRE(hierarchy.B->IsAncestorOf(hierarchy.C));
        REQUIRE(hierarchy.Root->IsAncestorOf(hierarchy.C));
    }
}

TEST_CASE("Visiting walks a subtree in pre-order", "[AcyclicGraphNode]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    Hierarchy hierarchy = BuildHierarchy(graph);

    // This traversal did not compile at all until the iteration over the children was
    // fixed, and nothing in the engine called it, so nothing instantiated the template
    // and the two errors in it stayed out of sight.
    SECTION("From the graph, starting at the root")
    {
        const Array<int> expected = { 0, 1, 3, 4, 2, 5 };

        Array<ProbeNode*> visited;
        graph.Visit([&visited](ProbeNode* node) {
            visited.Add(node);
        });

        REQUIRE(ValuesOf(visited) == expected);
    }

    SECTION("From a node, starting at that node")
    {
        const Array<int> expected = { 1, 3, 4 };

        Array<int> values;
        hierarchy.A->Visit([&values](const ProbeNode* node) {
            values.Add(node->GetValue());
        });

        REQUIRE(values == expected);
    }

    SECTION("From a leaf, which is visited on its own")
    {
        const Array<int> expected = { 5 };

        Array<int> values;
        hierarchy.E->Visit([&values](const ProbeNode* node) {
            values.Add(node->GetValue());
        });

        REQUIRE(values == expected);
    }

    SECTION("Nodes not reachable from the root are not visited")
    {
        ProbeNode* loose = graph.CreateNode(9);

        Array<ProbeNode*> visited;
        graph.Visit([&visited](ProbeNode* node) {
            visited.Add(node);
        });

        REQUIRE_FALSE(visited.Contains(loose));
        REQUIRE(visited.GetSize() == static_cast<Index>(6));

        // Owned all the same, which is what the count reports and the traversal does not.
        REQUIRE(graph.GetNodeCount() == 7);
    }
}

TEST_CASE("One callable is passed down the whole traversal", "[AcyclicGraphNode]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    BuildHierarchy(graph);

    const Array<int> expected = { 0, 1, 3, 4, 2, 5 };

    Collector collector;
    graph.Visit(collector);

    // Copied once per child, the callable would come back holding the root and whatever
    // the last branch walked, since every copy would accumulate into itself and then be
    // dropped. Six values is the whole hierarchy in one object.
    REQUIRE(collector.Values == expected);
}

TEST_CASE("A hierarchy can be visited through a const reference", "[AcyclicGraphNode]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    Hierarchy hierarchy = BuildHierarchy(graph);

    SECTION("From a const graph")
    {
        const Array<int> expected = { 0, 1, 3, 4, 2, 5 };
        const AcyclicGraph<ProbeNode>& constGraph = graph;

        Array<int> values;
        constGraph.Visit([&values](auto* node) {
            static_assert(std::is_const_v<std::remove_pointer_t<decltype(node)>>,
                "the const traversal must hand the callable a pointer to const");

            values.Add(node->GetValue());
        });

        REQUIRE(values == expected);
    }

    SECTION("From a const node")
    {
        const Array<int> expected = { 1, 3, 4 };
        const ProbeNode& constNode = *hierarchy.A;

        Array<int> values;
        constNode.Visit([&values](auto* node) {
            static_assert(std::is_const_v<std::remove_pointer_t<decltype(node)>>,
                "the const traversal must hand the callable a pointer to const");

            values.Add(node->GetValue());
        });

        REQUIRE(values == expected);
    }
}

TEST_CASE("Destroying a node destroys its subtree", "[AcyclicGraph]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    Hierarchy hierarchy = BuildHierarchy(graph);

    REQUIRE(ProbeNode::Live() == 6);

    SECTION("A leaf goes on its own")
    {
        graph.DestroyNode(hierarchy.E);

        REQUIRE(ProbeNode::Live() == 5);
        REQUIRE(graph.GetNodeCount() == 5);

        // The surviving parent stops listing it, or the next traversal walks into a slot
        // the pool has taken back.
        REQUIRE(hierarchy.B->GetChildren().IsEmpty());
    }

    SECTION("An inner node takes its children with it")
    {
        graph.DestroyNode(hierarchy.A);

        // A, C and D. Children are only reachable through the node being destroyed, so
        // leaving them behind would keep storage alive that nothing can name.
        REQUIRE(ProbeNode::Live() == 3);
        REQUIRE(graph.GetNodeCount() == 3);
    }

    SECTION("What is left of the hierarchy is intact")
    {
        const Array<int> expected = { 0, 2, 5 };

        graph.DestroyNode(hierarchy.A);

        REQUIRE(hierarchy.Root->GetChildren().GetSize() == static_cast<Index>(1));
        REQUIRE(hierarchy.Root->GetChildren()[0] == hierarchy.B);

        Array<ProbeNode*> visited;
        graph.Visit([&visited](ProbeNode* node) {
            visited.Add(node);
        });

        REQUIRE(ValuesOf(visited) == expected);
    }

    SECTION("A node held outside the hierarchy goes as well")
    {
        ProbeNode* loose = graph.CreateNode(9);

        REQUIRE(graph.GetNodeCount() == 7);

        graph.DestroyNode(loose);

        REQUIRE(graph.GetNodeCount() == 6);
        REQUIRE(ProbeNode::Live() == 6);
    }

    SECTION("Destroying the root clears it")
    {
        graph.DestroyNode(hierarchy.Root);

        REQUIRE(graph.GetRoot() == nullptr);
        REQUIRE(graph.GetNodeCount() == 0);
        REQUIRE(ProbeNode::Live() == 0);
    }

    SECTION("A detached subtree can be destroyed")
    {
        hierarchy.Root->RemoveChild(hierarchy.A);
        graph.DestroyNode(hierarchy.A);

        REQUIRE(ProbeNode::Live() == 3);
        REQUIRE(hierarchy.Root->GetChildren().GetSize() == static_cast<Index>(1));
    }
}

TEST_CASE("The root is cleared whenever the node it names is destroyed", "[AcyclicGraph]")
{
    ProbeNode::Reset();

    // The root need not be the top of the hierarchy: it is a pointer a deriving class
    // writes, so it can name a node with a parent above it. Destroying that parent takes
    // the root along, and the graph must not be left naming a recycled slot.
    OpenGraph graph;
    ProbeNode* above = graph.CreateNode(0);
    ProbeNode* designated = graph.CreateNode(1);

    above->InsertChild(designated);
    graph.SetRoot(designated);

    REQUIRE(graph.GetRoot() == designated);

    graph.DestroyNode(above);

    REQUIRE(graph.GetRoot() == nullptr);
    REQUIRE(graph.GetNodeCount() == 0);
    REQUIRE(ProbeNode::Live() == 0);
}

TEST_CASE("Detaching a graph destroys every node it owns", "[AcyclicGraph]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    BuildHierarchy(graph);
    graph.CreateNode(9);

    REQUIRE(ProbeNode::Live() == 7);

    graph.Detach();

    // Including the one never inserted anywhere: ownership is what Detach goes by, not
    // reachability.
    REQUIRE(ProbeNode::Live() == 0);
    REQUIRE(graph.GetNodeCount() == 0);
    REQUIRE(graph.GetRoot() == nullptr);

    SECTION("Detaching twice is allowed")
    {
        graph.Detach();

        REQUIRE(graph.GetNodeCount() == 0);
    }

    SECTION("The graph can be filled again")
    {
        ProbeNode* root = graph.CreateNode(10);
        ProbeNode* child = graph.CreateNode(11);

        root->InsertChild(child);

        REQUIRE(graph.GetNodeCount() == 2);
        REQUIRE(child->GetParent() == root);
        REQUIRE(ProbeNode::Live() == 2);
    }
}

TEST_CASE("A graph going out of scope destroys its nodes", "[AcyclicGraph]")
{
    ProbeNode::Reset();

    {
        ProbeGraph graph;
        BuildHierarchy(graph);

        REQUIRE(ProbeNode::Live() == 6);
    }

    // Every node is unlinked before any of them is released, which is what the node
    // destructor asserts: a destructor reading its parent or its children has to find
    // nodes rather than slots already back on the free list.
    REQUIRE(ProbeNode::Live() == 0);
}

TEST_CASE("Node storage is reused across a graph's life", "[AcyclicGraph]")
{
    ProbeNode::Reset();

    ProbeGraph graph;
    ProbeNode* first = graph.CreateNode(1);
    ProbeNode* address = first;

    graph.DestroyNode(first);

    ProbeNode* second = graph.CreateNode(2);

    // The pool hands the slot straight back, which is the whole reason the graph carves
    // its nodes out of one.
    REQUIRE(second == address);
    REQUIRE(second->GetValue() == 2);

    // Reconstructed in place, so it starts over with no parent, no children and nothing
    // recorded by its hook, rather than inheriting what the previous occupant held.
    REQUIRE(second->GetParent() == nullptr);
    REQUIRE(second->GetChildren().IsEmpty());
    REQUIRE(second->GetParentChangeCount() == 0);
}

TEST_CASE("A value cached from the ancestors follows the node that moves", "[AcyclicGraphNode]")
{
    // What the reparenting hook is for. A node deriving anything from its ancestors has
    // no other way to learn that the answer moved: nothing on the node itself was
    // written, so a cache keyed on its own mutators never hears about it. This is the
    // shape TransformationNode has, its world transformation being its local one
    // composed with its parent's.
    AccumulatingGraph graph;
    AccumulatingNode* root = graph.GetRoot();
    AccumulatingNode* first = graph.CreateNode(10);
    AccumulatingNode* second = graph.CreateNode(100);
    AccumulatingNode* child = graph.CreateNode(2);

    root->InsertChild(first);
    root->InsertChild(second);
    first->InsertChild(child);

    // Reading it is what fills the cache, and every check below depends on that having
    // happened before the hierarchy changes.
    REQUIRE(child->GetTotal() == 13);

    SECTION("Reparenting invalidates what was cached under the previous parent")
    {
        second->InsertChild(child);

        REQUIRE(child->GetTotal() == 103);
    }

    SECTION("Being detached invalidates it too")
    {
        first->RemoveChild(child);

        REQUIRE(child->GetTotal() == 2);
    }

    SECTION("The subtree of a moved node is invalidated with it")
    {
        AccumulatingNode* grandChild = graph.CreateNode(5);
        child->InsertChild(grandChild);

        REQUIRE(grandChild->GetTotal() == 18);

        second->InsertChild(child);

        REQUIRE(child->GetTotal() == 103);
        REQUIRE(grandChild->GetTotal() == 108);
    }

    SECTION("A node's own mutator still invalidates it")
    {
        child->SetLocal(3);

        REQUIRE(child->GetTotal() == 14);
    }
}
