#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_THREADLOCALSINGLETON_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_THREADLOCALSINGLETON_HPP

#include <Cocktail/Core/Application/Detail/CallableServiceBinding.hpp>
#include <Cocktail/Core/System/Concurrency/ThreadLocal.hpp>

namespace Ck::Detail
{
    template <typename T>
    class ThreadLocalSingletonServiceBinding : public CallableServiceBinding<T>
    {
    public:

        /**
         * \brief
         * \param owner
         * \param resolver
         */
        ThreadLocalSingletonServiceBinding(ServiceContainer* owner, ServiceResolver<T> resolver) :
            mInstance(MakeFactory(owner, std::move(resolver)))
        {
            /// Nothing
        }

        /**
         * \brief
         * \return
         */
        T* Resolve() override
        {
            return mInstance.Get();
        }

    private:

        typename ThreadLocal<T>::Factory MakeFactory(ServiceContainer* owner, ServiceResolver<T>&& resolver)
        {
            return [this, o = owner, r = std::move(resolver)]() {
                return this->InvokeDecorators(r(o));
            };
        }

        ThreadLocal<T> mInstance;
    };
}

#endif //COCKTAIL_CORE_APPLICATION_DETAIL_THREADLOCALSINGLETON_HPP
