#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

namespace Ck
{
	class Application;

	namespace Detail
	{
		/**
		 * \brief
		 */
		class COCKTAIL_CORE_API ServiceFacadeBase
		{
		public:

			/**
			 * \brief
			 * \return
			 */
			static Application* GetApplicationFacade();

			/**
			 * \brief
			 * \tparam Callable
			 * \param callable
			 */
			template <typename Callable>
			static void OnApplicationReady(Callable&& callable)
			{
				sReadyConnections.Add(
					sOnFacadeApplicationReady.Connect(callable)
				);
			}

		private:

			friend Application;

			/**
			 * \brief
			 * \param application
			 */
			static void Boot(Application* application);

			/**
			 * \brief
			 */
			static void Terminate();

			static Application* sFacadeApplication;
			static Signal<Application*> sOnFacadeApplicationReady;
			static Array<Connection> sReadyConnections;
		};
	}
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP
