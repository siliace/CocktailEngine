#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_CURSOR_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_CURSOR_HPP

#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class Cursor : public Implements<Cursor, Interface>
    {
    public:

	    /**
         * \brief 
         * \return 
         */
        virtual void* GetSystemHandle() const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_CURSOR_HPP
