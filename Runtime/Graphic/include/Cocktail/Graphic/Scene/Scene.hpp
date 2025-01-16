#ifndef COCKTAIL_GRAPHIC_SCENE_SCENE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENE_HPP

#include <Cocktail/Graphic/Camera/Camera.hpp>
#include <Cocktail/Graphic/Mesh/Mesh.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/SceneGraph.hpp>
#include <Cocktail/Graphic/Spatial/TransformationGraph.hpp>

namespace Ck
{
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
		 * \return
		 */
		Signal<Ref<Camera>>& OnCameraAdded();

		/**
		 * \brief
		 * \return
		 */
		Ref<SceneNode> CreateSceneNode();

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Ref<SceneNode>>& OnSceneNodeAdded();

		/**
		 * \brief
		 * \return
		 */
		Ref<TransformationNode> CreateTransformationNode(const Transformation& transformation = Transformation::Identity()) const;

		/**
		 * \brief
		 * \return
		 */
		std::vector<Renderable*> Collect() const;

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
		Signal<Ref<Camera>> mOnCameraAdded;
		Signal<Ref<SceneNode>> mOnSceneNodeAdded;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENE_HPP
