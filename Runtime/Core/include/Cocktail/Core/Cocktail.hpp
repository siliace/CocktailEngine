#ifndef COCKTAIL_CORE_COCKTAIL_HPP
#define COCKTAIL_CORE_COCKTAIL_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <limits>
#include <type_traits>
#include <utility>

#define COCKTAIL_MAJOR_VERSION 0
#define COCKTAIL_MINOR_VERSION 1
#define COCKTAIL_PATCH_VERSION 0

#ifdef _WIN32
	#define COCKTAIL_OS_WINDOWS
	#ifdef _WIN64
		#define COCKTAIL_OS_WINDOWS_64
	#endif
#endif

#ifdef __linux__
	#define COCKTAIL_OS_GNU_LINUX
#endif

#if defined _MSC_VER
	#define COCKTAIL_COMPILER_MSC
#elif defined __GNUC__
	#if defined __clang__
		#define COCKTAIL_COMPILER_CLANG
		#define COCKTAIL_COMPILER_CLANG_VERSION_MAJOR __clang_major__
		#define COCKTAIL_COMPILER_CLANG_VERSION_MINOR __clang_minor__
	#else
		#define COCKTAIL_COMPILER_GCC
		#define COCKTAIL_COMPILER_GCC_VERSION_MAJOR __GNUC__
		#define COCKTAIL_COMPILER_GCC_VERSION_MINOR __GNUC_MINOR__
	#endif
#endif

#ifndef COCKTAIL_STATIC
	#if defined COCKTAIL_COMPILER_MSC
		#define COCKTAIL_EXPORT_API __declspec(dllexport)
		#define COCKTAIL_IMPORT_API __declspec(dllimport)
	#elif defined COCKTAIL_COMPILER_GCC
		#if COCKTAIL_COMPILER_GCC_VERSION_MAJOR >= 4
			#define COCKTAIL_EXPORT_API __attribute__ ((__visibility__ ("default")))
			#define COCKTAIL_IMPORT_API __attribute__ ((__visibility__ ("default")))
		#else
			#define COCKTAIL_EXPORT_API
			#define COCKTAIL_IMPORT_API
		#endif
	#else
		#define COCKTAIL_EXPORT_API
		#define COCKTAIL_IMPORT_API
	#endif
#else
	#define COCKTAIL_EXPORT_API
	#define COCKTAIL_IMPORT_API
#endif

#ifndef COCKTAIL_UNREACHABLE
	#if defined COCKTAIL_COMPILER_MSC
		#define COCKTAIL_UNREACHABLE() __assume(0)
	#elif defined COCKTAIL_COMPILER_GCC
		#define COCKTAIL_UNREACHABLE() __builtin_unreachable()
	#elif defined COCKTAIL_COMPILER_CLANG
		#define COCKTAIL_UNREACHABLE() assert(false && "Unreachable code reached")
	#else
		#define COCKTAIL_UNREACHABLE()
	#endif
#endif

#ifndef COCKTAIL_STACK_ALLOC
	#define COCKTAIL_STACK_ALLOC(__Type, __Number) static_cast<__Type*>(alloca(sizeof(__Type) * (__Number)))
#endif

#ifndef COCKTAIL_CONCATENATE_STRING
	#ifdef COCKTAIL_CONCATENATE_STRING2
		#undef COCKTAIL_CONCATENATE_STRING2
	#endif

	#define COCKTAIL_CONCATENATE_STRING2(__Left, __Right) __Left ## __Right
	#define COCKTAIL_CONCATENATE_STRING(__Left, __Right) COCKTAIL_CONCATENATE_STRING2(__Left, __Right)
#endif

#ifndef COCKTAIL_DECLARE_TAG
	#define COCKTAIL_DECLARE_TAG(__Name) struct __Name##Tag{}; constexpr __Name##Tag __Name;
#endif // COCKTAIL_DECLARE_TAG

namespace Ck
{
	using Uint8 = std::uint8_t;
	using Uint16 = std::uint16_t;
	using Uint32 = std::uint32_t;
	using Uint64 = std::uint64_t;
	using Int8 = std::int8_t;
	using Int16 = std::int16_t;
	using Int32 = std::int32_t;
	using Int64 = std::int64_t;

	COCKTAIL_DECLARE_TAG(InPlace);
	COCKTAIL_DECLARE_TAG(InOptional);

	/**
	 * \brief 
	 * \tparam T 
	 * \param index 
	 * \return 
	 */
	template <typename T>
	constexpr T Bit(T index)
	{
		return 0x1 << index;
	}

	/**
	 * \brief Assign a value to a variable only if its new value is different of its current one
	 * \tparam T 
	 * \tparam U 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T, typename U>
	constexpr bool CheckedAssign(T& lhs, const U& rhs)
	{
		if (lhs == rhs)
			return false;

		lhs = rhs;
		return true;
	}

	template <typename T>
	constexpr T AbsoluteDifference(const T& lhs, const T& rhs)
	{
		return std::max(lhs, rhs) - std::min(lhs, rhs);
	}

	template <typename T>
	constexpr bool NearlyEqual(T lhs, T rhs, T epsilon = std::numeric_limits<T>::epsilon())
	{
		return AbsoluteDifference(lhs, rhs) < epsilon;
	}

	constexpr std::size_t HashMerge(std::size_t lhs, std::size_t rhs)
	{
		std::size_t merge = lhs;
		merge ^= rhs + 0x9e3779b9 + (merge << 6) + (merge >> 2);
		return merge;
	}

	template <typename T>
	constexpr void HashCombine(std::size_t& hash, const T& value)
	{
		hash = HashMerge(hash, std::hash<T>()(value));
	}

	template <typename T>
	T NextPowerOfTwo(T n)
	{
		--n;

		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;

		return n + 1;
	}

	template <typename T, typename... TArgs>
	void Construct(T* element, TArgs... args)
	{
		new (element) T(std::forward<TArgs>(args)...);
	}

	template <typename T, typename TSizeType, typename... TArgs>
	void ConstructRange(TSizeType size, T* elements, TArgs... args)
	{
		static_assert(std::is_integral_v<TSizeType>, "TSizeType must be an integral type");

		for (TSizeType i = 0; i < size; ++i)
			new (&elements[i]) T(std::forward<TArgs>(args)...);
	}

	template <typename T, typename TSizeType>
	void MoveRange(TSizeType size, T* destination, T* source)
	{
		for (TSizeType i = 0; i < size; ++i)
		{
			T& element = source[i];
			new (&destination[i]) T(std::move(element));
			element.~T();
		}
	}

	template <typename T>
	void Destroy(T* element)
	{
		if constexpr (!std::is_trivially_destructible_v<T>)
			element->~T();
	}

	template <typename T, typename TSizeType>
	void DestroyRange(TSizeType size, T* elements)
	{
		static_assert(std::is_integral_v<TSizeType>, "TSizeType must be an integral type");

		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			for (TSizeType i = 0; i < size; ++i)
				elements[i].~T();
		}
	}

	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T operator~(T value)
	{
		using Underlying = std::underlying_type_t<T>;
		return static_cast<T>(~static_cast<Underlying>(value));
	}

	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T operator|(T left, T right)
	{
		using Underlying = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<Underlying>(left) | static_cast<Underlying>(right));
	}

	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T& operator|=(T& left, T right)
	{
		return left = left | right;
	}

	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T operator&(T left, T right)
	{
		using Underlying = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<Underlying>(left) & static_cast<Underlying>(right));
	}

	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T& operator&=(T& left, T right)
	{
		return left = left & right;
	}

	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T operator^(T left, T right)
	{
		using Underlying = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<Underlying>(left) ^ static_cast<Underlying>(right));
	}

	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	constexpr T& operator^=(T& left, T right)
	{
		return left = left ^ right;
	}
}

#endif // COCKTAIL_CORE_COCKTAIL_HPP
