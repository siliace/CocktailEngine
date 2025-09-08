#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_MIPMAPSTEXTURERESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_MIPMAPSTEXTURERESOURCE_HPP

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

namespace Ck
{
    class COCKTAIL_GRAPHIC_API MipMapsTextureResource : public TextureResource
    {
    public:
		
	    /**
         * \brief Constructor
         * \param graphicEngine 
         * \param mipMaps 
         * \param name 
         */
        MipMapsTextureResource(std::shared_ptr<GraphicEngine> graphicEngine, std::shared_ptr<MipMaps> mipMaps, const AnsiChar* name = "");

	    /**
         * \brief
         * \param baseLayer 
         * \param baseLevel 
         * \param levelCount 
         */
        void LoadLevels(unsigned int baseLayer, unsigned int baseLevel, unsigned int levelCount);

	    /**
         * 
         * @return 
         */
        std::shared_ptr<MipMaps> GetMipMaps() const;

    private:    

        std::shared_ptr<MipMaps> mMipMaps;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_MIPMAPSTEXTURERESOURCE_HPP
