#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

#include <Cocktail/Renderer/Context/RenderSurfaceCreateInfo.hpp>

namespace Ck
{
	namespace
	{
		PixelFormat GetBestCompatibleDepthStencilFormat(const Renderer::RenderDevice& renderDevice, const PixelFormat& pixelFormat)
		{
			if (pixelFormat.IsStencil())
			{
				unsigned int stencilBits = pixelFormat.GetChannel(0)->Length;
				for (unsigned int depthBits : {16, 24, 32})
				{
					PixelFormat depthStencilFormat = PixelFormat::DepthStencil(depthBits, stencilBits);
					if (renderDevice.GetTextureFormatSupport(depthStencilFormat, Renderer::MemoryType::Static) & Renderer::TextureUsageFlagBits::Attachment)
						return depthStencilFormat;
				}
			}
			else
			{
				unsigned int depthBits = pixelFormat.GetChannel(0)->Length;
				unsigned int stencilBits = pixelFormat.IsDepthStencil() ? pixelFormat.GetChannel(1)->Length : 0;

				if (depthBits == 16)
				{
					// If Depth 16 is not supported, try we better precision
					for (unsigned int betterDepthBits : {24, 32})
					{
						PixelFormat depthStencilFormat = PixelFormat::DepthStencil(betterDepthBits, stencilBits);
						if (renderDevice.GetTextureFormatSupport(depthStencilFormat, Renderer::MemoryType::Static) & Renderer::TextureUsageFlagBits::Attachment)
							return depthStencilFormat;
					}
				}
				else
				{
					// Android has a better support for Depth 24 formats while other platforms tend to prefer Depth 32
					// Here we assume if one those Depth format is not supported, the other will be
					return PixelFormat::DepthStencil(depthBits == 24 ? 32 : 24, stencilBits);
				}
			}
			return PixelFormat::Undefined();
		}
	}

	WindowSceneViewer::WindowSceneViewer(std::shared_ptr<Scene> scene, std::shared_ptr<Window> window, SceneViewerParameters parameters, bool vSync) :
		SceneViewer(std::move(scene))
	{
		std::shared_ptr<Renderer::RenderDevice> renderDevice = GetScene()->GetGraphicEngine()->GetRenderDevice();

		Renderer::RenderSurfaceCreateInfo renderSurfaceCreateInfo;
		renderSurfaceCreateInfo.Window = std::move(window);
		renderSurfaceCreateInfo.DepthStencilFormat = parameters.DepthStencilFormat;
		renderSurfaceCreateInfo.Samples = parameters.Samples;
		renderSurfaceCreateInfo.EnableVSync = vSync;
		renderSurfaceCreateInfo.BufferCount = parameters.FrameCount;

		PixelFormat& depthStencilFormat = renderSurfaceCreateInfo.DepthStencilFormat;
		if (!renderDevice->GetTextureFormatSupport(depthStencilFormat, Renderer::MemoryType::Static))
		{
			depthStencilFormat = GetBestCompatibleDepthStencilFormat(*renderDevice, depthStencilFormat);
			assert(renderDevice->GetTextureFormatSupport(depthStencilFormat, Renderer::MemoryType::Static));
		}

		mRenderSurface = renderDevice->CreateRenderSurface(renderSurfaceCreateInfo);
	}

	Renderer::Framebuffer* WindowSceneViewer::AcquireNextFramebuffer(Renderer::RenderContext& renderContext) const
	{
		return renderContext.AcquireFramebuffer(mRenderSurface.get());
	}
}
