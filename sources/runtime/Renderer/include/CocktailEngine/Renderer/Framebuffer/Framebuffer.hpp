#ifndef COCKTAILENGINE_RENDERER_RENDERTARGET_FRAMEBUFFER_HPP
#define COCKTAILENGINE_RENDERER_RENDERTARGET_FRAMEBUFFER_HPP

#include <CocktailEngine/Core/Extent3D.hpp>
#include <CocktailEngine/Core/Memory/SharedPtr.hpp>

#include <CocktailEngine/Renderer/RasterizationSamples.hpp>
#include <CocktailEngine/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
    class TextureView;

	/**
     * \brief Interface representing an opaque handle to a GPU framebuffer object
     * A Framebuffer is a collection of specific memory attachments that a render pass instance uses.
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
        virtual SharedPtr<TextureView> GetColorAttachment(unsigned int index) const = 0;

        /**
         * \brief
         * \return
         */
        virtual unsigned int GetColorAttachmentCount() const = 0;

        /**
         * \brief
         * \return
         */
        virtual SharedPtr<TextureView> GetDepthStencilAttachment() const = 0;
    };
}

#endif // COCKTAILENGINE_RENDERER_RENDERTARGET_FRAMEBUFFER_HPP
