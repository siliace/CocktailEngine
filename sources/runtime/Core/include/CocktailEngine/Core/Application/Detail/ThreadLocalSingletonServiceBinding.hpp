#ifndef COCKTAILENGINE_CORE_APPLICATION_DETAIL_THREADLOCALSINGLETON_HPP
#define COCKTAILENGINE_CORE_APPLICATION_DETAIL_THREADLOCALSINGLETON_HPP

#include <CocktailEngine/Core/Application/Detail/CallableServiceBinding.hpp>
#include <CocktailEngine/Core/System/Concurrency/ThreadLocal.hpp>

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
            mInstance(MakeFactory(owner, Move(resolver)))
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
            return [this, o = owner, r = Move(resolver)]() {
                return this->InvokeDecorators(r(o));
            };
        }

        ThreadLocal<T> mInstance;
    };
}

#endif //COCKTAILENGINE_CORE_APPLICATION_DETAIL_THREADLOCALSINGLETON_HPP
