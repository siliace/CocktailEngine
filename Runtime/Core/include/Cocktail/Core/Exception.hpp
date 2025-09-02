#ifndef COCKTAIL_CORE_EXCEPTION_HPP
#define COCKTAIL_CORE_EXCEPTION_HPP

#include <exception>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include <fmt/core.h>

#define COCKTAIL_DECLARE_EXCEPTION_FROM(__Name, __Base)                                               \
	class __Name : public __Base                                                                      \
	{                                                                                                 \
	public:                                                                                           \
		static_assert(std::is_base_of_v<::Ck::Exception, __Base>);                                    \
                                                                                                      \
		template <typename... Args>                                                                   \
		static std::string FormatExceptionMessage(std::string_view format, Args&&... args)            \
		{                                                                                             \
			return fmt::format("{} : {}", #__Name, fmt::format(format, std::forward<Args>(args)...)); \
		}                                                                                             \
                                                                                                      \
		__Name() = default;                                                                           \
                                                                                                      \
		template <typename... Args>                                                                   \
		explicit __Name(std::string_view format, Args&&... args) :                                    \
			__Base(FormatExceptionMessage(format, std::forward<Args>(args)...))                       \
		{		                                                                                      \
		}                                                                                             \
                                                                                                      \
		template <typename... Args>                                                                   \
		explicit __Name(const ::Ck::Exception& nested, std::string_view format, Args&&... args) :     \
			__Base(nested, FormatExceptionMessage(format, std::forward<Args>(args)...))               \
		{		                                                                                      \
		}                                                                                             \
                                                                                                      \
		const char* GetName() const override { return #__Name; }                                      \
                                                                                                      \
		const char* GetTypeName() const override { return typeid(*this).name(); }                     \
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
	class Exception : public std::exception
	{
	public:

		Exception() :
			mNested(nullptr)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param message 
		 */
		explicit Exception(std::string message) :
			mMessage(std::move(message)),
			mNested(nullptr)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param message 
		 * \param nested 
		 */
		Exception(const Exception& nested, std::string message) :
			mMessage(std::move(message))
		{
			mNested = nested.Clone();
		}

		/**
		 * \brief 
		 * \param other 
		 */
		Exception(const Exception& other) :
			std::exception(other),
			mMessage(other.mMessage)
		{
			mNested = other.mNested ? other.mNested->Clone() : nullptr;
		}

		/**
		 * \brief 
		 * \param other 
		 */
		Exception(Exception&& other) noexcept :
			std::exception(other),
			mMessage(std::move(other.mMessage)),
			mNested(std::exchange(other.mNested, nullptr))
		{
			/// Nothing
		}

		/**
		 * \brief
		 */
		~Exception() override
		{
			delete mNested;
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Exception& operator=(const Exception& other)
		{
			if (this == &other)
				return *this;

			std::exception::operator=(other);
			mMessage = other.mMessage;
			mNested = other.mNested;

			return *this;
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Exception& operator=(Exception&& other) noexcept
		{
			if (this == &other)
				return *this;

			std::exception::operator=(other);
			mMessage = std::move(other.mMessage);
			mNested = std::exchange(other.mNested, nullptr);

			return *this;
		}

		/**
		 * \brief 
		 * \return 
		 */
		char const* what() const noexcept override
		{
			return GetName();
		}

		/**
		 * \brief 
		 * \return 
		 */
		virtual const char* GetName() const
		{
			return "Exception";
		}

		/**
		 * \brief 
		 * \return 
		 */
		virtual const char* GetTypeName() const
		{
			return typeid(*this).name();
		}

		/**
		 * \brief 
		 * \return 
		 */
		const std::string& GetMessage() const
		{
			return mMessage;
		}

	protected:

		/**
		 * \brief 
		 * \return 
		 */
		virtual Exception* Clone() const
		{
			return new Exception(*this);
		}

	private:

		std::string mMessage;
		Exception* mNested;
	};

	COCKTAIL_DECLARE_EXCEPTION(LogicException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(InvalidParameterException, LogicException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(NotImplementedException, LogicException);

	COCKTAIL_DECLARE_EXCEPTION(RuntimeException);
	COCKTAIL_DECLARE_EXCEPTION_FROM(OutOfMemory, RuntimeException);
}

#endif // COCKTAIL_CORE_EXCEPTION_HPP
