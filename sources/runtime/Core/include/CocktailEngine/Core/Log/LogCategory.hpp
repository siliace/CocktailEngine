#ifndef COCKTAILENGINE_CORE_LOG_LOGCATEGORY_HPP
#define COCKTAILENGINE_CORE_LOG_LOGCATEGORY_HPP

#include <string>

#include <CocktailEngine/Core/Enum.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/Log/LogLevel.hpp>

#define COCKTAIL_DECLARE_LOG_CATEGORY(__Name, __Level)       \
extern class __Name ## Type : public ::Ck::LogCategory \
{                                                      \
public:                                                \
    __Name ## Type() :                                 \
       ::Ck::LogCategory(CK_TEXT(#__Name), __Level)    \
    {                                                  \
    }                                                  \
} __Name                                               \

#define COCKTAIL_DEFINE_LOG_CATEGORY(__Name)  __Name ## Type __Name
 
namespace Ck
{
    /**
     * \brief
     */
    class COCKTAILENGINE_CORE_API LogCategory
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
        StringView GetName() const;

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
        LogCategory(StringView name, LogLevel defaultLevel);

    private:

        StringView mName;
        LogLevel mDefaultLogLevel;
        LogLevel mLogLevel;
    };
}

#endif // COCKTAILENGINE_CORE_LOG_LOGCATEGORY_HPP
