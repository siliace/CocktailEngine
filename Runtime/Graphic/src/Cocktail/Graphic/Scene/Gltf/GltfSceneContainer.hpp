#ifndef COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFSCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFSCENECONTAINER_HPP

#include <tiny_gltf.h>

#include <Cocktail/Graphic/Scene/SceneContainer.hpp>

namespace Ck
{
    class GltfSceneContainer : public Extends<GltfSceneContainer, SceneContainer>
    {
    public:

        GltfSceneContainer(const tinygltf::Model& model);

    private:

        SceneContainer::MaterialInfo ProcessMaterial(const tinygltf::Model& model, const tinygltf::Material& gltfMaterial);
        void ProcessMesh(const tinygltf::Model& model, const tinygltf::Mesh& gltfMesh);
        SceneContainer::NodeInfo ProcessNode(const tinygltf::Model& model, const tinygltf::Node& gltfNode);

        std::vector<MaterialInfo> mMaterial;
        std::vector<MeshInfo> mMeshes;
        std::unordered_multimap<const tinygltf::Mesh*, std::size_t> mPrimitiveMeshes;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_GLTF_GLTFSCENECONTAINER_HPP
