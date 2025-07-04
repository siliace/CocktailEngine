#ifndef COCKTAIL_CORE_LOG_LOG_HPP
#define COCKTAIL_CORE_LOG_LOG_HPP

#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Application/ServiceFacade.hpp>

#define CK_LOG(__Category, __Level, __Message, ...) ::Ck::Log::Trace(__Category, __Level, __Message, __FILE__, __LINE__, ## __VA_ARGS__)

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
		 * \tparam T
		 * \tparam Args
		 * \param name
		 * \param logChannel
		 * \return
		 */
		static void RegisterChannel(const std::string& name, std::unique_ptr<LogChannel> logChannel);

		/**
		 * \brief 
		 * \tparam Args 
		 * \param category
		 * \param level 
		 * \param message 
		 * \param file 
		 * \param line 
		 * \param args 
		 */
		template <typename... Args>
		static void Trace(const LogCategory& category, LogLevel level, std::string_view message, std::string_view file, Uint64 line, Args&&... args)
		{
			ResolveFacadeInstance()->Trace(category, level, message, file, line, std::forward<Args>(args)...);
		}
	};
}

#endif // COCKTAIL_CORE_LOG_LOG_HPP
