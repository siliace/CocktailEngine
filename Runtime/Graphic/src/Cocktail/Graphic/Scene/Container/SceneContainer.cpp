#include <Cocktail/Graphic/Scene/Container/SceneContainer.hpp>

namespace Ck
{
	std::shared_ptr<SceneNode> SceneContainer::AddToScene(Scene& scene)
	{
		std::shared_ptr<GraphicEngine> graphicEngine = scene.GetGraphicEngine();

		std::vector<std::shared_ptr<Mesh>> meshes;
		meshes.reserve(mMeshes.size());

		std::vector<std::shared_ptr<Material>> materials;
		materials.resize(mMaterials.size());

		for (const MeshInfo& meshInfo : mMeshes)
		{
			std::vector<Mesh::SubMesh> subMeshes;
			subMeshes.reserve(meshInfo.SubMeshes.size());
			for (const SubMeshInfo& subMeshInfo : meshInfo.SubMeshes)
			{
				subMeshes.push_back({
					subMeshInfo.Count,
					subMeshInfo.FirstVertex,
					subMeshInfo.FirstIndex,
					subMeshInfo.PrimitiveTopology,
					subMeshInfo.MaterialIndex,
				});

				if (materials[subMeshInfo.MaterialIndex] == nullptr)
				{
					const MaterialInfo& materialInfo = mMaterials[subMeshInfo.MaterialIndex];

					std::shared_ptr<Material> material = std::make_shared<Material>(materialInfo.Name, materialInfo.ShadingMode, materialInfo.DoubleSided);
					material->SetBaseColor(materialInfo.Colors.Base);
					material->SetSpecularColor(materialInfo.Colors.Specular);
					material->SetEmissiveColor(materialInfo.Colors.Emission);

					for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
					{
						std::shared_ptr<MipMaps> mipMaps = materialInfo.Textures[textureType];
						if (!mipMaps)
							continue;

						std::shared_ptr<TextureResource> sampler = graphicEngine->CreateTextureSampler(std::move(mipMaps));
						material->SetTexture(textureType, sampler);
					}

					material->SetOpaque(materialInfo.Opaque);

					materials[subMeshInfo.MaterialIndex] = std::move(material);
				}
			}

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshInfo.Vertices, meshInfo.Indices, std::move(subMeshes));
			meshes.push_back(std::move(mesh));
		}

		std::shared_ptr<SceneNode> sceneNode = ProcessNode(scene, nullptr, mRoot, meshes, materials);
		graphicEngine->FlushTransfer();

		return sceneNode;
	}

	std::shared_ptr<SceneNode> SceneContainer::ProcessNode(Scene& scene, std::shared_ptr<SceneNode> parent, NodeInfo& nodeInfo, const std::vector<std::shared_ptr<Mesh>>& meshes, const std::vector<std::shared_ptr<Material>>& materials)
	{
		std::shared_ptr<SceneNode> sceneNode = scene.CreateSceneNode();
		std::shared_ptr<GraphicEngine> graphicEngine = scene.GetGraphicEngine();

		if (parent)
		{
			parent->InsertChild(sceneNode);
			sceneNode->AttachTo(*parent);
		}
		sceneNode->SetLocalTransformation(nodeInfo.LocalTransformation);

		for (unsigned int meshIndex : nodeInfo.MeshIndices)
			sceneNode->AddShape(std::make_shared<Shape>(*graphicEngine, meshes[meshIndex], materials));

		for (NodeInfo& childNodeInfo : nodeInfo.Children)
			ProcessNode(scene, sceneNode, childNodeInfo, meshes, materials);

		return sceneNode;
	}
}
