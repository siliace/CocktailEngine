#ifndef COCKTAIL_RENDERER_SHADER_SHADER_HPP
#define COCKTAIL_RENDERER_SHADER_SHADER_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>
#include <Cocktail/Renderer/Shader/ShaderType.hpp>

namespace Ck::Renderer
{
    class VertexAttributeLocation;

	/**
     * \brief 
     */
    class Shader : public RenderDeviceObject
    {
    public:

	    /**
         * \brief 
         */
        static constexpr unsigned int MaxAttributeCount = 32;
        
	    /**
         * \brief 
         * \return 
         */
        virtual ShaderType GetType() const = 0;

	    /**
         * \brief 
         * \param name 
         * \return 
         */
        virtual VertexAttributeLocation* FindInputAttribute(AsciiStringView name) const = 0;

	    /**
         * \brief 
         * \param name 
         * \return 
         */
        virtual VertexAttributeLocation* FindOutputAttribute(AsciiStringView name) const = 0;
    };
}

#endif // COCKTAIL_RENDERER_SHADER_SHADER_HPP
