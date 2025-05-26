#ifndef COCKTAIL_RENDERER_RENDERTARGET_FRAMEBUFFER_HPP
#define COCKTAIL_RENDERER_RENDERTARGET_FRAMEBUFFER_HPP

#include <Cocktail/Core/Extent3D.hpp>

#include <Cocktail/Renderer/RasterizationSamples.hpp>
#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
    class TextureView;

	/**
     * \brief 
     */
    class Framebuffer : public RenderDeviceObject
    {
    public:

        /**
         * \brief The maximum number of color attachment on a RenderTarget
         */
        static constexpr unsigned int MaxColorAttachmentCount = 8;

        /**
         * \brief
         * \return
         */
        virtual Extent3D<unsigned int> GetSize() const = 0;

        /**
         * \brief
         * \return
         */
        virtual RasterizationSamples GetSamples() const = 0;

        /**
         * \brief
         * \param index
         * \return
         */
        virtual std::shared_ptr<TextureView> GetColorAttachment(unsigned int index) const = 0;

        /**
         * \brief
         * \return
         */
        virtual unsigned int GetColorAttachmentCount() const = 0;

        /**
         * \brief
         * \return
         */
        virtual std::shared_ptr<TextureView> GetDepthStencilAttachment() const = 0;
    };
}

#endif // COCKTAIL_RENDERER_RENDERTARGET_FRAMEBUFFER_HPP
