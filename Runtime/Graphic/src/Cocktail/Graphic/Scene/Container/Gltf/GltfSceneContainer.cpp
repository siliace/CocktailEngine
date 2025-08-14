#include <Cocktail/Graphic/Scene/Container/Gltf/GltfSceneContainer.hpp>
#include <Cocktail/Graphic/Scene/Container/Gltf/GltfUtils.hpp>

namespace Ck
{
    GltfSceneContainer::GltfSceneContainer(const tinygltf::Model& model)
    {
        for (const tinygltf::Image& gltfImage : model.images)
        {
	        Extent2D<unsigned int> size = MakeExtent<unsigned int>(gltfImage.width, gltfImage.height);

            PixelFormat::Layout pixelFormatLayout = {};
	        switch (gltfImage.component)
	        {
	        case 1:
                pixelFormatLayout = PixelFormat::Layout::R;
                break;

	        case 2:
                pixelFormatLayout = PixelFormat::Layout::RG;
                break;

	        case 3:
                pixelFormatLayout = PixelFormat::Layout::RGB;
                break;

	        case 4:
                pixelFormatLayout = PixelFormat::Layout::RGBA;
                break;
	        }

            DataType dataType = GltfUtils::ConvertComponentType(gltfImage.pixel_type);
            std::shared_ptr<Image> image = std::make_shared<Image>(size, PixelFormat::Color(pixelFormatLayout, dataType), gltfImage.image.data());

            mMipMaps.Add(MipMaps::FromImage(*image));
        }

        for (const tinygltf::Camera& gltfCamera : model.cameras)
        {
            CameraInfo& cameraInfo = mCameras.Emplace();

            cameraInfo.Name = gltfCamera.name;
        	cameraInfo.IsPerspective = gltfCamera.type == "perspective";
            if (cameraInfo.IsPerspective)
            {
                cameraInfo.Perspective.FieldOfView = Angle<float>::Degree(gltfCamera.perspective.yfov);
                cameraInfo.Perspective.AspectRatio = gltfCamera.perspective.aspectRatio;
                cameraInfo.DepthBounds.X() = gltfCamera.perspective.znear;
                cameraInfo.DepthBounds.Y() = gltfCamera.perspective.zfar;
            }
            else
            {
                cameraInfo.DepthBounds.X() = gltfCamera.orthographic.znear;
                cameraInfo.DepthBounds.Y() = gltfCamera.orthographic.zfar;
            }
        }

        for (const tinygltf::Material& gltfMaterial : model.materials)
        {
            mMaterials.Add(
                ProcessMaterial(model, gltfMaterial)
            );
        }

        for (unsigned int i = 0; i < model.meshes.size(); i++)
            ProcessMesh(model, i, model.meshes[i]);

        const tinygltf::Scene& defautlScene = model.scenes[model.defaultScene];

    	Array<NodeInfo> childrenNodeInfo;
        childrenNodeInfo.Reserve(defautlScene.nodes.size());
        for (int sceneNodeIndex : defautlScene.nodes)
        {
            childrenNodeInfo.Add(
                ProcessNode(model, model.nodes[sceneNodeIndex])
            );
        }

        mRoot.Children = std::move(childrenNodeInfo);
        mRoot.LocalTransformation = Transformation::Identity();
    }
    
    SceneContainer::MaterialInfo GltfSceneContainer::ProcessMaterial(const tinygltf::Model& model, const tinygltf::Material& gltfMaterial) const
    {
        MaterialInfo materialInfo;
        materialInfo.Name = gltfMaterial.name;
        materialInfo.Colors.Base = GltfUtils::ConvertLinearColor(gltfMaterial.pbrMetallicRoughness.baseColorFactor);
        materialInfo.Colors.Emission = GltfUtils::ConvertLinearColor(gltfMaterial.emissiveFactor);
        materialInfo.ShadingMode = Material::ShadingMode::Phong;
        materialInfo.DoubleSided = gltfMaterial.doubleSided;
        materialInfo.AlphaMode = GltfUtils::ConvertAlphaMode(gltfMaterial.alphaMode);
        materialInfo.AlphaCutoff = static_cast<float>(gltfMaterial.alphaCutoff);

        if (int textureIndex = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index; textureIndex != -1)
        {
            const tinygltf::Texture& texture = model.textures[textureIndex];
            materialInfo.Textures[Material::TextureType::BaseColor] = mMipMaps[texture.source];
        }

        if (int textureIndex = gltfMaterial.emissiveTexture.index; textureIndex != -1)
        {
            const tinygltf::Texture& texture = model.textures[textureIndex];
            materialInfo.Textures[Material::TextureType::Emission] = mMipMaps[texture.source];
        }

        return materialInfo;
    }

    void GltfSceneContainer::ProcessMesh(const tinygltf::Model& model, unsigned int meshIndex, const tinygltf::Mesh& gltfMesh)
    {
        Array<MeshInfo> primitiveMeshes;
        for (const tinygltf::Primitive& primitive : gltfMesh.primitives)
        {
            VertexLayout::Builder vertexLayoutBuilder;
            std::unordered_map<VertexAttributeSemantic, const tinygltf::Accessor*> attributeAccessors;

            for (const auto& [name, index] : primitive.attributes)
            {
                const tinygltf::Accessor& accessor = model.accessors[index];

                unsigned int size = 1;
                if (accessor.type != TINYGLTF_TYPE_SCALAR)
                    size = accessor.type;

                DataType dataType = GltfUtils::ConvertComponentType(accessor.componentType);
                VertexAttributeSemantic attributeSemantic = GltfUtils::ConvertAttributeName(name);
                vertexLayoutBuilder.AddAttribute(attributeSemantic, dataType, size, 1, accessor.normalized);

                attributeAccessors[attributeSemantic] = &accessor;
            }

            std::shared_ptr<VertexLayout> vertexLayout = vertexLayoutBuilder.Get();

            std::size_t vertexCount = 0;
            for (const auto& [attributeSemantic, accessor] : attributeAccessors)
                vertexCount = std::max(vertexCount, accessor->count);

            std::shared_ptr<VertexArray> vertices = std::make_shared<VertexArray>(vertexLayout, vertexCount);
            for (const auto& [attributeSemantic, accessor] : attributeAccessors)
            {
                assert(accessor->count == vertexCount);

                const tinygltf::BufferView& bufferView = model.bufferViews[accessor->bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                int size = 1;
                if (accessor->type != TINYGLTF_TYPE_SCALAR)
                    size = accessor->type;

                unsigned int stride = accessor->ByteStride(bufferView);
                const std::size_t baseOffset = accessor->byteOffset + bufferView.byteOffset;
                for (std::size_t i = 0; i < vertices->GetVertexCount(); i++)
                {
                    VertexRef vertex = vertices->At(i);
                    const std::size_t offset = baseOffset + i * stride;
                    vertex.Set(attributeSemantic, &buffer.data[offset], GltfUtils::ConvertComponentType(accessor->componentType), size);
                }
            }

            std::shared_ptr<IndexArray> indices;
            if (primitive.indices != -1)
            {
                const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];

                indices = std::make_shared<IndexArray>(GltfUtils::ConvertIndexType(indexAccessor.componentType), indexAccessor.count);

                const tinygltf::BufferView& bufferView = model.bufferViews[indexAccessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                unsigned int stride = indexAccessor.ByteStride(bufferView);

                const std::size_t baseOffset = indexAccessor.byteOffset + bufferView.byteOffset;

	            for (std::size_t i = 0; i < indices->GetIndexCount(); i++)
                {
                    IndexRef index = indices->At(i);
                    const std::size_t offset = baseOffset + i * stride;
                	switch (indices->GetIndexType())
                    {
                        case Renderer::IndexType::Byte:
                            {
                                const Uint8* value = &buffer.data[offset];
                                index.Set<Uint8>(*value);
                            }
                            break;

                        case Renderer::IndexType::Short:
                            {
                                const Uint16* value = reinterpret_cast<const Uint16*>(&buffer.data[offset]);
                                index.Set<Uint16>(*value);
                            }
                            break;

                        case Renderer::IndexType::Integer:
                            {
                                const Uint32* value = reinterpret_cast<const Uint32*>(&buffer.data[offset]);
                                index.Set<Uint32>(*value);
                            }
                            break;
                    }
                }
            }

            SubMeshInfo subMesh;
            subMesh.Count = static_cast<unsigned int>(indices ? indices->GetIndexCount() : vertices->GetVertexCount());
            subMesh.PrimitiveTopology = GltfUtils::ConvertPrimitiveTopology(primitive.mode);
            subMesh.MaterialIndex = primitive.material == -1 ? 0 : primitive.material + 1; // material 0 is the default one (index -1 in gltf)

            MeshInfo* compatibleMeshInfo = nullptr;
            for (MeshInfo& meshInfo : primitiveMeshes)
            {
	            if (meshInfo.Vertices->GetVertexLayout()->IsCompatibleWith(*vertexLayout))
	            {
                    compatibleMeshInfo = &meshInfo;
                    break;
	            }
            }

        	if (compatibleMeshInfo)
            {
                subMesh.FirstIndex = compatibleMeshInfo->Indices->GetIndexCount();
                subMesh.FirstVertex = compatibleMeshInfo->Vertices->GetVertexCount();

                compatibleMeshInfo->Indices->Merge(*indices);
                compatibleMeshInfo->Vertices->Merge(*vertices);
        		compatibleMeshInfo->SubMeshes.Add(subMesh);
            }
            else
            {
                subMesh.FirstIndex = 0;
                subMesh.FirstVertex = 0;

                MeshInfo meshInfo;
                meshInfo.Name = gltfMesh.name;
                meshInfo.Vertices = std::move(vertices);
                meshInfo.Indices = std::move(indices);
                meshInfo.SubMeshes.Add(subMesh);

                primitiveMeshes.Add(meshInfo);
            }
        }

        for (auto it = primitiveMeshes.begin(); it != primitiveMeshes.end(); ++it)
        {
            mMeshes.Add(std::move(*it));
            mMeshIndirections[meshIndex].Add(mMeshes.GetSize() - 1);
        }
    }

    SceneContainer::NodeInfo GltfSceneContainer::ProcessNode(const tinygltf::Model& model, const tinygltf::Node& gltfNode)
    {
        Array<NodeInfo> childrenNodes;
        childrenNodes.Reserve(gltfNode.children.size());
        for (int childSceneNodeIndex : gltfNode.children)
        {
            childrenNodes.Add(
                ProcessNode(model, model.nodes[childSceneNodeIndex])
            );
        }

        Transformation localTransformation;
        if (gltfNode.matrix.empty())
        {
            localTransformation = Transformation::Identity();

            if (!gltfNode.translation.empty())
                localTransformation.SetTranslation(GltfUtils::ConvertVector3(gltfNode.translation));

            if (!gltfNode.rotation.empty())
                localTransformation.SetRotation(GltfUtils::ConvertQuaternion(gltfNode.rotation));

            if (!gltfNode.scale.empty())
                localTransformation.SetScale(GltfUtils::ConvertVector3(gltfNode.scale));
        }
        else
        {
            Matrix4<float> matrix;
            for (unsigned int i = 0; i < 4; i++)
            {
                for (unsigned int j = 0; j < 4; j++)
                    matrix.At(j, i) = static_cast<float>(gltfNode.matrix.at(i * 4 + j));
            }

            localTransformation = Transformation::Decompose(matrix);
        }

        SceneContainer::NodeInfo nodeInfo;
        nodeInfo.Children = std::move(childrenNodes);
        nodeInfo.MeshIndices = mMeshIndirections[gltfNode.mesh];
        nodeInfo.LocalTransformation = localTransformation;
        if (int cameraIndex = gltfNode.camera; cameraIndex != -1)
            nodeInfo.Camera = &mCameras[cameraIndex];

        return nodeInfo;
    }
}
