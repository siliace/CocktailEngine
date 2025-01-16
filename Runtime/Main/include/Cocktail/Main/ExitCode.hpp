#ifndef COCKTAIL_MAIN_EXITCODE_HPP
#define COCKTAIL_MAIN_EXITCODE_HPP

namespace Ck::Main
{
	/**
     * \brief Enumeration of possible exit code from ApplicationMain function
     */
    enum class ExitCode
    {
	    /**
         * \brief 
         */
        GeneralError = -1,
        
        /**
         * \brief 
         */
        Success = 0,
    };
}

#endif // COCKTAIL_MAIN_EXITCODE_HPP
