#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialLibraryServiceProvider.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp>

namespace Ck
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(MaterialLibraryServiceProvider);

	MaterialLibraryServiceProvider::MaterialLibraryServiceProvider(Ref<Application> application) :
		Super(std::move(application))
	{
		/// Nothing
	}

	void MaterialLibraryServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<MaterialProgramLibrary>();

		application->AfterBooted<SystemServiceProvider>([](Ref<Application> application, Ref<ServiceProvider> serviceProvider) {
			application->Invoke([](Ref<MaterialProgramLibrary> materialProgramLibrary) {
				MaterialProgramLibrary::Entry entry;
				entry.Name = "basic_mesh";
				entry.RenderableType = RenderableType::Mesh;
				entry.ShadingMode = Material::ShadingMode::Phong;
				entry.Interface.VertexAttributes[VertexAttributeSemantic::Position] = "ck_VertexPosition";
				entry.Interface.VertexAttributes[VertexAttributeSemantic::TexCoord] = "ck_VertexTexCoord";
				entry.Interface.VertexAttributes[VertexAttributeSemantic::Normal] = "ck_VertexNormal";
				entry.Interface.Textures[Material::TextureType::BaseColor] = "ck_MaterialBaseColor";

				{
					std::string basicMeshVertexStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/colored.vert.spv");
					std::string basicMeshFragmentStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/colored.frag.spv");

					EnumMap<Renderer::ShaderType, std::string> binaries;
					binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					entry.VariantsBinaries.push_back(std::move(binaries));
				}

				{
					std::string basicMeshVertexStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/default.vert.spv");
					std::string basicMeshFragmentStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/default.frag.spv");

					EnumMap<Renderer::ShaderType, std::string> binaries;
					binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					entry.VariantsBinaries.push_back(std::move(binaries));
				}

				{
					std::string basicMeshVertexStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/textured.vert.spv");
					std::string basicMeshFragmentStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/textured.frag.spv");

					EnumMap<Renderer::ShaderType, std::string> binaries;
					binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					entry.VariantsBinaries.push_back(std::move(binaries));
				}

				materialProgramLibrary->Register(entry);
			});
		});
	}
}
