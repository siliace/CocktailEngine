#ifndef COCKTAILENGINE_RENDERER_SHADER_VERTEXATTRIBUTELOCATION_HPP
#define COCKTAILENGINE_RENDERER_SHADER_VERTEXATTRIBUTELOCATION_HPP

#include <CocktailEngine/Core/DataType.hpp>
#include <CocktailEngine/Core/String.hpp>

namespace Ck::Renderer
{
    /**
     * \brief
     */
    class VertexAttributeLocation
    {
    public:

	    /**
	     * \brief 
	     */
	    virtual ~VertexAttributeLocation() = default;

	    /**
         * \brief
         * \return
         */
        virtual unsigned int GetLocation() const = 0;

        /**
         * \brief 
         * \return 
         */
        virtual const DataType& GetDataType() const = 0;

        /**
         * \brief 
         * \return 
         */
        virtual unsigned int GetElementCount() const = 0;

        /**
         * \brief
         * \return
         */
        virtual const AsciiString& GetName() const = 0;
    };
}

#endif // COCKTAILENGINE_RENDERER_SHADER_VERTEXATTRIBUTELOCATION_HPP
