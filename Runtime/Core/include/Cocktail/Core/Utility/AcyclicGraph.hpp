#ifndef COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP
#define COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP

#include <memory>

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

		~AcyclicGraph()
		{
			mRoot = nullptr;
			mNodePool.Clear();
		}

		/**
		 * \brief 
		 * \tparam Args 
		 * \param args 
		 * \return 
		 */
		template <typename... Args>
		std::shared_ptr<T> CreateNode(Args&&... args)
		{
			return mNodePool.Allocate(std::forward<Args>(args)...);
		}

		void Detach()
		{
			mRoot = nullptr;
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
		std::shared_ptr<T> GetRoot() const
		{
			return mRoot;
		}

    protected:

		std::shared_ptr<T> mRoot;

	private:

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
		void InsertChild(std::shared_ptr<T> child)
		{
			if (auto parent = child->GetParent())
				parent->RemoveChild(child);

			child->mParent = static_cast<T*>(this);
			mChildren.push_back(std::move(child));
		}

		/**
		 * \brief 
		 * \param child 
		 * \return 
		 */
		void RemoveChild(const std::shared_ptr<T>& child)
		{
			for (auto it = mChildren.begin(); it != mChildren.end();)
			{
				if (*it == child)
				{
					it = mChildren.erase(it);
				}
				else
				{
					++it;
				}
			}
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
			for (const std::shared_ptr<T>& child : mChildren)
				child->Visit(callable);
		}

		T* GetParent() const
		{
			return mParent;
		}

		const std::vector<std::shared_ptr<T>>& GetChildren() const
		{
			return mChildren;
		}

	private:

		T* mParent = nullptr;
		std::vector<std::shared_ptr<T>> mChildren;
	};
}

#endif // COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP
