#include <Cocktail/Graphic/Scene/SceneContainer.hpp>

namespace Ck
{
	Ref<SceneNode> SceneContainer::AddToScene(Ref<Scene> scene)
	{
		Ref<GraphicEngine> graphicEngine = scene->GetGraphicEngine();

		std::vector<Ref<Material>> materials;
		materials.reserve(mMaterials.size());
		for (const MaterialInfo& materialInfo : mMaterials)
		{
			Ref<Material> material = Material::New(materialInfo.Name, materialInfo.ShadingMode, materialInfo.DoubleSided);
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

			materials.push_back(std::move(material));
		}

		Ref<SceneNode> sceneNode = ProcessNode(graphicEngine, scene, nullptr, mRoot, materials);
		scene->GetGraphicEngine()->FlushTransfer();

		return sceneNode;
	}

	Ref<SceneNode> SceneContainer::ProcessNode(const Ref<GraphicEngine>& graphicEngine, const Ref<Scene>& scene, Ref<SceneNode> parent, NodeInfo& nodeInfo, const std::vector<Ref<Material>>& materials)
	{
		Ref<SceneNode> sceneNode = scene->CreateSceneNode();
		if (parent)
		{
			parent->InsertChild(sceneNode);
			sceneNode->AttachTo(*parent);
		}
		sceneNode->SetLocalTransformation(nodeInfo.LocalTransformation);

		for (const MeshInfo* meshInfo : nodeInfo.Meshes)
		{
			Ref<Mesh> mesh = Mesh::New(graphicEngine, meshInfo->Vertices, meshInfo->Indices, meshInfo->PrimitiveTopology);
			sceneNode->AddShape(Shape::New(std::move(mesh), materials[meshInfo->MaterialIndex]));
		}

		for (NodeInfo& childNodeInfo : nodeInfo.Children)
			ProcessNode(graphicEngine, scene, sceneNode, childNodeInfo, materials);

		return sceneNode;
	}
}
