#ifndef COCKTAIL_CORE_COCKTAIL_HPP
#define COCKTAIL_CORE_COCKTAIL_HPP

#include <algorithm>
#include <array>
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

	/**
	 * \brief
	 * \param first
	 * \param second
	 * \param third
	 * \param fourth
	 * \return
	 */
	constexpr Uint32 Pack(Uint8 first, Uint8 second, Uint8 third, Uint8 fourth)
	{
		return static_cast<Uint32>(first << 24 | second << 16 | third << 8 | fourth);
	}

	/**
	 * \brief 
	 * \param bytes 
	 * \return 
	 */
	constexpr Uint32 Pack(const std::array<Uint8, 4>& bytes)
	{
		return Pack(bytes[0], bytes[1], bytes[2], bytes[3]);
	}

	/**
	 * \brief 
	 * \param packed 
	 * \return 
	 */
	constexpr std::array<Uint8, 4> Unpack(Uint32 packed)
	{
		std::array<Uint8, 4> bytes{
			static_cast<Uint8>((packed & 0xff000000) >> 24),
			static_cast<Uint8>((packed & 0x00ff0000) >> 16),
			static_cast<Uint8>((packed & 0x0000ff00) >> 8),
			static_cast<Uint8>((packed & 0x000000ff) >> 0)
		};

		return bytes;
	}

	/**
	 * \brief Computes the absolute difference between two values of possibly different types
	 *
	 * This function promotes both operands to their common and returns the absolute difference between them. 
	 *
	 * \tparam T1 Type of the left-hand side operand
	 * \tparam T2 Type of the right-hand side operand
	 * \param lhs The left-hand side operand
	 * \param rhs The right-hand side operand
	 * \return The absolute difference between @p lhs and @p rhs, promoted to the common type
	 *
	 * \note Both values are converted to the common type before comparison
	 *
	 * \code
	 * AbsoluteDifference(5, 2.5);   // returns 2.5 (as double)
	 * AbsoluteDifference(3.0f, 7);  // returns 4.0f (as float)
	 * \endcode
	 */
	template <typename T1, typename T2>
	constexpr std::common_type_t<T1, T2> AbsoluteDifference(const T1& lhs, const T2& rhs)
	{
		using Common = std::common_type_t<T1, T2>;
		return std::max<Common>(lhs, rhs) - std::min<Common>(lhs, rhs);
	}

	/**
	 * \brief Checks whether two values are nearly equal within a tolerance (epsilon)
	 *
	 * This function compares two values of possibly different types for approximate equality.
	 * Both values are promoted to their common type, and the comparison is performed using an absolute
	 * difference check with a given epsilon tolerance.
	 *
	 * \tparam T1 Type of the left-hand side operand
	 * \tparam T2 Type of the right-hand side operand
	 * \param lhs The left-hand side operand
	 * \param rhs The right-hand side operand
	 * \param epsilon The maximum allowed difference between @p lhs and @p rhs for them to be considered equal
	 * \return true if the absolute difference between @p lhs and @p rhs is less than @p epsilon, false otherwise
	 *
	 * \code
	 * NearlyEqual(1.0f, 1.00001f, 1e-4f); // true
	 * NearlyEqual(1.0, 2.0);              // false
	 * NearlyEqual(1.0, 1);                // true (common type is double)
	 * \endcode
	 */
	template <typename T1, typename T2>
	constexpr bool NearlyEqual(T1 lhs, T2 rhs, std::common_type_t<T1, T2> epsilon = std::numeric_limits<std::common_type_t<T1, T2>>::epsilon())
	{
		using Common = std::common_type_t<T1, T2>;
		return AbsoluteDifference<Common>(lhs, rhs) < epsilon;
	}
	/**
	 * \brief Merges two 64-bit hash values into a single hash value
	 *
	 * This function combines two 64-bit hash values using the golden ratio constant
	 * to improve bit diffusion and reduce collisions.
	 *
	 * \param lhs The left-hand side hash value
	 * \param rhs The right-hand side hash value
	 * \return The merged 64-bit hash value
	 */
	constexpr Uint64 HashMerge(Uint64 lhs, Uint64 rhs)
	{
		constexpr Uint64 GoldenRatio = 0x9e3779b97f4a7c15ULL;

		Uint64 merge = lhs;
		merge ^= rhs + GoldenRatio + (merge << 6) + (merge >> 2);
		return merge;
	}
	/**
	 * \brief Combines a hash with the hash of a given value
	 *
	 * This function updates the given hash by merging it with the hash of a new value.
	 *
	 * \tparam T The type of the value to hash. Requires std::hash<T> to be defined
	 * \param hash Reference to the current hash value to update
	 * \param value The value to hash and combine
	 */
	template <typename T>
	constexpr void HashCombine(Uint64& hash, const T& value)
	{
		hash = HashMerge(hash, 
			static_cast<Uint64>(std::hash<T>()(value))
		);
	}

	/**
	 * \brief Computes a combined hash value from multiple values
	 *
	 * This function takes any number of arguments and combines their hashes into a single 64-bit hash value.
	 *
	 * \tparam Ts Variadic template parameter pack of value types
	 * \param values The values to hash and combine
	 * \return A 64-bit hash value representing the combination of all input values
	 *
	 * Example usage:
	 * \code{.cpp}
	 * Uint64 h = HashValues(42, std::string("Hello"), 3.14);
	 * \endcode
	 */
	template <typename... Ts>
	constexpr Uint64 HashValues(const Ts&... values) {
		Uint64 hash = 0;
		(HashCombine(hash, values), ...);
		return hash;
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
