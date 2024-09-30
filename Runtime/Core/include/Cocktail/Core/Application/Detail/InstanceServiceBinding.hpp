#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_INSTANCESERVICEBINDING_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_INSTANCESERVICEBINDING_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Application/Detail/ServiceBinding.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

namespace Ck::Detail
{
	template <typename T>
	class InstanceServiceBinding : public Inherit<InstanceServiceBinding<T>, Object, ServiceBinding<T>>
	{
	public:

		/**
		 * \brief 
		 * \param instance 
		 */
		explicit InstanceServiceBinding(const Ref<T>& instance) :
			mInstance(instance)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \return 
		 */
		Ref<T> Resolve() override
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

		Ref<T> mInstance;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_INSTANCESERVICEBINDING_HPP
