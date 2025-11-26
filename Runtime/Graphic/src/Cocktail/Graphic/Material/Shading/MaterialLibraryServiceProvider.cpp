#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>
#include <Cocktail/Core/Utility/StorageUtils.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialLibraryServiceProvider.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp>

namespace Ck
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(MaterialLibraryServiceProvider);

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
				MaterialProgramLibrary::Entry entry;
				entry.Name = CK_TEXT("basic_mesh");
				entry.RenderableType = RenderableType::Mesh;
				entry.ShadingMode = Material::ShadingMode::Phong;
				entry.Interface.VertexAttributes[VertexAttributeSemantic::Position] = "ck_VertexPosition";
				entry.Interface.VertexAttributes[VertexAttributeSemantic::TexCoord] = "ck_VertexTexCoord";
				entry.Interface.VertexAttributes[VertexAttributeSemantic::Normal] = "ck_VertexNormal";
				entry.Interface.VertexAttributes[VertexAttributeSemantic::Color] = "ck_VertexColor";
				entry.Interface.Textures[Material::TextureType::BaseColor] = { "ck_MaterialBaseColor", Renderer::StaticSampler::TrilinearWrap };
				entry.Interface.Textures[Material::TextureType::Alpha] = { "ck_MaterialAlpha", Renderer::StaticSampler::TrilinearWrap };

				{
					ByteArray basicMeshVertexStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/default.vert.spv"));
					ByteArray basicMeshFragmentStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/default.frag.spv"));

					EnumMap<Renderer::ShaderType, ByteArray> binaries;
					binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					entry.VariantsBinaries.Add(std::move(binaries));
				}

				{
					ByteArray basicMeshVertexStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/default-no-normal.vert.spv"));
					ByteArray basicMeshFragmentStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/default-no-normal.frag.spv"));

					EnumMap<Renderer::ShaderType, ByteArray> binaries;
					binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					entry.VariantsBinaries.Add(std::move(binaries));
				}

				{
					ByteArray basicMeshVertexStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/colored.vert.spv"));
					ByteArray basicMeshFragmentStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/colored.frag.spv"));

					EnumMap<Renderer::ShaderType, ByteArray> binaries;
					binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					entry.VariantsBinaries.Add(std::move(binaries));
				}

				{
					ByteArray basicMeshVertexStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/textured.vert.spv"));
					ByteArray basicMeshFragmentStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/textured.frag.spv"));

					EnumMap<Renderer::ShaderType, ByteArray> binaries;
					binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					entry.VariantsBinaries.Add(std::move(binaries));
				}

				{
					ByteArray basicMeshVertexStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/textured-blended.vert.spv"));
					ByteArray basicMeshFragmentStage = StorageUtils::ReadFile(CK_TEXT("builtin://graphic/resources/shaders/mesh/textured-blended.frag.spv"));

					EnumMap<Renderer::ShaderType, ByteArray> binaries;
					binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					entry.VariantsBinaries.Add(std::move(binaries));
				}

				materialProgramLibrary->Register(entry);
			});
		});
	}
}
