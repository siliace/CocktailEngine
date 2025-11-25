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
		void AddCamera(UniquePtr<Camera> camera);

		/**
		 * \brief 
		 * \param camera 
		 */
		void RemoveCamera(const Camera* camera);

		/**
		 * \brief 
		 * \param light 
		 */
		void AddLight(UniquePtr<Light> light);

		/**
		 * \brief 
		 * \param light 
		 */
		void RemoveLight(const Light* light);

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
		Signal<Camera*>& OnCameraAdded();

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Light*>& OnLightAdded();

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
		Array<Renderable*> CollectRenderables(const Camera& camera) const;

		/**
		 * \brief 
		 * \param camera 
		 * \return 
		 */
		Array<Light*> CollectLights(const Camera& camera) const;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<GraphicEngine> GetGraphicEngine() const;

	private:

		std::shared_ptr<GraphicEngine> mGraphicEngine;
		UniquePtr<TransformationGraph> mTransformationGraph;
		UniquePtr<SceneGraph> mSceneGraph;
		Array<UniquePtr<Camera>> mCameras;
		Array<UniquePtr<Light>> mLights;
		Signal<Camera*> mOnCameraAdded;
		Signal<Light*> mOnLightAdded;
		Signal<std::shared_ptr<SceneNode>> mOnSceneNodeAdded;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENE_HPP
