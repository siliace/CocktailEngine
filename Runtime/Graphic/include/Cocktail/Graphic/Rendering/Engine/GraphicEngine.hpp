#ifndef COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP

#include <Cocktail/Core/Log/LogCategory.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramManager.hpp>
#include <Cocktail/Graphic/Rendering/Engine/ResourceUploader.hpp>
#include <Cocktail/Graphic/Rendering/Resource/IndexBuffer.hpp>
#include <Cocktail/Graphic/Rendering/Resource/StorageBuffer.hpp>
#include <Cocktail/Graphic/Rendering/Resource/VertexBuffer.hpp>
#include <Cocktail/Graphic/Rendering/SceneViewer.hpp>

#include <Cocktail/Renderer/GraphicApi.hpp>
#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck
{
	class Window;

	COCKTAIL_DECLARE_LOG_CATEGORY(GraphicEngineLogCategory, LogLevel::Info);

	/**
     * \brief 
     */
	class COCKTAIL_GRAPHIC_API GraphicEngine : public Observable, public SharedFromThis<GraphicEngine>
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
		SharedPtr<VertexBuffer> CreateVertexBuffer(SharedPtr<VertexArray> vertices, bool updatable = false, const AnsiChar* name = "");

		/**
		 * \brief
		 * \param indices
		 * \param updatable
		 * \param name
		 * \return
		 */
		SharedPtr<IndexBuffer> CreateIndexBuffer(SharedPtr<IndexArray> indices, bool updatable = false, const AnsiChar* name = "");

        /**
         * \brief
         * \param size
         * \param updatable
         * \param name
         * \return
         */
	    SharedPtr<StorageBuffer> CreateStorageBuffer(std::size_t size, bool updatable = false, const AnsiChar* name = "");

	    /**
	     * \brief
	     *
	     * \tparam T
	     *
	     * \param elementCount
	     * \param updatable
	     * \param name
	     *
	     * \return
	     */
	    template <typename T>
	    SharedPtr<TypedStorageBuffer<T>> CreateTypedStorageBuffer(unsigned int elementCount, bool updatable = false, const AnsiChar* name = "")
	    {
	        return MakeShared<TypedStorageBuffer<T>>(AsShared(), elementCount, name);
	    }

		/**
		 * \brief 
		 * \param mipMaps
		 * \param name 
		 * \return 
		 */
		SharedPtr<TextureResource> CreateTextureSampler(SharedPtr<MipMaps> mipMaps, const AnsiChar* name = "");

		/**
	     * \brief 
	     * \param buffer 
	     * \param offset 
	     * \param length 
	     * \param data 
	     */
		void UploadBuffer(SharedPtr<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data);

		/**
		 * \brief 
		 * \param texture 
		 * \param arrayLayer 
		 * \param mipMapLevel 
		 * \param data 
		 */
		void UploadTexture(SharedPtr<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data);

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
		SharedPtr<Renderer::RenderContext> GetRenderContext() const;

		/**
		 * \brief 
		 * \return 
		 */
		SharedPtr<MaterialProgramManager> GetMaterialProgramManager() const;

	private:

		UniquePtr<Renderer::RenderDevice> mRenderDevice;
		SharedPtr<Renderer::RenderContext> mRenderContext;
		SharedPtr<ResourceUploader> mResourceUploader;
		SharedPtr<MaterialProgramManager> mMaterialProgramManager;
		HashMap<SharedPtr<Window>, SharedPtr<Renderer::RenderSurface>> mWindowRenderSurfaces;
		Array<SharedPtr<TextureResource>> mGeneratingMipMaps;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_ENGINE_GRAPHICENGINE_HPP
