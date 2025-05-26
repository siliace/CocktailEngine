#ifndef COCKTAIL_GRAPHIC_SCENE_SCENE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENE_HPP

#include <Cocktail/Graphic/Mesh/Mesh.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/SceneGraph.hpp>
#include <Cocktail/Graphic/Spatial/TransformationGraph.hpp>

namespace Ck
{
	class Camera;
	class Light;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API Scene
	{
	public:

		/**
		 * \brief 
		 * \param transformationGraph 
		 */
		explicit Scene(std::shared_ptr<GraphicEngine> graphicEngine);

		/**
		 * \brief
		 */
		~Scene();

		/**
		 * \brief 
		 * \param camera
		 */
		void AddCamera(const std::shared_ptr<Camera>& camera);

		/**
		 * \brief 
		 * \param light 
		 */
		void AddLight(const std::shared_ptr<Light>& light);

		/**
		 * \brief
		 * \return
		 */
		std::shared_ptr<SceneNode> CreateSceneNode();

		/**
		 * \brief
		 * \return
		 */
		std::shared_ptr<TransformationNode> CreateTransformationNode(const Transformation& transformation = Transformation::Identity());

		/**
		 * \brief
		 * \return
		 */
		Signal<std::shared_ptr<Camera>>& OnCameraAdded();

		/**
		 * \brief 
		 * \return 
		 */
		Signal<std::shared_ptr<Light>>& OnLightAdded();

		/**
		 * \brief 
		 * \return 
		 */
		Signal<std::shared_ptr<SceneNode>>& OnSceneNodeAdded();

		/**
		 * \brief
		 * \param camera
		 * \return
		 */
		std::vector<Renderable*> CollectRenderables(const Camera& camera) const;

		/**
		 * \brief 
		 * \param camera 
		 * \return 
		 */
		std::vector<Light*> CollectLights(const Camera& camera) const;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<GraphicEngine> GetGraphicEngine() const;

	private:

		std::shared_ptr<GraphicEngine> mGraphicEngine;
		std::unique_ptr<TransformationGraph> mTransformationGraph;
		std::unique_ptr<SceneGraph> mSceneGraph;
		std::vector<std::shared_ptr<Camera>> mCameras;
		std::vector<std::shared_ptr<Light>> mLights;
		Signal<std::shared_ptr<Camera>> mOnCameraAdded;
		Signal<std::shared_ptr<Light>> mOnLightAdded;
		Signal<std::shared_ptr<SceneNode>> mOnSceneNodeAdded;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENE_HPP
