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

        application->AfterBooted<SystemServiceProvider>([](Application* application, ServiceProvider* systemServiceProvider) {
            application->Invoke([](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_line");
                entry.RenderableType = RenderableType::Line;
                entry.ShadingMode = Material::ShadingMode::Unlit;
                entry.Interface.VertexAttributes[VertexAttributeSemantic::Position] = "ck_VertexPosition";
                entry.Interface.VertexAttributes[VertexAttributeSemantic::Color] = "ck_VertexColor";

                for (auto& [shaderName, binaries] : ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/line"), Path::Format::Generic), builtinDriver))
                    entry.VariantsBinaries.Add(std::move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_mesh");
                entry.RenderableType = RenderableType::Mesh;
                entry.ShadingMode = Material::ShadingMode::Lit;
                entry.Interface.VertexAttributes[VertexAttributeSemantic::Position] = "ck_VertexPosition";
                entry.Interface.VertexAttributes[VertexAttributeSemantic::Normal] = "ck_VertexNormal";
                entry.Interface.VertexAttributes[VertexAttributeSemantic::TexCoord] = "ck_VertexTexCoord";
                entry.Interface.VertexAttributes[VertexAttributeSemantic::Color] = "ck_VertexColor";
                entry.Interface.VertexAttributes[VertexAttributeSemantic::Tangent] = "ck_VertexTangent";
                entry.Interface.VertexAttributes[VertexAttributeSemantic::BiTangent] = "ck_VertexBiTangent";
                entry.Interface.Textures[Material::TextureType::BaseColor] = { "ck_MaterialBaseColor", Renderer::StaticSampler::TrilinearWrap };
                entry.Interface.Textures[Material::TextureType::MetallicRoughness] = { "ck_MaterialMetallicRoughness", Renderer::StaticSampler::TrilinearWrap };
                entry.Interface.Textures[Material::TextureType::Normal] = { "ck_MaterialNormal", Renderer::StaticSampler::TrilinearWrap };
                entry.Interface.Textures[Material::TextureType::Alpha] = { "ck_MaterialAlpha", Renderer::StaticSampler::TrilinearWrap };

                for (auto& [shaderName, binaries] : ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/lit"), Path::Format::Generic), builtinDriver))
                    entry.VariantsBinaries.Add(std::move(binaries));

                materialProgramLibrary->Register(entry);
            });

            application->Invoke([](MaterialProgramLibrary* materialProgramLibrary) {
                FileSystemDriver* builtinDriver = Storage::ResolveDriver(CK_TEXT("builtin"));

                MaterialProgramLibrary::Entry entry;
                entry.Name = CK_TEXT("basic_mesh");
                entry.RenderableType = RenderableType::Mesh;
                entry.ShadingMode = Material::ShadingMode::Unlit;
                entry.Interface.VertexAttributes[VertexAttributeSemantic::Position] = "ck_VertexPosition";
                entry.Interface.VertexAttributes[VertexAttributeSemantic::TexCoord] = "ck_VertexTexCoord";
                entry.Interface.VertexAttributes[VertexAttributeSemantic::Color] = "ck_VertexColor";
                entry.Interface.Textures[Material::TextureType::BaseColor] = { "ck_MaterialBaseColor", Renderer::StaticSampler::TrilinearWrap };
                entry.Interface.Textures[Material::TextureType::Normal] = { "ck_MaterialNormal", Renderer::StaticSampler::TrilinearWrap };
                entry.Interface.Textures[Material::TextureType::Alpha] = { "ck_MaterialAlpha", Renderer::StaticSampler::TrilinearWrap };

                for (auto& [shaderName, binaries] : ParseVariantsBinaries(Path::Parse(CK_TEXT("graphic/resources/shaders/mesh/unlit"), Path::Format::Generic), builtinDriver))
                    entry.VariantsBinaries.Add(std::move(binaries));

                materialProgramLibrary->Register(entry);
            });
        });
    }
}
