#include <CocktailEngine/Graphic/Scene/Camera/OrhtographicCamera.hpp>
#include <CocktailEngine/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <CocktailEngine/Graphic/Scene/Container/SceneContainer.hpp>
#include <CocktailEngine/Graphic/Scene/Light/DirectionalLight.hpp>
#include <CocktailEngine/Graphic/Scene/Light/PointLight.hpp>
#include <CocktailEngine/Graphic/Scene/Light/SpotLight.hpp>
#include <CocktailEngine/Graphic/Scene/Shape/StaticMeshShape.hpp>

namespace Ck
{
	const Array<SceneContainer::CameraInfo>& SceneContainer::GetCameras() const
	{
		return mCameras;
	}

    const Array<SceneContainer::LightInfo>& SceneContainer::GetLights() const
    {
        return mLights;
    }

    SceneNode* SceneContainer::AddToScene(Scene& scene)
	{
		SharedPtr<GraphicEngine> graphicEngine = scene.GetGraphicEngine();

		Array<SharedPtr<Mesh>> meshes;
		meshes.Reserve(mMeshes.GetSize());

		Array<SharedPtr<Material>> materials;
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

					SharedPtr<Material> material = MakeShared<Material>(materialInfo.Name, materialInfo.ShadingMode, materialInfo.DoubleSided);
					material->SetBaseColor(materialInfo.Parameters.Base);
					material->SetEmissiveColor(materialInfo.Parameters.Emission);
                    material->SetRoughness(materialInfo.Parameters.Roughness);
                    material->SetMetallic(materialInfo.Parameters.Metallic);

					for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
					{
						SharedPtr<MipMaps> mipMaps = materialInfo.Textures[textureType];
						if (!mipMaps)
							continue;

						SharedPtr<TextureResource> sampler = graphicEngine->CreateTextureSampler(Move(mipMaps));
						material->SetTexture(textureType, sampler);
					}

					material->SetAlphaMode(materialInfo.AlphaMode);
					material->SetAlphaCutoff(materialInfo.AlphaCutoff);

					materials[subMeshInfo.MaterialIndex] = Move(material);
				}
			}

			SharedPtr<Mesh> mesh = MakeShared<Mesh>(meshInfo.Vertices, meshInfo.Indices, Move(subMeshes));
			meshes.Add(Move(mesh));
		}

		SceneNode* sceneNode = ProcessNode(scene, nullptr, mRoot, meshes, materials);
		graphicEngine->FlushTransfer();

		return sceneNode;
	}

	SceneContainer::SceneContainer()
	{
		mMaterials.Add(CreateDefaultMaterial(LinearColor::Black));
	}

	SceneContainer::MaterialInfo SceneContainer::CreateDefaultMaterial(LinearColor baseColor)
	{
		MaterialInfo material;
		material.Name = "Default";
		material.ShadingMode = Material::ShadingMode::Lit;
		material.Parameters.Base = baseColor;
		material.DoubleSided = true;
		material.AlphaMode = Material::AlphaMode::Opaque;

		return material;
	}

	SceneNode* SceneContainer::ProcessNode(Scene& scene, SceneNode* parent, NodeInfo& nodeInfo, const Array<SharedPtr<Mesh>>& meshes, const Array<SharedPtr<Material>>& materials)
	{
		SceneNode* sceneNode = scene.CreateSceneNode();
		SharedPtr<GraphicEngine> graphicEngine = scene.GetGraphicEngine();

		if (parent)
		{
			parent->InsertChild(sceneNode);
			sceneNode->AttachTo(*parent);
		}
		sceneNode->SetLocalTransformation(nodeInfo.LocalTransformation);

		for (unsigned int meshIndex : nodeInfo.MeshIndices)
			sceneNode->AddShape(MakeShared<StaticMeshShape>(*graphicEngine, meshes[meshIndex], materials));

		for (NodeInfo& childNodeInfo : nodeInfo.Children)
			ProcessNode(scene, sceneNode, childNodeInfo, meshes, materials);

		if (CameraInfo* cameraInfo = nodeInfo.Camera)
		{
			UniquePtr<Camera> camera;
			if (cameraInfo->IsPerspective)
			{
				camera = MakeUnique<PerspectiveCamera>(
					sceneNode->GetTransformationNode(),
				    cameraInfo->Name,
					cameraInfo->Perspective.FieldOfView, cameraInfo->Perspective.AspectRatio, 
					cameraInfo->DepthBounds
				);
			}
			else
			{
			    Vector2<float> position;
			    position.X() = -cameraInfo->Orthographic.HorizontalMagnification;
			    position.Y() = -cameraInfo->Orthographic.VerticalMagnification;

			    Extent2D<float> extent;
			    extent.Width = 2.f * cameraInfo->Orthographic.HorizontalMagnification;
			    extent.Height = 2.f * cameraInfo->Orthographic.VerticalMagnification;

				camera = MakeUnique<OrthographicCamera>(
					sceneNode->GetTransformationNode(),
				    cameraInfo->Name,
					Rectangle(position, extent),
					cameraInfo->DepthBounds
				);
			}

			scene.AddCamera(Move(camera));
		}

	    if (LightInfo* lightInfo = nodeInfo.Light)
	    {
	        UniquePtr<Light> light;
	        switch (lightInfo->Type)
	        {
                case Light::Type::Directional:
	                light = MakeUnique<DirectionalLight>(lightInfo->Name, -sceneNode->GetFront(), lightInfo->Color, lightInfo->Intensity);
	                break;

                case Light::Type::Point:
	                light = MakeUnique<PointLight>(sceneNode->GetTransformationNode(), lightInfo->Name, lightInfo->Range, lightInfo->Color, lightInfo->Intensity);
	                break;

                case Light::Type::Spot:
	                light = MakeUnique<SpotLight>(sceneNode->GetTransformationNode(), lightInfo->Name, lightInfo->Range, lightInfo->Color, lightInfo->Intensity);
	                break;
            }

            scene.AddLight(Move(light));
	    }

		return sceneNode;
	}
}
