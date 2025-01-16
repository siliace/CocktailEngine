#include <Cocktail/Graphic/Scene/Gltf/GltfSceneContainer.hpp>

namespace Ck
{
    namespace
    {
	    Vector3<float> ToVec3(const std::vector<double>& values)
        {
            Vector3<float> vector;
            assert(values.size() == 3);
            for (std::size_t i = 0; i < values.size(); i++)
                vector.At(i) = static_cast<float>(values[i]);

            return vector;
        }

	    Quaternion<float> ToQuat(const std::vector<double>& values)
        {
            Quaternion<float> quaternion;
            assert(values.size() == 4);
            quaternion.X = static_cast<float>(values[0]);
            quaternion.Y = static_cast<float>(values[1]);
            quaternion.Z = static_cast<float>(values[2]);
            quaternion.W = static_cast<float>(values[3]);

            return quaternion;
        }

	    RgbColor<float> ToRgbColor(const std::vector<double>& values)
        {
            RgbColor<float> color;
            assert(values.size() == 3);
            color.Red = static_cast<float>(values[0]) * 255.f;
            color.Green = static_cast<float>(values[1]) * 255.f;
            color.Blue = static_cast<float>(values[2]) * 255.f;

            return color;
        }

        RgbaColor<float> ToRgbaColor(const std::vector<double>& values)
        {
            RgbaColor<float> color;
            assert(values.size() == 4);
            color.Red = static_cast<float>(values[0]) * 255.f;
            color.Green = static_cast<float>(values[1]) * 255.f;
            color.Blue = static_cast<float>(values[2]) * 255.f;
            color.Alpha = static_cast<float>(values[3]) * 255.f;

            return color;
        }
    }

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

            DataType dataType;
            switch (gltfImage.pixel_type)
            {
            case TINYGLTF_COMPONENT_TYPE_BYTE:
                dataType = DataType::Int8;
                break;

            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                dataType = DataType::UnsignedInt8;
                break;

            case TINYGLTF_COMPONENT_TYPE_SHORT:
                dataType = DataType::Int16;
                break;

            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                dataType = DataType::UnsignedInt16;
                break;

            case TINYGLTF_COMPONENT_TYPE_INT:
                dataType = DataType::Int32;
                break;

            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                dataType = DataType::UnsignedInt32;
                break;

            case TINYGLTF_COMPONENT_TYPE_FLOAT:
                dataType = DataType::Float32;
                break;

            case TINYGLTF_COMPONENT_TYPE_DOUBLE:
                dataType = DataType::Float64;
                break;
            }

            Ref<Image> image = Image::New(size, PixelFormat::Color(pixelFormatLayout, dataType), gltfImage.image.data());

            mMipMaps.push_back(MipMaps::FromImage(*image, true));
        }

        for (const tinygltf::Material& gltfMaterial : model.materials)
        {
            mMaterials.push_back(
                ProcessMaterial(model, gltfMaterial)
            );
        }

        for (const tinygltf::Mesh& gltfMesh : model.meshes)
        {
            ProcessMesh(model, gltfMesh);
        }

        const tinygltf::Scene& defautlScene = model.scenes[model.defaultScene];
        std::vector<NodeInfo> childrenNodeInfo;
        childrenNodeInfo.reserve(defautlScene.nodes.size());
        for (int sceneNodeIndex : defautlScene.nodes)
        {
            childrenNodeInfo.push_back(
                ProcessNode(model, model.nodes[sceneNodeIndex])
            );
        }

        mRoot.Children = std::move(childrenNodeInfo);
        mRoot.LocalTransformation = Transformation::Identity();
    }
    
    SceneContainer::MaterialInfo GltfSceneContainer::ProcessMaterial(const tinygltf::Model& model, const tinygltf::Material& gltfMaterial)
    {
        MaterialInfo materialInfo;
        materialInfo.Name = gltfMaterial.name;
        materialInfo.Colors.Base = ToRgbaColor(gltfMaterial.pbrMetallicRoughness.baseColorFactor);
        materialInfo.Colors.Emission = ToRgbColor(gltfMaterial.emissiveFactor);
        materialInfo.ShadingMode = Material::ShadingMode::Phong;
        materialInfo.DoubleSided = gltfMaterial.doubleSided;
        materialInfo.Opaque = gltfMaterial.alphaMode == "OPAQUE";

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

    void GltfSceneContainer::ProcessMesh(const tinygltf::Model& model, const tinygltf::Mesh& gltfMesh)
    {
        for (const tinygltf::Primitive& primitive : gltfMesh.primitives)
        {
			VertexLayout::Builder vertexLayoutBuilder;
            std::unordered_map<VertexAttributeSemantic, const tinygltf::Accessor*> attributeAccessors;

        	for (const auto&[name, index] : primitive.attributes)
            {
                const tinygltf::Accessor& accessor = model.accessors[index];
                
                VertexAttributeSemantic attributeSemantic = {};
                if (name == "POSITION")
                {
                    attributeSemantic = VertexAttributeSemantic::Position;
                }
                else if (name == "NORMAL")
                {
                    attributeSemantic = VertexAttributeSemantic::Normal;
                }
                else if (name == "TEXCOORD_0")
                {
                    attributeSemantic = VertexAttributeSemantic::TexCoord;
                }
                else if (name == "TANGENT")
                {
                    attributeSemantic = VertexAttributeSemantic::Tangent;
                }

                DataType dataType = {};
                switch (accessor.componentType)
                {
                case TINYGLTF_COMPONENT_TYPE_BYTE:
                    dataType = DataType::Int8;
                    break;

                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    dataType = DataType::UnsignedInt8;
                    break;

                case TINYGLTF_COMPONENT_TYPE_SHORT:
                    dataType = DataType::Int16;
                    break;

                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    dataType = DataType::UnsignedInt16;
                    break;

                case TINYGLTF_COMPONENT_TYPE_INT:
                    dataType = DataType::Int32;
                    break;

                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                    dataType = DataType::UnsignedInt32;
                    break;

                case TINYGLTF_COMPONENT_TYPE_FLOAT:
                    dataType = DataType::Float32;
                    break;

                case TINYGLTF_COMPONENT_TYPE_DOUBLE:
                    dataType = DataType::Float64;
                    break;
                }

                int size = 1;
                if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                    size = accessor.type;
                }

                vertexLayoutBuilder.AddAttribute(attributeSemantic, dataType, size, 1, accessor.normalized);
                attributeAccessors[attributeSemantic] = &accessor;
            }

            MeshInfo meshInfo;
            Ref<VertexLayout> vertexLayout = vertexLayoutBuilder.Get();

            std::size_t vertexCount = 0;
            for (const auto& [attributeSemantic, accessor] : attributeAccessors)
                vertexCount = std::max(vertexCount, accessor->count);

            Ref<VertexArray> vertices = VertexArray::New(vertexLayout, vertexCount);
            for (const auto&[attributeSemantic, accessor] : attributeAccessors)
            {
	            const tinygltf::BufferView& bufferView = model.bufferViews[accessor->bufferView];
	            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                int size = 1;
                if (accessor->type != TINYGLTF_TYPE_SCALAR) 
                    size = accessor->type;

                unsigned int stride = accessor->ByteStride(bufferView);
                const std::size_t baseOffset = accessor->byteOffset + bufferView.byteOffset;
                for (std::size_t i = 0; i < vertices->GetVertexCount() && i < accessor->count; i++)
                {
                    VertexRef vertex = vertices->At(i);
                    const std::size_t offset = baseOffset + i * stride;
                    switch (accessor->componentType)
                    {
                    case TINYGLTF_COMPONENT_TYPE_BYTE:
	                    {
	                        const char* value = reinterpret_cast<const char*>(&buffer.data[offset]);
	                        vertex.Set(attributeSemantic, value, size);
	                    }
                        break;

                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
	                    {
	                        const unsigned char* value = &buffer.data[offset];
	                        vertex.Set(attributeSemantic, value, size);
	                    }
	                    break;

                    case TINYGLTF_COMPONENT_TYPE_SHORT:
	                    {
	                        const short* value = reinterpret_cast<const short*>(&buffer.data[offset]);
	                        vertex.Set(attributeSemantic, value, size);
	                    }
	                    break;

                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
	                    {
	                        const unsigned short* value = reinterpret_cast<const unsigned short*>(&buffer.data[offset]);
	                        vertex.Set(attributeSemantic, value, size);
	                    }
	                    break;

                    case TINYGLTF_COMPONENT_TYPE_INT:
	                    {
	                        const int* value = reinterpret_cast<const int*>(&buffer.data[offset]);
	                        vertex.Set(attributeSemantic, value, size);
	                    }
	                    break;

                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
	                    {
	                        const unsigned int* value = reinterpret_cast<const unsigned int*>(&buffer.data[offset]);
	                        vertex.Set(attributeSemantic, value, size);
	                    }
	                    break;

                    case TINYGLTF_COMPONENT_TYPE_FLOAT:
	                    {
	                        const float* value = reinterpret_cast<const float*>(&buffer.data[offset]);
	                        vertex.Set(attributeSemantic, value, size);
	                    }
	                    break;

                    case TINYGLTF_COMPONENT_TYPE_DOUBLE:
	                    {
	                        const double* value = reinterpret_cast<const double*>(&buffer.data[offset]);
	                        vertex.Set(attributeSemantic, value, size);
	                    }
	                    break;
                    }
                }
            }

            Ref<IndexArray> indices;
            if (primitive.indices != -1)
            {
                const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];

                switch (indexAccessor.componentType)
                {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    indices = IndexArray::New(Renderer::IndexType::Byte, indexAccessor.count);
                    break;

                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    indices = IndexArray::New(Renderer::IndexType::Short, indexAccessor.count);
                    break;

                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                    indices = IndexArray::New(Renderer::IndexType::Integer, indexAccessor.count);
                    break;

				default:
                    assert(false);
                }

                const auto& bufferView = model.bufferViews[indexAccessor.bufferView];
                const auto& buffer = model.buffers[bufferView.buffer];
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

            switch (primitive.mode)
            {
            case TINYGLTF_MODE_POINTS:
                meshInfo.PrimitiveTopology = Renderer::PrimitiveTopology::Point;
                break;

            case TINYGLTF_MODE_LINE:
                meshInfo.PrimitiveTopology = Renderer::PrimitiveTopology::Line;
                break;

            case TINYGLTF_MODE_LINE_STRIP:
                meshInfo.PrimitiveTopology = Renderer::PrimitiveTopology::LineStrip;
                break;

            case TINYGLTF_MODE_TRIANGLES:
                meshInfo.PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
                break;

            case TINYGLTF_MODE_TRIANGLE_STRIP:
                meshInfo.PrimitiveTopology = Renderer::PrimitiveTopology::TriangleStrip;
                break;

            case TINYGLTF_MODE_TRIANGLE_FAN:
                meshInfo.PrimitiveTopology = Renderer::PrimitiveTopology::TriangleFan;
                break;

            default:
                assert(false);
            }

            meshInfo.Vertices = std::move(vertices);
            meshInfo.Indices = std::move(indices);
            meshInfo.MaterialIndex = primitive.material;

            mMeshes.push_back(meshInfo);
            mPrimitiveMeshes.insert({ &gltfMesh, mMeshes.size() - 1 });
        }
    }

    SceneContainer::NodeInfo GltfSceneContainer::ProcessNode(const tinygltf::Model& model, const tinygltf::Node& gltfNode)
    {
        std::vector<NodeInfo> childrenNodes;
        childrenNodes.reserve(gltfNode.children.size());
        for (int childSceneNodeIndex : gltfNode.children)
        {
            childrenNodes.push_back(
                ProcessNode(model, model.nodes[childSceneNodeIndex])
            );
        }

        std::vector<const MeshInfo*> meshes;
        auto [begin, end] = mPrimitiveMeshes.equal_range(&model.meshes[gltfNode.mesh]);
        for (auto it = begin; it != end; ++it)
        {
            const MeshInfo& meshInfo = mMeshes[it->second];
            meshes.push_back(&meshInfo);
        }

        Transformation localTransformation = Transformation::Identity();
        if (!gltfNode.translation.empty())
            localTransformation.SetTranslation(ToVec3(gltfNode.translation));

        if (!gltfNode.rotation.empty())
            localTransformation.SetRotation(ToQuat(gltfNode.rotation));

        if (!gltfNode.scale.empty())
            localTransformation.SetScale(ToVec3(gltfNode.scale));

        SceneContainer::NodeInfo nodeInfo;
        nodeInfo.Children = std::move(childrenNodes);
        nodeInfo.Meshes = std::move(meshes);
        nodeInfo.LocalTransformation = localTransformation;

        return nodeInfo;
    }
}