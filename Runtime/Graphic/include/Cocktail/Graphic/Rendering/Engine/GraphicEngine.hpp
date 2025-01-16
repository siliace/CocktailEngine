#ifndef COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP

#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>
#include <Cocktail/Graphic/Rendering/SceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/ResourceUploader.hpp>
#include <Cocktail/Graphic/Rendering/Engine/SamplerType.hpp>
#include <Cocktail/Graphic/Rendering/Resource/IndexBuffer.hpp>
#include <Cocktail/Graphic/Rendering/Resource/VertexBuffer.hpp>

#include <Cocktail/Renderer/GraphicApi.hpp>
#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck
{
	class Window;

	/**
     * \brief 
     */
	class COCKTAIL_GRAPHIC_API GraphicEngine : public Extends<GraphicEngine, Object>, public Observable
	{
	public:

		/**
	     * \brief 
	     * \param application
	     * \param api 
	     */
		GraphicEngine(Application& application, Renderer::GraphicApi api);

		/**
		 * \brief Destructor
		 */
		~GraphicEngine() override;

		/**
	     * \brief 
	     * \param vertices 
	     * \param updatable 
	     * \param name 
	     * \return 
	     */
		Ref<VertexBuffer> CreateVertexBuffer(Ref<VertexArray> vertices, bool updatable = false, std::string_view name = "");

		/**
		 * \brief
		 * \param indices
		 * \param updatable
		 * \param name
		 * \return
		 */
		Ref<IndexBuffer> CreateIndexBuffer(Ref<IndexArray> indices, bool updatable = false, std::string_view name = "");

		/**
		 * \brief 
		 * \param mipMaps 
		 * \return 
		 */
		Ref<TextureResource> CreateTextureSampler(Ref<MipMaps> mipMaps, std::string_view name = "");

		/**
	     * \brief 
	     * \param buffer 
	     * \param offset 
	     * \param length 
	     * \param data 
	     */
		void UploadBuffer(Ref<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data);

		/**
		 * \brief 
		 * \param texture 
		 * \param arrayLayer 
		 * \param mipMapLevel 
		 * \param data 
		 */
		void UploadTexture(Ref<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data);

		/**
		 * \brief 
		 */
		void FlushTransfer();

		/**
		 * \brief 
		 */
		void Present();

		/**
	     * \brief 
	     * \return 
	     */
		Ref<Renderer::RenderDevice> GetRenderDevice() const;

		/**
	     * \brief 
	     * \return 
	     */
		Ref<Renderer::RenderContext> GetRenderContext() const;

		Renderer::FrameContext* GetFrameContext() const;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<MaterialProgramManager> GetMaterialProgramManager() const;

		/**
		 * \brief
		 * \param
		 * \return
		 */
		Ref<Renderer::Sampler> GetSampler(SamplerType samplerType) const;

	private:

		Ref<Renderer::RenderDevice> mRenderDevice;
		Ref<Renderer::RenderContext> mRenderContext;
		Renderer::FrameContext* mFrameContext;
		Ref<ResourceUploader> mResourceUploader;
		Ref<MaterialProgramManager> mMaterialProgramManager;
		std::unordered_map<Ref<Window>, Ref<Renderer::RenderSurface>> mWindowRenderSurfaces;
		std::vector<Ref<TextureResource>> mGeneratingMipMaps;
		EnumMap<SamplerType, Ref<Renderer::Sampler>> mSamplers;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP
