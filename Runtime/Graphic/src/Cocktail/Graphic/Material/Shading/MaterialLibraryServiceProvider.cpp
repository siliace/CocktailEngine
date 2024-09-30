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
				{
					std::string basicMeshVertexStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/colored.vert.spv");
					std::string basicMeshFragmentStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/colored.frag.spv");

					MaterialProgramInterface materialProgramInterface;
					materialProgramInterface.VertexAttributes[VertexAttributeSemantic::Position] = "ck_VertexPosition";
					materialProgramInterface.VertexAttributes[VertexAttributeSemantic::Color] = "ck_VertexColor";

					MaterialProgramLibrary::Entry entry;
					entry.Name = "colored_mesh";
					entry.RenderableType = RenderableType::Mesh;
					entry.ShadingMode = Material::ShadingMode::Phong;
					entry.Interface = std::move(materialProgramInterface);
					entry.Binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					entry.Binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					materialProgramLibrary->Register(entry);
				}

				{
					std::string basicMeshVertexStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/default.vert.spv");
					std::string basicMeshFragmentStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/default.frag.spv");

					MaterialProgramInterface materialProgramInterface;
					materialProgramInterface.VertexAttributes[VertexAttributeSemantic::Position] = "ck_VertexPosition";

					MaterialProgramLibrary::Entry entry;
					entry.Name = "default_mesh";
					entry.RenderableType = RenderableType::Mesh;
					entry.ShadingMode = Material::ShadingMode::Phong;
					entry.Interface = std::move(materialProgramInterface);
					entry.Binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					entry.Binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					materialProgramLibrary->Register(entry);
				}

				{
					std::string basicMeshVertexStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/textured.vert.spv");
					std::string basicMeshFragmentStage = FileUtils::ReadFile("builtin://graphic/resources/shaders/mesh/textured.frag.spv");

					MaterialProgramInterface materialProgramInterface;
					materialProgramInterface.VertexAttributes[VertexAttributeSemantic::Position] = "ck_VertexPosition";
					materialProgramInterface.VertexAttributes[VertexAttributeSemantic::TexCoord] = "ck_VertexTexCoord";
					materialProgramInterface.VertexAttributes[VertexAttributeSemantic::Normal] = "ck_VertexNormal";
					materialProgramInterface.Textures[Material::TextureType::BaseColor] = "ck_MaterialBaseColor";

					MaterialProgramLibrary::Entry entry;
					entry.Name = "textured_mesh";
					entry.RenderableType = RenderableType::Mesh;
					entry.ShadingMode = Material::ShadingMode::Phong;
					entry.Interface = std::move(materialProgramInterface);
					entry.Binaries[Renderer::ShaderType::Vertex] = std::move(basicMeshVertexStage);
					entry.Binaries[Renderer::ShaderType::Fragment] = std::move(basicMeshFragmentStage);

					materialProgramLibrary->Register(entry);
				}
			});
		});
	}
}
