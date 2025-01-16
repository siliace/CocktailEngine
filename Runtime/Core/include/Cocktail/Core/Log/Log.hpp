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
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		static void Debug(std::string_view message, Args&&... args)
		{
			ResolveFacadeInstance()->Debug(message, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		static void Info(std::string_view message, Args&&... args)
		{
			ResolveFacadeInstance()->Info(message, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		static void Warning(std::string_view message, Args&&... args)
		{
			ResolveFacadeInstance()->Warning(message, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		static void Error(std::string_view message, Args&&... args)
		{
			ResolveFacadeInstance()->Error(message, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		static void Critical(std::string_view message, Args&&... args)
		{
			ResolveFacadeInstance()->Critical(message, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam Args
		 * \param level
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		static void Trace(LogLevel level, std::string_view message, Args&&... args)
		{
			ResolveFacadeInstance()->Trace(level, message, std::forward<Args>(args)...);
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
