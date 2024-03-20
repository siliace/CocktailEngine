#ifndef COCKTAIL_CORE_LOG_LOG_HPP
#define COCKTAIL_CORE_LOG_LOG_HPP

#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Application/ServiceFacade.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class Log : public ServiceFacade<LogManager>
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
		 * \param args
		 * \return
		 */
		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<LogChannel, T>>>
		static Ref<T> CreateChannel(std::string_view name, Args&&... args)
		{
			return ResolveFacadeInstance()->CreateChannel<T>(name, std::forward<Args>(args)...);
		}
	};
}

#endif // COCKTAIL_CORE_LOG_LOG_HPP
