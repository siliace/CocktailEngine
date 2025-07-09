#ifndef COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFSCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFSCENECONTAINER_HPP

#include <tiny_gltf.h>

#include <Cocktail/Graphic/Scene/Container/SceneContainer.hpp>

namespace Ck
{
    class GltfSceneContainer : public SceneContainer
    {
    public:

	    /**
	     * \brief Constructor
	     * \param model 
	     */
	    explicit GltfSceneContainer(const tinygltf::Model& model);

    private:

        MaterialInfo ProcessMaterial(const tinygltf::Model& model, const tinygltf::Material& gltfMaterial) const;
        void ProcessMesh(const tinygltf::Model& model, unsigned int meshIndex, const tinygltf::Mesh& gltfMesh);
        NodeInfo ProcessNode(const tinygltf::Model& model, const tinygltf::Node& gltfNode);

        std::unordered_map<unsigned int, Array<unsigned int>> mMeshIndirections; /*!< gltf meshs can issue several MeshInfo, bind them together here */
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFSCENECONTAINER_HPP
