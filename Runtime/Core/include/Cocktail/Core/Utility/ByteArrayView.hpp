#ifndef COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP
#define COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    class ByteArray;

    /**
     * \brief Utility class to access to a memory area
     */
    class COCKTAIL_CORE_API ByteArrayView
    {
    public:

        /**
         * \brief Default constructor
         */
        ByteArrayView();

        /**
         * \brief
         * \param data
         * \param size
         */
        ByteArrayView(const Byte* data, std::size_t size);

        /**
         * \brief 
         * \param byteArray 
         * \param offset 
         */
        ByteArrayView(const ByteArray& byteArray, std::size_t offset = 0);

        /**
         * \brief 
         * \param byteArray 
         * \param offset 
         * \param length 
         */
        ByteArrayView(const ByteArray& byteArray, std::size_t offset, std::size_t length);

        /**
         * \brief 
         * \param other 
         */
        ByteArrayView(const ByteArrayView& other) = default;

        /**
         * \brief 
         * \param other 
         */
        ByteArrayView(ByteArrayView&& other) noexcept;
        
        /**
         * \brief 
         * \param other 
         * \return 
         */
        ByteArrayView& operator=(const ByteArrayView& other) = default;

        /**
         * \brief 
         * \param other 
         * \return 
         */
        ByteArrayView& operator=(ByteArrayView&& other) noexcept;

        /**
         * \brief 
         * \param offset 
         * \return 
         */
        ByteArrayView Slice(std::size_t offset) const;

        /**
         * \brief 
         * \param offset 
         * \param length 
         * \return 
         */
        ByteArrayView Slice(std::size_t offset, std::size_t length) const;

        /**
    	 * \brief 
    	 * \param index 
    	 * \return 
    	 */
    	const Byte& At(std::size_t index) const;

        /**
         * \brief 
         * \return 
         */
        bool IsEmpty() const;

        /**
         * \brief 
         * \return 
         */
        std::size_t GetSize() const;

        /**
         * \brief 
         * \return 
         */
        const Byte* GetData() const;

        bool operator==(const ByteArray& rhs) const;
        bool operator==(const ByteArrayView& rhs) const;

        bool operator!=(const ByteArray& rhs) const;
        bool operator!=(const ByteArrayView& rhs) const;

    private:

        std::size_t mSize;
        const Byte* mData;
    };
}

#endif // COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP
