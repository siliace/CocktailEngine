#ifndef COCKTAIL_CORE_MEMORY_STORAGEREFCOUNTER_HPP
#define COCKTAIL_CORE_MEMORY_STORAGEREFCOUNTER_HPP

#include <Cocktail/Core/Memory/RefCounter.hpp>

namespace Ck::Detail
{
	/**
	 * \brief RefCounter holding also the counted object to improve memory locality 
	 */
	template <typename T>
	class StorageRefCounter : public RefCounter
	{
	public:

		/**
		 * \brief Constructor
		 * \tparam Args 
		 * \param args Arguments to use to construct the held object
		 */
		template <typename... Args>
		explicit StorageRefCounter(Args&&... args) :
			mStorage{}
		{
			new (&mStorage[0]) T(std::forward<Args>(args)...);
		}

		/**
		 * \brief Get a pointer to the held object
		 * \return The object
		 */
		T* GetPointer()
		{
			return reinterpret_cast<T*>(&mStorage[0]);
		}

	protected:

		/**
		 * \brief Mimic destruction of the held object
		 * This invoke directly the destructor of the held object.
		 */
		void Destroy() override
		{
			GetPointer()->~T();
		}

		/**
		 * \brief Dispose this counter
		 */
		void Suicide() override
		{
			delete this;
		}

	private:

		unsigned char mStorage[sizeof(T)];
	};
}

#endif // COCKTAIL_CORE_MEMORY_STORAGEREFCOUNTER_HPP
