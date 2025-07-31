#include <Cocktail/Graphic/Scene/Container/SceneContainer.hpp>

namespace Ck
{
	std::shared_ptr<SceneNode> SceneContainer::AddToScene(Scene& scene)
	{
		std::shared_ptr<GraphicEngine> graphicEngine = scene.GetGraphicEngine();

		Array<std::shared_ptr<Mesh>> meshes;
		meshes.Reserve(mMeshes.GetSize());

		Array<std::shared_ptr<Material>> materials;
		materials.Resize(mMaterials.GetSize());

		for (const MeshInfo& meshInfo : mMeshes)
		{
			Array<Mesh::SubMesh> subMeshes;
			subMeshes.Reserve(meshInfo.SubMeshes.GetSize());
			for (const SubMeshInfo& subMeshInfo : meshInfo.SubMeshes)
			{
				subMeshes.Add(Mesh::SubMesh{
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

					material->SetAlphaMode(materialInfo.AlphaMode);
					material->SetAlphaCutoff(materialInfo.AlphaCutoff);

					materials[subMeshInfo.MaterialIndex] = std::move(material);
				}
			}

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshInfo.Vertices, meshInfo.Indices, std::move(subMeshes));
			meshes.Add(std::move(mesh));
		}

		std::shared_ptr<SceneNode> sceneNode = ProcessNode(scene, nullptr, mRoot, meshes, materials);
		graphicEngine->FlushTransfer();

		return sceneNode;
	}

	std::shared_ptr<SceneNode> SceneContainer::ProcessNode(Scene& scene, std::shared_ptr<SceneNode> parent, NodeInfo& nodeInfo, const Array<std::shared_ptr<Mesh>>& meshes, const Array<std::shared_ptr<Material>>& materials)
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
