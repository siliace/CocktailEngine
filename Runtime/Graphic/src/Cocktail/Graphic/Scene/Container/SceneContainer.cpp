#include <Cocktail/Graphic/Scene/Container/SceneContainer.hpp>

namespace Ck
{
	Ref<SceneNode> SceneContainer::AddToScene(Scene& scene)
	{
		Ref<GraphicEngine> graphicEngine = scene.GetGraphicEngine();

		std::vector<Ref<Mesh>> meshes;
		meshes.reserve(mMeshes.size());

		std::vector<Ref<Material>> materials;
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

					Ref<Material> material = Material::New(materialInfo.Name, materialInfo.ShadingMode, SamplerType::TrilinearWrap, materialInfo.DoubleSided);
					material->SetBaseColor(materialInfo.Colors.Base);
					material->SetSpecularColor(materialInfo.Colors.Specular);
					material->SetEmissiveColor(materialInfo.Colors.Emission);

					for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
					{
						Ref<MipMaps> mipMaps = materialInfo.Textures[textureType];
						if (!mipMaps)
							continue;

						Ref<TextureResource> sampler = graphicEngine->CreateTextureSampler(std::move(mipMaps));
						material->SetTexture(textureType, sampler);
					}

					material->SetOpaque(materialInfo.Opaque);

					materials[subMeshInfo.MaterialIndex] = std::move(material);
				}
			}

			Ref<Mesh> mesh = Mesh::New(meshInfo.Vertices, meshInfo.Indices, std::move(subMeshes));
			meshes.push_back(std::move(mesh));
		}

		Ref<SceneNode> sceneNode = ProcessNode(scene, nullptr, mRoot, meshes, materials);
		graphicEngine->FlushTransfer();

		return sceneNode;
	}

	Ref<SceneNode> SceneContainer::ProcessNode(Scene& scene, Ref<SceneNode> parent, NodeInfo& nodeInfo, const std::vector<Ref<Mesh>>& meshes, const std::vector<Ref<Material>>& materials)
	{
		Ref<SceneNode> sceneNode = scene.CreateSceneNode();
		Ref<GraphicEngine> graphicEngine = scene.GetGraphicEngine();

		if (parent)
		{
			parent->InsertChild(sceneNode);
			sceneNode->AttachTo(*parent);
		}
		sceneNode->SetLocalTransformation(nodeInfo.LocalTransformation);

		for (unsigned int meshIndex : nodeInfo.MeshIndices)
			sceneNode->AddShape(Shape::New(*graphicEngine, meshes[meshIndex], materials));

		for (NodeInfo& childNodeInfo : nodeInfo.Children)
			ProcessNode(scene, sceneNode, childNodeInfo, meshes, materials);

		return sceneNode;
	}
}
