#ifndef COCKTAIL_GRAPHIC_RENDERING_VIEWPORT_HPP
#define COCKTAIL_GRAPHIC_RENDERING_VIEWPORT_HPP

#include <Cocktail/Core/Math/Polygon/Rectangle.hpp>

#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>
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
		 * \param camera 
		 * \param area 
		 */
		Viewport(Camera* camera, const Rectangle<unsigned>& area);

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
		Rectangle<unsigned int> GetArea() const;

		/**
		 * \brief 
		 * \param area 
		 */
		void SetArea(Rectangle<unsigned int> area);

		/**
		 * \brief Get the Camera used to render this Viewport
		 * \return The camera
		 */
		Camera* GetCamera() const;

	private:

		Camera* mCamera;
		Rectangle<unsigned int> mArea;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_VIEWPORT_HPP
