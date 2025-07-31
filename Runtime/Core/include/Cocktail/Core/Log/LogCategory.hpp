#ifndef COCKTAIL_CORE_LOG_LOGCATEGORY_HPP
#define COCKTAIL_CORE_LOG_LOGCATEGORY_HPP

#include <string>

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>

#define COCKTAIL_DECLARE_LOG_CATEGORY(__Name, __Level)       \
extern class __Name ## Type : public ::Ck::LogCategory \
{                                                      \
public:                                                \
    __Name ## Type() :                                 \
       ::Ck::LogCategory(#__Name, __Level)             \
    {                                                  \
    }                                                  \
} __Name                                               \

#define COCKTAIL_DEFINE_LOG_CATEGORY(__Name)  __Name ## Type __Name
 
namespace Ck
{
    /**
     * \brief
     */
    class COCKTAIL_CORE_API LogCategory
    {
    public:

        /**
         * \brief Tell whether a LogLevel is suppressed by the category
         * \param level 
         * \return 
         */
        bool IsSuppressed(LogLevel level) const;

        /**
         * \brief 
         * \return 
         */
        const std::string& GetName() const;

        /**
         * \brief 
         * \return 
         */
        LogLevel GetDefaultLevel() const;

        /**
         * \brief 
         * \return 
         */
        LogLevel GetLevel() const;

        /**
         * \brief 
         * \param level 
         */
        void SetLevel(LogLevel level);

    protected:

        /**
         * \brief Constructor
         * Create a new BaseLogCategory
         * \param name
         * \param defaultLevel
         */
        LogCategory(std::string name, LogLevel defaultLevel);

    private:

        std::string mName;
        LogLevel mDefaultLogLevel;
        LogLevel mLogLevel;
    };
}

#endif // COCKTAIL_CORE_LOG_LOGCATEGORY_HPP
