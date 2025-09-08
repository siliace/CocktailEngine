#ifndef COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP

#include <Cocktail/Core/Log/LogCategory.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>
#include <Cocktail/Graphic/Rendering/SceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/ResourceUploader.hpp>
#include <Cocktail/Graphic/Rendering/Resource/IndexBuffer.hpp>
#include <Cocktail/Graphic/Rendering/Resource/VertexBuffer.hpp>

#include <Cocktail/Renderer/GraphicApi.hpp>
#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck
{
	class Window;

	COCKTAIL_DECLARE_LOG_CATEGORY(GraphicEngineLogCategory, LogLevel::Info);

	/**
     * \brief 
     */
	class COCKTAIL_GRAPHIC_API GraphicEngine : public Observable, public std::enable_shared_from_this<GraphicEngine>
	{
	public:

		/**
	     * \brief 
	     * \param api 
	     */
		explicit GraphicEngine(Renderer::GraphicApi api);

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
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(std::shared_ptr<VertexArray> vertices, bool updatable = false, const AnsiChar* name = "");

		/**
		 * \brief
		 * \param indices
		 * \param updatable
		 * \param name
		 * \return
		 */
		std::shared_ptr<IndexBuffer> CreateIndexBuffer(std::shared_ptr<IndexArray> indices, bool updatable = false, const AnsiChar* name = "");

		/**
		 * \brief 
		 * \param mipMaps
		 * \param name 
		 * \return 
		 */
		std::shared_ptr<TextureResource> CreateTextureSampler(std::shared_ptr<MipMaps> mipMaps, const AnsiChar* name = "");

		/**
	     * \brief 
	     * \param buffer 
	     * \param offset 
	     * \param length 
	     * \param data 
	     */
		void UploadBuffer(std::shared_ptr<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data);

		/**
		 * \brief 
		 * \param texture 
		 * \param arrayLayer 
		 * \param mipMapLevel 
		 * \param data 
		 */
		void UploadTexture(std::shared_ptr<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data);

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
		Renderer::RenderDevice* GetRenderDevice() const;

		/**
	     * \brief 
	     * \return 
	     */
		std::shared_ptr<Renderer::RenderContext> GetRenderContext() const;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<MaterialProgramManager> GetMaterialProgramManager() const;

	private:

		std::unique_ptr<Renderer::RenderDevice> mRenderDevice;
		std::shared_ptr<Renderer::RenderContext> mRenderContext;
		std::shared_ptr<ResourceUploader> mResourceUploader;
		std::shared_ptr<MaterialProgramManager> mMaterialProgramManager;
		std::unordered_map<std::shared_ptr<Window>, std::shared_ptr<Renderer::RenderSurface>> mWindowRenderSurfaces;
		Array<std::shared_ptr<TextureResource>> mGeneratingMipMaps;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP
