#ifndef COCKTAILENGINE_CORE_EXCEPTION_HPP
#define COCKTAILENGINE_CORE_EXCEPTION_HPP

#include <exception>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include <CocktailEngine/Core/String.hpp>

#define COCKTAIL_DECLARE_EXCEPTION_FROM(__Name, __Base)                                               \
	class __Name : public __Base                                                                      \
	{                                                                                                 \
	public:                                                                                           \
		static_assert(std::is_base_of_v<::Ck::Exception, __Base>);                                    \
                                                                                                      \
		template <typename... Args>                                                                   \
		static ::Ck::String FormatExceptionMessage(::Ck::StringView format, Args&&... args)           \
		{                                                                                             \
			return ::Ck::String::Format(                                                              \
						CK_TEXT("%s : %s"),                                                           \
						CK_TEXT(#__Name),                                                             \
						::Ck::String::Format(format, Forward<Args>(args)...)                     \
			);                                                                                        \
		}                                                                                             \
		                                                                                              \
		__Name() = default;                                                                           \
		                                                                                              \
		explicit __Name(::Ck::String message) :                                                       \
			__Base(Move(message))                                                                \
		{                                                                                             \
		}                                                                                             \
		                                                                                              \
		 __Name(const ::Ck::Exception& nested, ::Ck::String message) :                                \
			__Base(nested, Move(message))                                                        \
		{                                                                                             \
		}                                                                                             \
                                                                                                      \
		template <typename... Args>                                                                   \
		explicit __Name(::Ck::StringView format, Args&&... args) :                                    \
			__Base(FormatExceptionMessage(format, Forward<Args>(args)...))                       \
		{		                                                                                      \
		}                                                                                             \
                                                                                                      \
		template <typename... Args>                                                                   \
		explicit __Name(const ::Ck::Exception& nested, ::Ck::StringView format, Args&&... args) :     \
			__Base(nested, FormatExceptionMessage(format, Forward<Args>(args)...))               \
		{		                                                                                      \
		}                                                                                             \
                                                                                                      \
		char const* what() const noexcept override { return #__Name; }                                \
					                                                                                  \
		::Ck::StringView GetName() const override { return CK_TEXT(#__Name); }                        \
                                                                                                      \
	protected:                                                                                        \
		__Name* Clone() const override { return new __Name(*this); }                                  \
	}                                                                                                 

#define COCKTAIL_DECLARE_EXCEPTION(__Name) COCKTAIL_DECLARE_EXCEPTION_FROM(__Name, ::Ck::Exception)

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API Exception : public std::exception
	{
	public:

		Exception();

		/**
		 * \brief 
		 * \param message 
		 */
		explicit Exception(String message);

		/**
		 * \brief 
		 * \param message 
		 * \param nested 
		 */
		Exception(const Exception& nested, String message);

		/**
		 * \brief 
		 * \param other 
		 */
		Exception(const Exception& other);

		/**
		 * \brief 
		 * \param other 
		 */
		Exception(Exception&& other) noexcept;

		/**
		 * \brief
		 */
		~Exception() override;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Exception& operator=(const Exception& other);

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Exception& operator=(Exception&& other) noexcept;

		/**
		 * \brief 
		 * \return 
		 */
		char const* what() const noexcept override;

		/**
		 * \brief 
		 * \return 
		 */
		virtual StringView GetName() const;

		/**
		 * \brief 
		 * \return 
		 */
		const String& GetMessage() const;

	protected:

		/**
		 * \brief 
		 * \return 
		 */
		virtual Exception* Clone() const;

	private:

		String mMessage;
		Exception* mNested;
	};

	COCKTAIL_DECLARE_EXCEPTION(LogicException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(InvalidParameterException, LogicException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(NotImplementedException, LogicException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(ContainerEmpty, LogicException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(ContainerOutOfRange, LogicException);

	COCKTAIL_DECLARE_EXCEPTION(RuntimeException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(OutOfMemory, RuntimeException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(EmptyOptionalException, RuntimeException);

	COCKTAIL_DECLARE_EXCEPTION_FROM(EncodingException, RuntimeException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(CodepointEncodingException, EncodingException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(CodepointDecodingException, EncodingException);
}

#endif // COCKTAILENGINE_CORE_EXCEPTION_HPP
