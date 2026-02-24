#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>
#include <Cocktail/Core/Utility/Finder.hpp>
#include <Cocktail/Core/Utility/StorageUtils.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialLibraryServiceProvider.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp>

namespace Ck
{
    COCKTAIL_REGISTER_SERVICE_PROVIDER(MaterialLibraryServiceProvider);

    namespace
    {
        std::unordered_map<String, EnumMap<Renderer::ShaderType, ByteArray>> ParseVariantsBinaries(const Path& basePath, FileSystemDriver* driver)
        {
            Array<Path> filenames = Finder().WithExtension(CK_TEXT("spv")).From(basePath).Get(driver).Transform([](const Path& path) {
                return path.GetFilename();
            });

            std::unordered_map<String, EnumMap<Renderer::ShaderType, ByteArray>> binariesMap;
            for (const Path& filename : filenames)
            {
                Path shaderName = filename.GetFilename();
                String extension = filename.GetExtension();
                Path shaderFullPath = basePath;
                shaderFullPath.Join(filename.ToString() + CK_TEXT(".spv"));

                Renderer::ShaderType shaderType = {};
                if (filename.HasExtension(CK_TEXT("vert")))
                {
                    shaderType = Renderer::ShaderType::Vertex;
                }
                else if (filename.HasExtension(CK_TEXT("geom")))
                {
                    shaderType = Renderer::ShaderType::Geometry;
                }
                else if (filename.HasExtension(CK_TEXT("frag")))
                {
                    shaderType = Renderer::ShaderType::Fragment;
                }

                binariesMap[shaderName.ToString()][shaderType] = FileUtils::ReadFile(shaderFullPath, driver);
            }

            return binariesMap;
        }
    }

    MaterialLibraryServiceProvider::MaterialLibraryServiceProvider(Application* application) :
        ServiceProvider(application)
    {
        /// Nothing
    }

    void MaterialLibraryServiceProvider::DoRegister(Application* application)
    {
        application->Singleton<MaterialProgramLibrary>();

        application->AfterBooted<SystemServiceProvider>([this](Application* application, ServiceProvider* systemServiceProvider) {
            application->Invoke([this](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_line");
                entry.RenderableType = RenderableType::Line;
                entry.ShadingMode = Material::ShadingMode::Unlit;
                entry.Interface = MaterialProgramInterface::Builder()
                .SetVertexAttribute(VertexAttributeSemantic::Position, "ck_VertexPosition")
                .SetVertexAttribute(VertexAttributeSemantic::Normal, "ck_VertexNormal")
                .SetVertexAttribute(VertexAttributeSemantic::TexCoord, "ck_VertexTexCoord")
                .SetVertexAttribute(VertexAttributeSemantic::Color, "ck_VertexColor")
                .SetVertexAttribute(VertexAttributeSemantic::Tangent, "ck_VertexTangent")
                .SetVertexAttribute(VertexAttributeSemantic::BiTangent, "ck_VertexBiTangent")
                .SetBindingSlot(ShaderBindingDomain::Scene, SceneBindingSlots::SceneInfo, "sceneInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::CameraInfo, "cameraInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::Lights, "lightsInfo", Renderer::DescriptorType::StorageBuffer)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::BaseColor, "ck_MaterialBaseColor", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::MetallicRoughness, "ck_MaterialMetallicRoughness", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Normal, "ck_MaterialNormal", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Alpha, "ck_MaterialAlpha", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Emission, "ck_MaterialEmission", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Drawcall, DrawcallBindingSlots::Instances, "instances", Renderer::DescriptorType::StorageBuffer)
                .Get();;

                for (auto& [shaderName, binaries] : ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/line/basic"), Path::Format::Generic), builtinDriver))
                    entry.VariantsBinaries.Add(std::move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([this](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_mesh");
                entry.RenderableType = RenderableType::Mesh;
                entry.ShadingMode = Material::ShadingMode::Lit;
                entry.Interface = MaterialProgramInterface::Builder()
                .SetVertexAttribute(VertexAttributeSemantic::Position, "ck_VertexPosition")
                .SetVertexAttribute(VertexAttributeSemantic::Normal, "ck_VertexNormal")
                .SetVertexAttribute(VertexAttributeSemantic::TexCoord, "ck_VertexTexCoord")
                .SetVertexAttribute(VertexAttributeSemantic::Color, "ck_VertexColor")
                .SetVertexAttribute(VertexAttributeSemantic::Tangent, "ck_VertexTangent")
                .SetVertexAttribute(VertexAttributeSemantic::BiTangent, "ck_VertexBiTangent")
                .SetBindingSlot(ShaderBindingDomain::Scene, SceneBindingSlots::SceneInfo, "sceneInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::CameraInfo, "cameraInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::Lights, "lightsInfo", Renderer::DescriptorType::StorageBuffer)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::BaseColor, "ck_MaterialBaseColor", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::MetallicRoughness, "ck_MaterialMetallicRoughness", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Normal, "ck_MaterialNormal", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Alpha, "ck_MaterialAlpha", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Emission, "ck_MaterialEmission", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Drawcall, DrawcallBindingSlots::Instances, "instances", Renderer::DescriptorType::StorageBuffer)
                .Get();;

                for (auto& [shaderName, binaries] : ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/lit/basic"), Path::Format::Generic), builtinDriver))
                    entry.VariantsBinaries.Add(std::move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([this](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("instanced_mesh");
                entry.RenderableType = RenderableType::InstancedMesh;
                entry.ShadingMode = Material::ShadingMode::Lit;
                entry.Interface = MaterialProgramInterface::Builder()
                .SetVertexAttribute(VertexAttributeSemantic::Position, "ck_VertexPosition")
                .SetVertexAttribute(VertexAttributeSemantic::Normal, "ck_VertexNormal")
                .SetVertexAttribute(VertexAttributeSemantic::TexCoord, "ck_VertexTexCoord")
                .SetVertexAttribute(VertexAttributeSemantic::Color, "ck_VertexColor")
                .SetVertexAttribute(VertexAttributeSemantic::Tangent, "ck_VertexTangent")
                .SetVertexAttribute(VertexAttributeSemantic::BiTangent, "ck_VertexBiTangent")
                .SetBindingSlot(ShaderBindingDomain::Scene, SceneBindingSlots::SceneInfo, "sceneInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::CameraInfo, "cameraInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::Lights, "lightsInfo", Renderer::DescriptorType::StorageBuffer)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::BaseColor, "ck_MaterialBaseColor", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::MetallicRoughness, "ck_MaterialMetallicRoughness", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Normal, "ck_MaterialNormal", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Alpha, "ck_MaterialAlpha", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Emission, "ck_MaterialEmission", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Drawcall, DrawcallBindingSlots::Instances, "instances", Renderer::DescriptorType::StorageBuffer)
                .Get();;

                for (auto& [shaderName, binaries] :
                     ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/lit/instanced"), Path::Format::Generic), builtinDriver))
                    entry.VariantsBinaries.Add(std::move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([this](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_mesh");
                entry.RenderableType = RenderableType::Mesh;
                entry.ShadingMode = Material::ShadingMode::Unlit;
                entry.Interface = MaterialProgramInterface::Builder()
                .SetVertexAttribute(VertexAttributeSemantic::Position, "ck_VertexPosition")
                .SetVertexAttribute(VertexAttributeSemantic::Normal, "ck_VertexNormal")
                .SetVertexAttribute(VertexAttributeSemantic::TexCoord, "ck_VertexTexCoord")
                .SetVertexAttribute(VertexAttributeSemantic::Color, "ck_VertexColor")
                .SetVertexAttribute(VertexAttributeSemantic::Tangent, "ck_VertexTangent")
                .SetVertexAttribute(VertexAttributeSemantic::BiTangent, "ck_VertexBiTangent")
                .SetBindingSlot(ShaderBindingDomain::Scene, SceneBindingSlots::SceneInfo, "sceneInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::CameraInfo, "cameraInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::Lights, "lightsInfo", Renderer::DescriptorType::StorageBuffer)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::BaseColor, "ck_MaterialBaseColor", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::MetallicRoughness, "ck_MaterialMetallicRoughness", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Normal, "ck_MaterialNormal", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Alpha, "ck_MaterialAlpha", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Emission, "ck_MaterialEmission", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Drawcall, DrawcallBindingSlots::Instances, "instances", Renderer::DescriptorType::StorageBuffer)
                .Get();;

                for (auto& [shaderName, binaries] : ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/unlit/basic"), Path::Format::Generic), builtinDriver))
                    entry.VariantsBinaries.Add(std::move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([this](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("instanced_mesh");
                entry.RenderableType = RenderableType::InstancedMesh;
                entry.ShadingMode = Material::ShadingMode::Unlit;
                entry.Interface = MaterialProgramInterface::Builder()
                .SetVertexAttribute(VertexAttributeSemantic::Position, "ck_VertexPosition")
                .SetVertexAttribute(VertexAttributeSemantic::Normal, "ck_VertexNormal")
                .SetVertexAttribute(VertexAttributeSemantic::TexCoord, "ck_VertexTexCoord")
                .SetVertexAttribute(VertexAttributeSemantic::Color, "ck_VertexColor")
                .SetVertexAttribute(VertexAttributeSemantic::Tangent, "ck_VertexTangent")
                .SetVertexAttribute(VertexAttributeSemantic::BiTangent, "ck_VertexBiTangent")
                .SetBindingSlot(ShaderBindingDomain::Scene, SceneBindingSlots::SceneInfo, "sceneInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::CameraInfo, "cameraInfo", Renderer::DescriptorType::UniformBuffer)
                .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::Lights, "lightsInfo", Renderer::DescriptorType::StorageBuffer)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::BaseColor, "ck_MaterialBaseColor", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::MetallicRoughness, "ck_MaterialMetallicRoughness", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Normal, "ck_MaterialNormal", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Alpha, "ck_MaterialAlpha", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Emission, "ck_MaterialEmission", Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                .SetBindingSlot(ShaderBindingDomain::Drawcall, DrawcallBindingSlots::Instances, "instances", Renderer::DescriptorType::StorageBuffer)
                .Get();;

                for (auto& [shaderName, binaries] :
                     ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/unlit/instanced"), Path::Format::Generic), builtinDriver))
                    entry.VariantsBinaries.Add(std::move(binaries));

                materialProgramLibrary->Register(entry);
            });
        });
    }
}
