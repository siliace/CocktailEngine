#ifndef COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP
#define COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP

#include <memory>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>
#include <Cocktail/Core/Meta/Implements.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

namespace Ck
{
	/**
     * \brief 
     * \tparam T 
     */
    template <typename T>
	class AcyclicGraph : public Extends<AcyclicGraph<T>, Object>
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
		Ref<T> CreateNode(Args&&... args)
		{
			return mNodePool.Allocate(std::forward<Args>(args)...);
		}

		void Detach()
		{
			mRoot = nullptr;
		}

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param callable 
		 */
		template <typename Callable>
		void Visit(Callable&& callable)
		{
			mRoot->Visit(callable);
		}

		/**
		 * \brief 
		 * \return 
		 */
		Ref<T> GetRoot() const
		{
			return mRoot;
		}

    protected:

		Ref<T> mRoot;

	private:

		ObjectPool<T> mNodePool;
	};

    /**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class AcyclicGraphNode : public Implements<AcyclicGraphNode<T>, Interface>
	{
	public:

		/**
		 * \brief 
		 * \param child 
		 */
		void InsertFrontChild(Ref<T> child)
		{
			child->SetNextSibling(std::move(mFirstChild));
			mFirstChild = std::move(child);
		}

		/**
		 * \brief 
		 * \param child 
		 */
		void InsertChild(Ref<T> child)
		{
			child->mParent = ConcreteSelf();

			if (!mFirstChild)
			{
				mFirstChild = std::move(child);
			}
			else
			{
				Ref<T> currentChild = mFirstChild;
				while (currentChild->GetNextSibling())
					currentChild = currentChild->GetNextSibling();

				currentChild->SetNextSibling(std::move(child));
			}
		}

		/**
		 * \brief 
		 * \param child 
		 * \return 
		 */
		bool RemoveChild(const Ref<T>& child)
		{
			if (child->GetParent() != ConcreteSelf())
				return false;

			Ref<T> currentChild = mFirstChild;
			while (currentChild)
			{
				if (currentChild == child)
				{
					auto previousSibling = currentChild->GetPreviousSibling();
					previousSibling->SetNextSibling(currentChild->GetNextSibling());
					return true;
				}

				currentChild = currentChild->GetNextSibling();
			}

			return false;
		}

		template <typename Callable>
		void Visit(Callable&& callable)
		{
			if (mFirstChild)
				mFirstChild->Visit(callable);

			callable(ConcreteSelf());

			if (mNextSibling)
				mNextSibling->Visit(callable);
		}

		/**
		 * \brief 
		 * \return 
		 */
		Ref<T> GetParent() const
		{
			return mParent.IsExpired() ? nullptr : mParent.ToStrong();
		}

		/**
		 * \brief 
		 * \return 
		 */
		Ref<T> GetPreviousSibling() const
		{
			return mPreviousSibling.IsExpired() ? nullptr : mParent.ToStrong();
		}

		/**
		 * \brief 
		 * \return 
		 */
		Ref<T> GetNextSibling() const
		{
			return mNextSibling;
		}

		/**
		 * \brief 
		 * \return 
		 */
		Ref<T> GetFirstChild() const
		{
			return mFirstChild;
		}

	protected:

		/**
		 * \brief 
		 * \param parent 
		 */
		void SetParent(const Ref<T>& parent)
		{
			Ref<T> self = ConcreteSelf();
			if (Ref<T> currentParent = GetParent())
				currentParent->RemoveChild(self);

			parent->InsertChild(self);
		}

		/**
		 * \brief 
		 * \param nextSibling 
		 */
		void SetNextSibling(Ref<T> nextSibling)
		{
			nextSibling->mPreviousSibling = ConcreteWeakSelf();
			mNextSibling = std::move(nextSibling);
		}

	private:

		Ref<T> ConcreteSelf()
		{
			return T::Cast(Implements<AcyclicGraphNode, Interface>::Self());
		}

		WeakRef<T> ConcreteWeakSelf()
		{
			return T::Cast(Implements<AcyclicGraphNode, Interface>::WeakSelf());
		}

		WeakRef<T> mParent;
		Ref<T> mFirstChild;
		WeakRef<T> mPreviousSibling;
		Ref<T> mNextSibling;
	};
}

#endif // COCKTAIL_CORE_UTILITY_ACYCLICGRAPH_HPP
