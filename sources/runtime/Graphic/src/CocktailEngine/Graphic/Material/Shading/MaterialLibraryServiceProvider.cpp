#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/System/SystemServiceProvider.hpp>
#include <CocktailEngine/Core/Utility/Finder.hpp>
#include <CocktailEngine/Core/Utility/StorageUtils.hpp>

#include <CocktailEngine/Graphic/Material/Shading/MaterialLibraryServiceProvider.hpp>
#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramLibrary.hpp>

namespace Ck
{
    COCKTAIL_REGISTER_SERVICE_PROVIDER(MaterialLibraryServiceProvider);

    namespace
    {
        HashMap<String, EnumMap<Renderer::ShaderType, ByteArray>> ParseVariantsBinaries(const Path& basePath, FileSystemDriver* driver)
        {
            Array<Path> filenames = Finder().WithExtension(CK_TEXT("spv")).From(basePath).Get(driver).Transform([](const Path& path) {
                return path.GetFilename();
            });

            HashMap<String, EnumMap<Renderer::ShaderType, ByteArray>> binariesMap;
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

        application->AfterBooted([&](Application*) {
            SharedPtr<MaterialProgramInterface> materialProgramInterface =
                MaterialProgramInterface::Builder()
                    .SetVertexAttribute(VertexAttributeSemantic::Position, "ck_VertexPosition")
                    .SetVertexAttribute(VertexAttributeSemantic::Normal, "ck_VertexNormal")
                    .SetVertexAttribute(VertexAttributeSemantic::TexCoord, "ck_VertexTexCoord")
                    .SetVertexAttribute(VertexAttributeSemantic::Color, "ck_VertexColor")
                    .SetVertexAttribute(VertexAttributeSemantic::Tangent, "ck_VertexTangent")
                    .SetVertexAttribute(VertexAttributeSemantic::BiTangent, "ck_VertexBiTangent")
                    .SetVertexAttribute(VertexAttributeSemantic::Joints, "ck_VertexJoints")
                    .SetVertexAttribute(VertexAttributeSemantic::Weights, "ck_VertexWeights")
                    .SetBindingSlot(ShaderBindingDomain::Scene, SceneBindingSlots::SceneInfo, "sceneInfo", Renderer::DescriptorType::UniformBuffer)
                    .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::CameraInfo, "cameraInfo", Renderer::DescriptorType::UniformBuffer)
                    .SetBindingSlot(ShaderBindingDomain::Viewport, ViewportBindingSlots::Lights, "lightsInfo", Renderer::DescriptorType::StorageBuffer)
                    .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::BaseColor, "ck_MaterialBaseColor", Renderer::DescriptorType::TextureSampler,
                                    Renderer::StaticSampler::TrilinearWrap)
                    .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::MetallicRoughness, "ck_MaterialMetallicRoughness",
                                    Renderer::DescriptorType::TextureSampler, Renderer::StaticSampler::TrilinearWrap)
                    .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Normal, "ck_MaterialNormal", Renderer::DescriptorType::TextureSampler,
                                    Renderer::StaticSampler::TrilinearWrap)
                    .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Alpha, "ck_MaterialAlpha", Renderer::DescriptorType::TextureSampler,
                                    Renderer::StaticSampler::TrilinearWrap)
                    .SetBindingSlot(ShaderBindingDomain::Material, MaterialBindingSlots::Emission, "ck_MaterialEmission", Renderer::DescriptorType::TextureSampler,
                                    Renderer::StaticSampler::TrilinearWrap)
                    .SetBindingSlot(ShaderBindingDomain::Drawcall, DrawcallBindingSlots::Instances, "instances", Renderer::DescriptorType::StorageBuffer)
                    .Get();

            application->Invoke([&](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_line");
                entry.Renderable = RenderableType::Line;
                entry.ShadingMode = Material::ShadingMode::Unlit;
                entry.Interface = materialProgramInterface;

                auto variantBinaries = ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/line/basic"), Path::Format::Generic), builtinDriver);
                for (auto& [shaderName, binaries] : variantBinaries)
                    entry.VariantsBinaries.Add(Move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([&](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_mesh");
                entry.Renderable = RenderableType::Mesh;
                entry.ShadingMode = Material::ShadingMode::Lit;
                entry.Interface = materialProgramInterface;

                auto variantBinaries = ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/lit/basic"), Path::Format::Generic), builtinDriver);
                for (auto& [shaderName, binaries] : variantBinaries)
                    entry.VariantsBinaries.Add(Move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([&](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("instanced_mesh");
                entry.Renderable = RenderableType::InstancedMesh;
                entry.ShadingMode = Material::ShadingMode::Lit;
                entry.Interface = materialProgramInterface;

                auto variantBinaries = ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/lit/instanced"), Path::Format::Generic), builtinDriver);
                for (auto& [shaderName, binaries] : variantBinaries)
                    entry.VariantsBinaries.Add(Move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([&](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_mesh");
                entry.Renderable = RenderableType::Mesh;
                entry.ShadingMode = Material::ShadingMode::Unlit;
                entry.Interface = materialProgramInterface;

                auto variantBinaries = ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/unlit/basic"), Path::Format::Generic), builtinDriver);
                for (auto& [shaderName, binaries] : variantBinaries)
                    entry.VariantsBinaries.Add(Move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([&](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("instanced_mesh");
                entry.Renderable = RenderableType::InstancedMesh;
                entry.ShadingMode = Material::ShadingMode::Unlit;
                entry.Interface = materialProgramInterface;

                auto variantBinaries = ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/unlit/instanced"), Path::Format::Generic), builtinDriver);
                for (auto& [shaderName, binaries] : variantBinaries)
                    entry.VariantsBinaries.Add(Move(binaries));

                materialProgramLibrary->Register(entry);
            });
        });
    }
}
