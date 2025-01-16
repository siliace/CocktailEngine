#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_MIPMAPSTEXTURERESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_MIPMAPSTEXTURERESOURCE_HPP

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

namespace Ck
{
    class COCKTAIL_GRAPHIC_API MipMapsTextureResource : public Extends<MipMapsTextureResource, TextureResource>
    {
    public:
		
	    /**
         * \brief Constructor
         * \param graphicEngine 
         * \param mipMaps 
         * \param name 
         */
        MipMapsTextureResource(Ref<GraphicEngine> graphicEngine, Ref<MipMaps> mipMaps, std::string_view name = "");

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
        Ref<MipMaps> GetMipMaps() const;

    private:    

        Ref<MipMaps> mMipMaps;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_MIPMAPSTEXTURERESOURCE_HPP
