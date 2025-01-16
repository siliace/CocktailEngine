#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWSTYLE_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWSTYLE_HPP

namespace Ck
{
	/**
     * \brief 
     */
    enum class WindowStyle
    {
	    /**
         * \brief 
         */
        Closable = Bit(0),

        /**
         * \brief 
         */
        Titlebar = Bit(1),

        /**
         * \brief 
         */
        Resizable = Bit(2),

        /**
         * \brief 
         */
        Minimizable = Bit(3),

        /**
         * \brief 
         */
        Maximizable = Bit(4),
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWSTYLE_HPP
