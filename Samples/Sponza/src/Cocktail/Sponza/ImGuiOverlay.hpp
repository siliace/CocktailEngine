#ifndef IMGUIOVERLAY_HPP
#define IMGUIOVERLAY_HPP

#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/System/Window/Window.hpp>

#include <Cocktail/Graphic/Rendering/SceneViewer.hpp>

/**
 * \brief 
 */
class ImGuiOverlay : public Ck::Observable
{
public:

	/**
	 * \brief 
	 * \param sceneViewer 
	 * \param renderDevice 
	 * \param renderContext 
	 */
	ImGuiOverlay(Ck::Window& window, Ck::SceneViewer& sceneViewer, Ck::Renderer::RenderDevice& renderDevice, Ck::Renderer::RenderContext& renderContext);

	/**
	 * \brief 
	 */
	void Update();

private:

	/**
	 * \brief 
	 * \param mouseCursor 
	 * \return 
	 */
	static Ck::Ref<Ck::Cursor> GetCursor(int mouseCursor);


	/**
	 * \brief 
	 * \param commandList 
	 * \param framebufferSize 
	 */
	void SetupRenderState(Ck::Renderer::CommandList* commandList, Ck::Extent2D<float> framebufferSize) const;

	/**
	 * \brief 
	 * \param renderContext 
	 * \param frameContext 
	 * \param framebuffer 
	 */
	void Render(Ck::Renderer::RenderContext& renderContext, Ck::Renderer::FrameContext& frameContext, Ck::Renderer::Framebuffer& framebuffer);

	Ck::Window* mWindow;
	int mLastMouseCursor = -1;
	Ck::Ref<Ck::Renderer::ShaderProgram> mShaderProgram;
	Ck::Renderer::UniformSlot* mTextureUniformSlot;
	Ck::Ref<Ck::Renderer::TextureView> mFontTextureView;
	Ck::Ref<Ck::Renderer::Sampler> mTextureSampler;
};

#endif // IMGUIOVERLAY_HPP
