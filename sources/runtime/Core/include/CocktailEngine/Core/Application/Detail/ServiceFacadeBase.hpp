#ifndef COCKTAILENGINE_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP
#define COCKTAILENGINE_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Signal/Signal.hpp>

namespace Ck
{
	class Application;

	namespace Detail
	{
		/**
		 * \brief
		 */
		class COCKTAILENGINE_CORE_API ServiceFacadeBase
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

#endif // COCKTAILENGINE_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP
