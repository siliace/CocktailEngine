#ifndef COCKTAILENGINE_RENDERER_CONTEXT_RENDERSURFACE_HPP
#define COCKTAILENGINE_RENDERER_CONTEXT_RENDERSURFACE_HPP

#include <CocktailEngine/Core/Extent2D.hpp>

#include <CocktailEngine/Renderer/RasterizationSamples.hpp>
#include <CocktailEngine/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	/**
     * \brief Interface representing an opaque handle to a GPU surface object
     * A RenderSurface is an abstraction of a native platform surface or window.
     */
    class RenderSurface : public RenderDeviceObject
    {
    public:

        /**
         * \brief 
         * \return 
         */
        virtual Extent2D<unsigned int> GetSize() const = 0;

        /**
         * \brief 
         * \return 
         */
        virtual PixelFormat GetColorFormat() const = 0;

        /**
         * \brief 
         * \return 
         */
        virtual PixelFormat DepthStencilFormat() const = 0;

        /**
         * \brief
         * \return
         */
        virtual RasterizationSamples GetSamples() const = 0;

        /**
         * \brief
         * \return
         */
        virtual bool IsVsyncEnabled() const = 0;

        /**
         * \brief Enable or disable vertical synchronization
         * \param enable
         */
        virtual void EnableVSync(bool enable = true) = 0;

        /**
         * \brief
         * \return
         */
        virtual unsigned int GetBufferCount() const = 0;
    };
}

#endif // COCKTAILENGINE_RENDERER_CONTEXT_RENDERSURFACE_HPP
