#include <Cocktail/Graphic/Rendering/Viewport.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>

namespace Ck
{
	Viewport::Viewport(Ref<Camera> camera, const Rectangle<unsigned int>& area):
		mCamera(std::move(camera)),
		mArea(area)
	{
		/// Nothing
	}

	void Viewport::Bind(Renderer::CommandList& commandList, Renderer::Framebuffer& framebuffer, RecordDrawContext& drawContext, bool clear)
	{
		struct CameraInfo
		{
			Matrix4<float> ViewProjection;
		};

		Renderer::RenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.Mode = clear ? Renderer::RenderPassMode::Clear : Renderer::RenderPassMode::Load;
		renderPassBeginInfo.Framebuffer = &framebuffer;
		commandList.BeginRenderPass(renderPassBeginInfo);

		Renderer::Viewport viewport;
		viewport.X = mArea.Position.X();
		viewport.Y = mArea.Position.Y();
		viewport.Width = mArea.Extent.X();
		viewport.Height = mArea.Extent.Y();
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;
		commandList.SetViewport(viewport);

		Renderer::Scissor scissor;
		scissor.Position.Width = mArea.Position.X();
		scissor.Position.Height = mArea.Position.Y();
		scissor.Size.Width = mArea.Extent.X();
		scissor.Size.Height = mArea.Extent.Y();
		commandList.SetScissor(scissor);

		CameraInfo cameraInfo;
		cameraInfo.ViewProjection = mCamera->ComputeProjectionViewMatrix().Transpose();
		drawContext.BindPersistentUniformData(commandList, "cameraInfo", 0, sizeof(CameraInfo), &cameraInfo);
	}

	Rectangle<unsigned int> Viewport::GetArea() const
	{
		return mArea;
	}

	void Viewport::SetArea(Rectangle<unsigned int> area)
	{
		mArea = area;
	}
	
	Ref<Camera> Viewport::GetCamera() const
	{
    	return mCamera;
	}
}