#ifndef COCKTAIL_RENDERER_CONTEXT_RENDERSURFACE_HPP
#define COCKTAIL_RENDERER_CONTEXT_RENDERSURFACE_HPP

#include <Cocktail/Core/Extent2D.hpp>

#include <Cocktail/Renderer/RasterizationSamples.hpp>
#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	/**
     * \brief 
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

#endif // COCKTAIL_RENDERER_CONTEXT_RENDERSURFACE_HPP
