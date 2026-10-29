#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_PTHREADEXCEPTION_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_PTHREADEXCEPTION_HPP

#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck::Detail::Pthread
{
    using PthreadException = GenericException<unsigned int, SystemErrorCodeTranslator>;
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_PTHREADEXCEPTION_HPP
