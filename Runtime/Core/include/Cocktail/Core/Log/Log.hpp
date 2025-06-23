#ifndef COCKTAIL_CORE_LOG_LOG_HPP
#define COCKTAIL_CORE_LOG_LOG_HPP

#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Application/ServiceFacade.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Log : public ServiceFacade<LogManager>
	{
	public:
		
		/**
		 * 
		 */
		template <typename... Args>
		static void Trace(const BaseLogCategory& category, LogLevel level, std::string_view message, std::string_view file, Uint64 line, Args&&... args)
		{
			ResolveFacadeInstance()->Trace(category, level, message, file, line, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam T
		 * \tparam Args
		 * \param name
		 * \param logChannel
		 * \return
		 */
		static void RegisterChannel(const std::string& name, std::unique_ptr<LogChannel> logChannel);

		/**
		 * \brief
		 * \return
		 */
		static LogLevel GetLevel();

		/**
		 * \brief
		 * \param level
		 */
		static void SetLevel(LogLevel level);
	};
}

#endif // COCKTAIL_CORE_LOG_LOG_HPP
