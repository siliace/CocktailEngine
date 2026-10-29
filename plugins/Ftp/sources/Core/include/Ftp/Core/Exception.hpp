#ifndef COCKTAILENGINE_FTP_CORE_EXCEPTION_HPP
#define COCKTAILENGINE_FTP_CORE_EXCEPTION_HPP

#include <CocktailEngine/Core/GenericException.hpp>
#include <CocktailEngine/Core/Utility/TranslatorCast.hpp>

namespace Ck
{
    namespace Ftp
    {
        class ErrorCodeTranslator : public Ck::ErrorCodeTranslator<unsigned int>
        {
        public:

            String TranslateErrorCode(unsigned int error) const override
            {
                return TranslatorCast<String>(error);
            }
        };

        using Exception = GenericException<unsigned int, ErrorCodeTranslator>;
    }

    using FtpException = Ftp::Exception;
    using FtpErrorCodeTranslator = Ftp::ErrorCodeTranslator;
}

#endif // COCKTAILENGINE_FTP_CORE_EXCEPTION_HPP
