#ifndef COCKTAIL_GRAPHIC_RENDERING_VIEWPORT_HPP
#define COCKTAIL_GRAPHIC_RENDERING_VIEWPORT_HPP

#include <Cocktail/Core/Math/Polygon/Rectangle.hpp>

#include <Cocktail/Graphic/Scene/SceneView.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API Viewport
	{
	public:
		/**
		 * \brief 
		 * \param sceneView
		 * \param area 
		 */
		Viewport(UniquePtr<SceneView> sceneView, const Rectangle<float>& area);

		/**
		 * \brief 
		 * \param commandList 
		 * \param framebuffer 
		 * \param drawContext 
		 */
		void Bind(Renderer::CommandList& commandList, Renderer::Framebuffer& framebuffer, RecordDrawContext& drawContext, bool clear);

        /**
         * \brief
         * \return
         */
	    SceneView* GetSceneView() const;

        /**
         * \brief
         *
         * \return
         */
	    const Rectangle<float>& GetArea() const;

        /**
         * \brief
         *
         * \param area
         */
	    void SetArea(const Rectangle<float>& area);

    private:

        /**
         * \brief
         * \param clientSize
         * \return
         */
	    Rectangle<float> ComputeClientArea(const Extent2D<unsigned int>& clientSize);

	    UniquePtr<SceneView> mSceneView;
		Rectangle<float> mArea;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_VIEWPORT_HPP
