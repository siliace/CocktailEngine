#ifndef COCKTAILENGINE_CORE_APPLICATION_DETAIL_INSTANCESERVICEBINDING_HPP
#define COCKTAILENGINE_CORE_APPLICATION_DETAIL_INSTANCESERVICEBINDING_HPP

#include <CocktailEngine/Core/Application/Detail/ServiceBinding.hpp>

namespace Ck::Detail
{
	template <typename T>
	class InstanceServiceBinding : public ServiceBinding<T>
	{
	public:

		/**
		 * \brief 
		 * \param instance 
		 */
		explicit InstanceServiceBinding(T* instance) :
			mInstance(instance)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \return 
		 */
		T* Resolve() override
		{
			return mInstance;
		}

		/**
		 * \brief 
		 * \return 
		 */
		bool IsCallable() const override
		{
			return false;
		}

	private:

		T* mInstance;
	};
}

#endif // COCKTAILENGINE_CORE_APPLICATION_DETAIL_INSTANCESERVICEBINDING_HPP
