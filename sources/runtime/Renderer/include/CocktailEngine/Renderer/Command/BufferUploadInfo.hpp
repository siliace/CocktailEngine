#ifndef COCKTAIL_GRAPHIC_COMMANDLIST_BUFFERUPLOADINFO_HPP
#define COCKTAIL_GRAPHIC_COMMANDLIST_BUFFERUPLOADINFO_HPP

#include <utility>

namespace Ck::Renderer
{
    /**
	 * \brief
	 */
    struct BufferUploadInfo
    {
        /**
         * \brief
         */
        std::size_t Offset = 0;

        /**
         * \brief
         */
        std::size_t Length = 0;

        /**
         * \brief
         */
        const void* Data = nullptr;
    };
}

#endif // COCKTAIL_GRAPHIC_COMMANDLIST_BUFFERUPLOADINFO_HPP
