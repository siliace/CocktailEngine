#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_CURSOR_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_CURSOR_HPP

namespace Ck
{
	/**
     * \brief 
     */
    class Cursor
    {
    public:

	    /**
	     * \brief 
	     */
	    virtual ~Cursor() = default;

	    /**
         * \brief 
         * \return 
         */
        virtual void* GetSystemHandle() const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_CURSOR_HPP
