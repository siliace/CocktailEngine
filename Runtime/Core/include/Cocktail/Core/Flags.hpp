#ifndef COCKTAIL_CORE_FLAGS_HPP
#define COCKTAIL_CORE_FLAGS_HPP

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Utility/StringFormater.hpp>

namespace Ck
{
	/**
     * \brief 
     * \tparam T 
     */
    template <typename T>
    class Flags
    {
    public:

	    /**
         * \brief 
         */
        using UnderlyingType = std::make_unsigned_t<std::underlying_type_t<T>>;

	    /**
         * \brief 
         * \param args 
         * \return 
         */
        template <typename... Args>
        static Flags Of(Args&&... args)
        {
            Flags flags;
            for (T value : {args...})
                flags |= value;

            return flags;
        }

	    /**
         * \brief 
         * \return 
         */
        static Flags All()
        {
            Flags flags;
            for (T value : Enum<T>::Values)
                flags |= value;

            return flags;
        }

	    /**
         * \brief 
         */
        Flags() :
			mMask(0)
        {
	        /// Nothing
        }

        /**
         * \brief
         * \param value
         */
        Flags(T value) :
			mMask(static_cast<UnderlyingType>(value))
        {
	        /// Nothing
        }

        /**
         * \brief 
         * \param mask 
         */
        Flags(UnderlyingType mask) :
			mMask(mask)
        {
	        /// Nothing
        }

	    /**
	     * \brief 
	     * \param other 
	     */
	    Flags(const Flags<T>& other) = default;


	    /**
    	 * \brief 
    	 * \param other 
    	 */
    	Flags(Flags<T>&& other) noexcept :
			mMask(std::exchange(other.mMask, 0))
    	{
    		/// Nothing
    	}

	    /**
    	 * \brief 
    	 * \param other 
    	 * \return 
    	 */
    	Flags& operator=(const Flags<T>& other) = default;

	    /**
    	 * \brief 
    	 * \param other 
    	 * \return 
    	 */
    	Flags& operator=(Flags<T>&& other) noexcept
        {
            mMask = std::exchange(other.mMask, 0);

            return *this;
        }

	    /**
         * \brief 
         * \param rhs 
         * \return 
         */
        Flags& operator=(T rhs)
        {
            mMask = static_cast<UnderlyingType>(rhs);

            return *this;
        }

        /**
         * \brief
         * \param mask
         * \return
         */
        Flags& operator=(UnderlyingType mask)
        {
            mMask = mask;

            return *this;
        }

	    /**
         * \brief 
         * \return 
         */
        std::size_t GetHashCode() const
    	{
            return std::hash<UnderlyingType>()(mMask);
    	}

	    /**
         * \brief 
         */
        explicit operator UnderlyingType() const
        {
            return mMask;
        }

	    /**
         * \brief 
         */
        operator bool() const
    	{
            return mMask > 0;
    	}

    private:

        UnderlyingType mMask;
    };

    /**
     * \brief 
     * \tparam T 
     * \param value 
     * \return 
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator~(const Flags<T>& value)
    {
        return Flags<T>(~static_cast<typename Flags<T>::UnderlyingType>(value));
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator|(T lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return Flags<T>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator|(const Flags<T>& lhs, T rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return Flags<T>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator|(const Flags<T>& lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return Flags<T>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator|=(Flags<T>& lhs, T rhs)
    {
        return lhs = lhs | rhs;
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator|=(Flags<T>& lhs, const Flags<T>& rhs)
    {
        return lhs = lhs | rhs;
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator&(T lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return Flags<T>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator&(const Flags<T>& lhs, T rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return Flags<T>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator&(const Flags<T>& lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return Flags<T>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator&=(Flags<T>& lhs, T rhs)
    {
        return lhs = lhs & rhs;
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    Flags<T> operator&=(Flags<T>& lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return Flags<T>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator==(typename Flags<T>::UnderlyingType lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return static_cast<Underlying>(lhs) == static_cast<Underlying>(rhs);
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator==(const Flags<T>& lhs, typename Flags<T>::UnderlyingType rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return static_cast<Underlying>(lhs) == static_cast<Underlying>(rhs);
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator==(const Flags<T>& lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return static_cast<Underlying>(lhs) == static_cast<Underlying>(rhs);
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator!=(T lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return static_cast<Underlying>(lhs) != static_cast<Underlying>(rhs);
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator!=(const Flags<T>& lhs, T rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return static_cast<Underlying>(lhs) != static_cast<Underlying>(rhs);
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator!=(const Flags<T>& lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return static_cast<Underlying>(lhs) != static_cast<Underlying>(rhs);
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator>(typename Flags<T>::UnderlyingType lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return lhs > static_cast<Underlying>(rhs);
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator>(const Flags<T>& lhs, typename Flags<T>::UnderlyingType rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return static_cast<Underlying>(lhs) > rhs;
    }

    /**
     * \brief
     * \param lhs
     * \param rhs
     * \return
     */
    template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    bool operator>(const Flags<T>& lhs, const Flags<T>& rhs)
    {
        using Underlying = typename Flags<T>::UnderlyingType;
        return static_cast<Underlying>(lhs) > static_cast<Underlying>(rhs);
    }

    template <typename T>
    struct Formatter<Flags<T>, void>
    {
        using FlagsType = Flags<T>;
        using IntegerType = typename Flags<T>::UnderlyingType;

        IntegerType Apply(const FlagsType& value) const noexcept
        {
            return value.GetHashCode();
        }
    };
}

namespace std
{
    template <typename T>
    struct hash<Ck::Flags<T>>
    {
	    size_t operator()(const Ck::Flags<T>& flags) const noexcept
	    {
            return flags.GetHashCode();
	    }
    };
}

#endif // COCKTAIL_CORE_FLAGS_HPP
