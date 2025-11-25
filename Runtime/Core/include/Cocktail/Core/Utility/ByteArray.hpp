#ifndef COCKTAIL_CORE_UTILITY_BYTEARRAY_HPP
#define COCKTAIL_CORE_UTILITY_BYTEARRAY_HPP

#include <cstring>

#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/Utility/ByteArrayView.hpp>

namespace Ck
{
	/**
     * \brief Utility class to own and manage a memory area
     */
    class COCKTAIL_CORE_API ByteArray
    {
    public:

        /**
         * \brief Default constructor
         */
        ByteArray();

        /**
         * \brief Constructor
         * Create a ByteArray by allocating and filling its own memory area
         * \param size The size of the memory area to create
         * \param value The value to fill the memory with
         */
        explicit ByteArray(std::size_t size, Uint8 value = 0);

        /**
         * \brief Constructor
         * Create a ByteArray by copying an existing memory area
         * \param data A pointer to the memory area to copy
         * \param length The length to copy
         */
        template <typename T>
        ByteArray(const T* data, std::size_t length)
        {
            assert(data != nullptr);
            assert(length > 0);

            mSize = length;
            if constexpr (!std::is_void_v<T>)
                mSize *= sizeof(T);

            mData = MakeUnique<Uint8[]>(mSize);
            std::memcpy(mData.Get(), data, mSize);
        }

        /**
         * \brief Copy constructor
         * Create a ByteArray by copying ownership of the one of another ByteArray
         * \param other The ByteArray to copy
         */
        ByteArray(const ByteArray& other);

	    /**
         * \brief Move constructor
         * Create a ByteArray by taking ownership of the one of another ByteArray
         * \param other The ByteArray to move
         */
        ByteArray(ByteArray&& other) noexcept;
        
	    /**
         * \brief Assignment operator
         * Assign this ByteArray to a copy of another one
         * \param other The ByteArray to copy
         * \return This
         */
        ByteArray& operator=(const ByteArray& other);

        /**
         * \brief Assignment operator
         * Assign this ByteArray to a move of another one
         * \param other The ByteArray to move
         * \return This
         */
        ByteArray& operator=(ByteArray&& other) noexcept;

        ByteArray& Append(const ByteArray& other);
        ByteArray& Append(const ByteArrayView& other);
        ByteArray& Append(const void* data, std::size_t length);

        ByteArray& Prepend(const ByteArray& other);
        ByteArray& Prepend(const ByteArrayView& other);
        ByteArray& Prepend(const void* data, std::size_t length);

        ByteArray& Insert(std::size_t where, const ByteArray& other);
        ByteArray& Insert(std::size_t where, const ByteArrayView& other);

	    /**
         * \brief Insert a copy of a memory area in the ByteArray
         * \param where The index where copying the memory area
         * \param data A pointer to the memory area to copy
         * \param length The length to copy
         * \return This
         */
        ByteArray& Insert(std::size_t where, const void* data, std::size_t length);

        /**
         * \brief Get a chunk to data in the ByteArray
         * \param offset The offset of the first byte to get
         * \return The chunk
         */
        ByteArray Slice(std::size_t offset) const;

        /**
    	 * \brief Get a chunk to data in the ByteArray
    	 * \param offset The offset of the first byte to get
    	 * \param length The length of data to get
    	 * \return The chunk
    	 */
    	ByteArray Slice(std::size_t offset, std::size_t length) const;

        /**
         * \brief Remove a chunk of data
         * \param offset The offset of the first byte to remove
         * \return This
         */
        ByteArray& Remove(std::size_t offset);

        /**
         * \brief Remove a chunk of data
         * \param offset The offset of the first byte to remove
         * \param length The length of data to remove
         * \return This
         */
        ByteArray& Remove(std::size_t offset, std::size_t length);

        /**
         * \brief Change the length of data
         * If \p newSize is smaller than the current size, data will be truncated
         * If \p newSize is bigger than the current size, the additional data will be filled with \p value
         * \param newSize The new length of data
         * \param value The value to use to init newly allocated memory
         */
        void Resize(std::size_t newSize, Uint8 value = 0);

	    /**
         * \brief Access to the n-th byte of the array
         * \param index The index of the byte
         * \return The n-th byte
         */
        Uint8& At(std::size_t index);

        /**
         * \brief Access to the n-th byte of the array
         * \param index The index of the byte
         * \return The n-th byte
         */
        const Uint8& At(std::size_t index) const;

        /**
         * \brief Tell whether the ByteArray is empty
         * An empty ByteArray means its size is 0
         * \return True if empty, false otherwise
         */
        bool IsEmpty() const;

        std::size_t GetSize() const;
        Uint8* GetData();
        const Uint8* GetData() const;

        bool operator==(const ByteArray& rhs) const;
        bool operator==(const ByteArrayView& rhs) const;

        bool operator!=(const ByteArray& rhs) const;
        bool operator!=(const ByteArrayView& rhs) const;

    private:

        std::size_t mSize;
        UniquePtr<Uint8[]> mData;
    };
}

#endif // COCKTAIL_CORE_UTILITY_BYTEARRAY_HPP
