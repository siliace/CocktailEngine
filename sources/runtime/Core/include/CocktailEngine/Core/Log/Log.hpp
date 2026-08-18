#ifndef COCKTAILENGINE_CORE_LOG_LOG_HPP
#define COCKTAILENGINE_CORE_LOG_LOG_HPP

#include <CocktailEngine/Core/Log/LogManager.hpp>
#include <CocktailEngine/Core/Application/ServiceFacade.hpp>

#define CK_LOG(__Category, __Level, __Message, ...) ::Ck::Log::Trace(__Category, __Level, __Message, CK_TEXT(__FILE__), __LINE__, ## __VA_ARGS__)

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API Log : public ServiceFacade<LogManager>
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
		static void RegisterChannel(const String& name, UniquePtr<LogChannel> logChannel);

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
		static void Trace(const LogCategory& category, LogLevel level, StringView message, StringView file, Uint64 line, Args&&... args)
		{
			ResolveFacadeInstance()->Trace(category, level, message, file, line, Forward<Args>(args)...);
		}
	};
}

#endif // COCKTAILENGINE_CORE_LOG_LOG_HPP
