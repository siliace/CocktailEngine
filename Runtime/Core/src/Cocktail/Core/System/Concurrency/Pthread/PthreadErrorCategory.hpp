#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_PTHREADERRORCATEGORY_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_PTHREADERRORCATEGORY_HPP

#include <system_error>

namespace Ck::Detail::Pthread
{
    /**
     * \class PthreadErrorCategory
     * \brief Represents a custom error category for POSIX thread (pthread) error codes
     *
     * This class provides a way to convert pthread error codes into human-readable error messages.
     * It inherits from std::error_category and can be used with std::error_code and std::error_condition
     * to handle thread-related errors in a standardized way.
     */
    class PthreadErrorCategory : public std::error_category
    {
    public:

        /**
         * \brief Singleton instance of the PthreadErrorCategory
         *
         * This static member allows the category to be accessed globally
         * without needing to instantiate the class multiple times.
         */
        static PthreadErrorCategory Instance;

        /**
         * \brief Returns the name of this error category
         *
         * This method provides a short, unique identifier for the category.
         *
         * \return A pointer to a null-terminated string representing the category name
         */
        const char* name() const noexcept override;

        /**
         * \brief Returns a human-readable error message corresponding to a pthread error code
         *
         * This function translates a given error code into a descriptive message
         * that can be displayed to the user or logged for debugging.
         *
         * \param error The integer error code (typically a pthread error code)
         * \return A string containing the corresponding error message
         */
        std::string message(int error) const override;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_PTHREADERRORCATEGORY_HPP
