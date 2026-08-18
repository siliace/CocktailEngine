#ifndef COCKTAILENGINE_CORE_UTILITY_FINALY_HPP
#define COCKTAILENGINE_CORE_UTILITY_FINALY_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

namespace Ck
{
    template <typename TCallable>
    class Finally
    {
    public:

        Finally(TCallable&& callable) :
            mCallable(Forward<TCallable>(callable))
        {
            /// Nothing
        }

        ~Finally()
        {
            mCallable();
        }

    private:

        TCallable mCallable;
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_FINALY_HPP
