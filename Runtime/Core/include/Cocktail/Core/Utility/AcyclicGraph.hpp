#ifndef COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP
#define COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP

#include <memory>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

namespace Ck
{
	/**
     * \brief 
     * \tparam T 
     */
    template <typename T>
	class AcyclicGraph
	{
	public:

		/**
		 * \brief 
		 */
		AcyclicGraph() = default;

		/**
		 * \brief 
		 * \param other 
		 */
		AcyclicGraph(const AcyclicGraph& other) = delete;

		/**
    	 * \brief 
    	 * \param other 
    	 */
    	AcyclicGraph(AcyclicGraph&& other) noexcept = default;

		/**
    	 * \brief 
    	 * \param other 
    	 * \return 
    	 */
    	AcyclicGraph& operator=(const AcyclicGraph& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		AcyclicGraph& operator=(AcyclicGraph&& other) noexcept = default;

		/**
		 * \brief 
		 */
		~AcyclicGraph()
		{
		    Detach();
		}

		/**
		 * \brief 
		 * \tparam Args 
		 * \param args 
		 * \return 
		 */
		template <typename... Args>
		T* CreateNode(Args&&... args)
		{
			NodePtr node = mNodePool.AllocateUnique(std::forward<Args>(args)...);
		    T* nodePtr = node.Get();

		    mNodes.Add(std::move(node));

		    return nodePtr;
		}

		void Detach()
		{
		    mRoot = nullptr;

		    mNodes.Clear();
		    mNodePool.Clear();
		}

		/**
		 * \brief 
		 * \tparam TCallable 
		 * \param callable 
		 */
		template <typename TCallable>
		void Visit(TCallable&& callable)
		{
			mRoot->Visit(std::forward<TCallable>(callable));
		}

		/**
		 * \brief 
		 * \return 
		 */
		T* GetRoot() const
		{
			return mRoot;
		}

    protected:

		T* mRoot;

	private:

        using NodePtr = typename ObjectPool<T>::Unique;

        Array<NodePtr> mNodes;
		ObjectPool<T> mNodePool;
	};

    /**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class AcyclicGraphNode
	{
	public:

		virtual ~AcyclicGraphNode() = default;

		/**
		 * \brief 
		 * \param child 
		 */
		void InsertChild(T* child)
		{
			if (T* parent = child->GetParent())
				parent->RemoveChild(child);

			child->mParent = static_cast<T*>(this);
			mChildren.Add(child);
		}

		/**
		 * \brief 
		 * \param child 
		 * \return 
		 */
		void RemoveChild(T* child)
		{
			mChildren.Remove(child);
		}

		/**
		 * \brief 
		 * \tparam TCallable 
		 * \param callable 
		 */
		template <typename TCallable>
		void Visit(TCallable callable)
		{
			callable(static_cast<T*>(this));
			for (const T*& child : mChildren)
				child->Visit(callable);
		}

		T* GetParent() const
		{
			return mParent;
		}

		const Array<T*>& GetChildren() const
		{
			return mChildren;
		}

	private:

		T* mParent = nullptr;
		Array<T*> mChildren;
	};
}

#endif // COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP
