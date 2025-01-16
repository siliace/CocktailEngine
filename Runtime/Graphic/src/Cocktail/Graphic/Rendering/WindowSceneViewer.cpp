#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

#include <Cocktail/Renderer/Context/RenderSurfaceCreateInfo.hpp>

namespace Ck
{
	WindowSceneViewer::WindowSceneViewer(Ref<Scene> scene, Ref<Window> window, SceneViewerParameters parameters, bool vSync) :
		Super(std::move(scene))
	{
		Ref<Renderer::RenderDevice> renderDevice = GetScene()->GetGraphicEngine()->GetRenderDevice();

		Renderer::RenderSurfaceCreateInfo renderSurfaceCreateInfo;
		renderSurfaceCreateInfo.Window = std::move(window);
		renderSurfaceCreateInfo.DepthStencilFormat = parameters.DepthStencilFormat;
		renderSurfaceCreateInfo.Samples = parameters.Samples;
		renderSurfaceCreateInfo.EnableVSync = vSync;
		renderSurfaceCreateInfo.BufferCount = parameters.FrameCount;

		mRenderSurface = renderDevice->CreateRenderSurface(renderSurfaceCreateInfo);
	}

	Renderer::Framebuffer* WindowSceneViewer::AcquireNextFramebuffer(Renderer::FrameContext& frameContext) const
	{
		return frameContext.AcquireNextFramebuffer(mRenderSurface.Get());
	}
}
