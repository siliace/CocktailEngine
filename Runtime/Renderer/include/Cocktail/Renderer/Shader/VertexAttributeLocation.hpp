#ifndef COCKTAIL_RENDERER_SHADER_VERTEXATTRIBUTELOCATION_HPP
#define COCKTAIL_RENDERER_SHADER_VERTEXATTRIBUTELOCATION_HPP

#include <Cocktail/Core/DataType.hpp>
#include <Cocktail/Core/String.hpp>

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
        virtual const AnsiString& GetName() const = 0;
    };
}

#endif // COCKTAIL_RENDERER_SHADER_VERTEXATTRIBUTELOCATION_HPP
