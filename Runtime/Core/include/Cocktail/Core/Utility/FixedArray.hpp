#ifndef COCKTAIL_CORE_UTILITY_FIXEDARRAY_HPP
#define COCKTAIL_CORE_UTILITY_FIXEDARRAY_HPP

#include <memory>
#include <stdexcept>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class FixedArray
	{
	public:

		/**
		 * \brief 
		 */
		FixedArray() :
			mSize(0)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param size 
		 */
		explicit FixedArray(std::size_t size) :
			mSize(size)
		{
			if (mSize)
				mElements = std::make_unique<T[]>(mSize);
		}

		/**
		 * \brief 
		 * \param list 
		 */
		FixedArray(std::initializer_list<T> list) :
			FixedArray(list.begin(), list.end())
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \tparam It 
		 * \param begin 
		 * \param end 
		 */
		template <typename It>
		FixedArray(It begin, It end) :
			FixedArray(std::distance(begin, end))
		{
			std::copy(begin, end, mElements.get());
		}

		/**
		 * \brief 
		 * \param other 
		 */
		FixedArray(const FixedArray& other) :
			mSize(other.mSize)
		{
			std::copy(std::begin(other), std::end(other), mElements.get());
		}

		/**
		 * \brief 
		 * \param other 
		 */
		FixedArray(FixedArray&& other) noexcept
		{
			*this = std::move(other);
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		FixedArray& operator=(const FixedArray& other)
		{
			if (this == &other)
				return *this;

			mSize = other.mSize;
			mElements = std::make_unique<T[]>(mSize);
			std::copy(std::begin(other), std::end(other), mElements.get());

			return *this;
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		FixedArray& operator=(FixedArray&& other) noexcept
		{
			if (this == &other)
				return *this;

			mSize = std::exchange(other.mSize, 0);
			mElements = std::move(other.mElements);

			return *this;
		}

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		T& At(std::size_t index)
		{
			if (index >= mSize)
				throw std::out_of_range("index ouf of FixedArray range");

			return mElements[index];
		}

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		const T& At(std::size_t index) const
		{
			if (index >= mSize)
				throw std::out_of_range("index ouf of FixedArray range");

			return mElements[index];
		}

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEmpty() const
		{
			return mSize == 0;
		}

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetSize() const
		{
			return mSize;
		}

		/**
		 * \brief 
		 * \return 
		 */
		T* GetData()
		{
			return mElements.get();
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T* GetData() const
		{
			return mElements.get();
		}

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		T& operator[](std::size_t index)
		{
			return mElements[index];
		}

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		const T& operator[](std::size_t index) const
		{
			return mElements[index];
		}

	private:

		std::size_t mSize;
		std::unique_ptr<T[]> mElements;
	};
}

namespace std
{
	template <typename T>
	T* begin(Ck::FixedArray<T>& arr)
	{
		return arr.GetData();
	}

	template <typename T>
	const T* begin(const Ck::FixedArray<T>& arr)
	{
		return arr.GetData();
	}

	template <typename T>
	T* end(Ck::FixedArray<T>& arr)
	{
		return arr.GetData() + arr.GetSize();
	}

	template <typename T>
	const T* end(const Ck::FixedArray<T>& arr)
	{
		return arr.GetData() + arr.GetSize();
	}
}

#endif // COCKTAIL_CORE_UTILITY_FIXEDARRAY_HPP
