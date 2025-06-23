#ifndef COCKTAIL_CORE_LOG_LOGCATEGORY_HPP
#define COCKTAIL_CORE_LOG_LOGCATEGORY_HPP

#include <string>

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>

#define CK_DECLARE_LOG_CATEGORY(__Name) extern __Name __Name;

#define CK_DEFINE_LOG_CATEGORY(__Name, __Level)      \
class __Name : public Ck::LogCategory<::Ck::__Level> \
{                                                    \
public:                                              \
    __Name() :                                       \
        LogCategory<::Ck::__Level>(#__Name)          \
    {                                                \
    }                                                \
} __Name ## LogCategory;                             \

namespace Ck
{
    /**
     * \brief
     */
    class COCKTAIL_CORE_API BaseLogCategory
    {
    public:
        
        /**
         * \brief Destructor
         */
        virtual ~BaseLogCategory() = default;

        /**
         * \brief Tell whether a LogLevel is suppressed by the category
         */
        bool IsSuppressed(LogLevel level) const
        {
            return Enum<LogLevel>::UnderlyingCast(level) > Enum<LogLevel>::UnderlyingCast(mLogLevel);
        }

    protected:

        /**
         * \brief Constructor
         * Create a new BaseLogCategory
         * \param name
         * \param defaultLogLevel
         */
        BaseLogCategory(std::string name, LogLevel defaultlLogLevel);

    private:

        const std::string mName;
        const LogLevel mDefaultLogLevel;
        LogLevel mLogLevel;
    };

    template <LogLevel DefaultLogLevel>
    class LogCategory : public BaseLogCategory
    {
    protected:

        explicit LogCategory(std::string name) :
            BaseLogCategory(std::move(name), DefaultLogLevel)
        {
            /// Nothing
        }
    };
}

#endif // COCKTAIL_CORE_LOG_LOGCATEGORY_HPP
