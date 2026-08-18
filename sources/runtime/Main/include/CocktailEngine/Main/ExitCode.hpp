#ifndef COCKTAILENGINE_MAIN_EXITCODE_HPP
#define COCKTAILENGINE_MAIN_EXITCODE_HPP

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

#endif // COCKTAILENGINE_MAIN_EXITCODE_HPP
