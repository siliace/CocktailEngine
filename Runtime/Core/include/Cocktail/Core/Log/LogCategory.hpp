#ifndef COCKTAIL_CORE_LOG_LOGCATEGORY_HPP
#define COCKTAIL_CORE_LOG_LOGCATEGORY_HPP

#include <string>

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>

#define CK_DECLARE_LOG_CATEGORY(__Name, __Level)       \
extern class __Name ## Type : public ::Ck::LogCategory \
{                                                      \
public:                                                \
    __Name ## Type() :                                 \
       ::Ck::LogCategory(#__Name, __Level)             \
    {                                                  \
    }                                                  \
} __Name;                                              \

#define CK_DEFINE_LOG_CATEGORY(__Name)  __Name ## Type __Name;
 
namespace Ck
{
    /**
     * \brief
     */
    class COCKTAIL_CORE_API LogCategory
    {
    public:
        
        /**
         * \brief Destructor
         */
        virtual ~LogCategory() = default;

        /**
         * \brief Tell whether a LogLevel is suppressed by the category
         */
        bool IsSuppressed(LogLevel level) const
        {
            return Enum<LogLevel>::UnderlyingCast(level) > Enum<LogLevel>::UnderlyingCast(mLogLevel);
        }

        const std::string& GetName() const
        {
            return mName;
        }

        LogLevel GetDefaultLevel() const
        {
            return mDefaultLogLevel;
        }

        LogLevel GetLevel() const
        {
	        return mLogLevel;
        }

        void SetLevel(LogLevel logLevel)
        {
	        mLogLevel = logLevel;
        }

    protected:

        /**
         * \brief Constructor
         * Create a new BaseLogCategory
         * \param name
         * \param defaultLogLevel
         */
        LogCategory(std::string name, LogLevel defaultLogLevel) :
			mName(std::move(name)),
			mDefaultLogLevel(defaultLogLevel)
        {
            mLogLevel = mDefaultLogLevel;
        }

    private:

        std::string mName;
        LogLevel mDefaultLogLevel;
        LogLevel mLogLevel;
    };
}

#endif // COCKTAIL_CORE_LOG_LOGCATEGORY_HPP
