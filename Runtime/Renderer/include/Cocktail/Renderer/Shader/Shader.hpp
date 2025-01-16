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
    class Shader : public Implements<Shader, RenderDeviceObject>
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
         * \return 
         */
        virtual std::size_t GetInputAttributeCount() const = 0;

	    /**
         * \brief 
         * \param attributeLocations 
         * \param attributeLocationCount 
         * \param firstAttributeLocation 
         * \return 
         */
        virtual std::size_t GetInputAttributes(VertexAttributeLocation** attributeLocations, std::size_t attributeLocationCount, std::size_t firstAttributeLocation = 0) const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual std::size_t GetOutputAttributeCount() const = 0;

	    /**
         * \brief 
         * \param attributeLocations 
         * \param attributeLocationCount 
         * \param firstAttributeLocation 
         * \return 
         */
        virtual std::size_t GetOutputAttributes(VertexAttributeLocation** attributeLocations, std::size_t attributeLocationCount, std::size_t firstAttributeLocation = 0) const = 0;
    };
}

#endif // COCKTAIL_RENDERER_SHADER_SHADER_HPP
