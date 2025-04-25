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
	class COCKTAIL_GRAPHIC_API Scene : public Extends<Scene, Object>
	{
	public:

		/**
		 * \brief 
		 * \param transformationGraph 
		 */
		Scene(Ref<GraphicEngine> graphicEngine, Ref<TransformationGraph> transformationGraph = TransformationGraph::New());

		/**
		 * \brief
		 */
		~Scene() override;

		/**
		 * \brief 
		 * \param camera
		 */
		void AddCamera(const Ref<Camera>& camera);

		/**
		 * \brief 
		 * \param light 
		 */
		void AddLight(const Ref<Light>& light);

		/**
		 * \brief
		 * \return
		 */
		Ref<SceneNode> CreateSceneNode();

		/**
		 * \brief
		 * \return
		 */
		Ref<TransformationNode> CreateTransformationNode(const Transformation& transformation = Transformation::Identity());

		/**
		 * \brief
		 * \return
		 */
		Signal<Ref<Camera>>& OnCameraAdded();

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Ref<Light>>& OnLightAdded();

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Ref<SceneNode>>& OnSceneNodeAdded();

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
		Ref<GraphicEngine> GetGraphicEngine() const;

	private:

		Ref<GraphicEngine> mGraphicEngine;
		Ref<TransformationGraph> mTransformationGraph;
		Ref<SceneGraph> mSceneGraph;
		std::vector<Ref<Camera>> mCameras;
		std::vector<Ref<Light>> mLights;
		Signal<Ref<Camera>> mOnCameraAdded;
		Signal<Ref<Light>> mOnLightAdded;
		Signal<Ref<SceneNode>> mOnSceneNodeAdded;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENE_HPP
