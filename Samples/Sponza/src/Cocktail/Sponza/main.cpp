#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/Keyboard/Keyboard.hpp>
#include <Cocktail/Core/System/Mouse/Mouse.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>

#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/SceneLoader.hpp>
#include <Cocktail/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Scene/Camera/Controller/FreeFlyCameraController.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>
#include <Cocktail/Graphic/Scene/Light/PointLight.hpp>

#include <Cocktail/Main/ExitCode.hpp>

#include <Cocktail/Vulkan/Vulkan.hpp>

using namespace Ck;

class MeshFactory
{
public:

	static Ref<Mesh> CreateCube()
	{
		static const Vector3<float> VertexPositions[] = {
			Vector3<float>(0.5f,  0.5f,  0.5f),
			Vector3<float>(-0.5f,  0.5f,  0.5f),
			Vector3<float>(0.5f, -0.5f,  0.5f),
			Vector3<float>(-0.5f, -0.5f,  0.5f),

			Vector3<float>(0.5f,  0.5f, -0.5f),
			Vector3<float>(-0.5f,  0.5f, -0.5f),
			Vector3<float>(0.5f, -0.5f, -0.5f),
			Vector3<float>(-0.5f, -0.5f, -0.5f),

			Vector3<float>(0.5f,  0.5f,  0.5f),
			Vector3<float>(0.5f, -0.5f,  0.5f),
			Vector3<float>(0.5f,  0.5f, -0.5f),
			Vector3<float>(0.5f, -0.5f, -0.5f),

			Vector3<float>(-0.5f,  0.5f,  0.5f),
			Vector3<float>(-0.5f, -0.5f,  0.5f),
			Vector3<float>(-0.5f,  0.5f, -0.5f),
			Vector3<float>(-0.5f, -0.5f, -0.5f),

			Vector3<float>(-0.5f,  0.5f,  0.5f),
			Vector3<float>(0.5f,  0.5f,  0.5f),
			Vector3<float>(-0.5f,  0.5f, -0.5f),
			Vector3<float>(0.5f,  0.5f, -0.5f),

			Vector3<float>(0.5f, -0.5f,  0.5f),
			Vector3<float>(-0.5f, -0.5f,  0.5f),
			Vector3<float>(0.5f, -0.5f, -0.5f),
			Vector3<float>(-0.5f, -0.5f, -0.5f),
		};

		static const unsigned short VertexIndices[] = {
			0, 1, 2,
			1, 2, 3,

			4, 5, 6,
			5, 6, 7,

			8, 9, 10,
			9, 10, 11,

			12, 13, 14,
			13, 14, 15,

			16, 17, 18,
			17, 18, 19,

			20, 21, 22,
			21, 22, 23
		};

		Ref<VertexLayout> vertexLayout = VertexLayout::Builder()
		                                 .AddAttribute(VertexAttributeSemantic::Position, DataType::Float32, 3)
		                                 .Get();

		Ref<VertexArray> vertices = VertexArray::New(vertexLayout, 24);
		for (std::size_t i = 0; i < vertices->GetVertexCount(); i++)
		{
			VertexRef vertex = vertices->At(i);
			vertex.Set(VertexAttributeSemantic::Position, VertexPositions[i]);
		}

		Ref<IndexArray> indices = IndexArray::New(Renderer::IndexType::Short, 36);
		for (std::size_t i = 0; i < indices->GetIndexCount(); i++)
		{
			IndexRef index = indices->At(i);
			index.Set<unsigned short>(VertexIndices[i]);
		}

		return Mesh::New(std::move(vertices), std::move(indices), Renderer::PrimitiveTopology::Triangle);
	}

	static Ref<Material> DefaultMaterial()
	{
		Ref<Material> material = Material::New("default", Material::ShadingMode::Phong, SamplerType::NearestClamp, true);
		material->SetEmissiveColor(LinearColor(1.f, 1.f, 1.f));
		return material;
	}
};

Main::ExitCode ApplicationMain(Ck::Application* application)
{
	Extent2D windowSize = MakeExtent(800u, 600u);
	Vulkan::ApiVersion vulkanVersion = Vulkan::GetSupportedApiVersion();
	Log::Info("Starting sponza sampler using vulkan {}", Enum<Vulkan::ApiVersion>::ToString(vulkanVersion));

	Ref<Window> window = application->Invoke([&](WindowFactory* windowFactory)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Size = windowSize;
		windowCreateInfo.Title = "Hello Sponza";
		windowCreateInfo.Style = Flags<WindowStyle>::All();
		windowCreateInfo.Position = MakeExtent(100, 100);

		return windowFactory->CreateWindow(windowCreateInfo);
	});

	Ref<GraphicEngine> graphicEngine = GraphicEngine::New(*application, Renderer::GraphicApi::Vulkan);
	Ref<Scene> scene = Scene::New(graphicEngine);

	Ref<SceneNode> sceneNode = application->Invoke([&](SceneLoader* sceneLoader) {
		return sceneLoader->LoadFromPath("./resources/Models/Sponza/Sponza.gltf", {})->AddToScene(*scene);
	});

	Vector3<float> lightDirection = Vector3<float>::Normalize(Vector3<float>::Down() - Vector3<float>::Right());
	Ref<DirectionalLight> directionalLight = DirectionalLight::Create(scene, LinearColor(1.f, 1.f, 1.f), lightDirection);
	
	Ref<PointLight> pointLight = PointLight::Create(scene);
	Ref<SceneNode> cube = scene->CreateSceneNode();
	cube->AttachTo(*pointLight);
	cube->AddShape(Shape::New(
		*graphicEngine,
		MeshFactory::CreateCube(),
		std::vector<Ref<Material>>{ MeshFactory::DefaultMaterial() }
	));

	pointLight->SetPosition(Vector3<float>(0.f, 3.f, 0.f));

	float aspectRatio = static_cast<float>(windowSize.Width) / static_cast<float>(windowSize.Height);
	Vector2<float> zBounds(0.1f, 4500.f);
	Ref<PerspectiveCamera> camera = PerspectiveCamera::Create(scene, Angle<float>::Degree(60.f), aspectRatio, zBounds);
	Ref<FreeFlyCameraViewController> cameraController = FreeFlyCameraViewController::New(camera);

	float move = 1.f;
	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Q), [&](KeyboardEvent event)
	{
		cameraController->Translate(FreeFlyCameraViewController::TranslationAxis::Right, -move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::D), [&](KeyboardEvent event)
	{
		cameraController->Translate(FreeFlyCameraViewController::TranslationAxis::Right, move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Z), [&](KeyboardEvent event)
	{
		cameraController->Translate(FreeFlyCameraViewController::TranslationAxis::Front, -move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::S), [&](KeyboardEvent event)
	{
		cameraController->Translate(FreeFlyCameraViewController::TranslationAxis::Front, move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Add), [&](KeyboardEvent event)
	{
		move *= 2.f;
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Subtract), [&](KeyboardEvent event)
	{
		move /= 2.f;
	});

	application->Connect(Mouse::OnMouseMovedEvent(), [&](MouseMovedEvent event)
	{
		if (!(event.Flags & MouseEventFlagBits::LeftButton))
			return;

		Vector2<float> offset;
		offset.X() = static_cast<float>(event.RelativeDisplacement.Width);
		offset.Y() = static_cast<float>(event.RelativeDisplacement.Height);
		offset *= 0.1f;

		cameraController->Rotate(FreeFlyCameraViewController::RotationAxis::Yaw, Angle<float>::Degree(offset.X()));
		cameraController->Rotate(FreeFlyCameraViewController::RotationAxis::Pitch, Angle<float>::Degree(offset.Y()));
	});

	SceneViewerParameters viewerParameters;
	viewerParameters.DepthStencilFormat = PixelFormat::DepthStencil(24, 8);
	viewerParameters.Samples = Renderer::RasterizationSamples::e4;
	Ref<SceneViewer> viewer = WindowSceneViewer::New(scene, window, viewerParameters, true);
	Ref<Viewport> viewport = Viewport::New(camera, Rectangle(Vector2<unsigned int>(0, 0), Vector2(windowSize.Width, windowSize.Height)));
	viewer->AttachViewport(viewport, 0);

	application->Connect(window->OnResizedEvent(), [&](WindowResizedEvent event)
	{
		windowSize = event.Size;
		camera->SetAspectRatio((float)windowSize.Width / (float)windowSize.Height);
		viewport->SetArea(Rectangle(Vector2<unsigned int>(0, 0), Vector2(windowSize.Width, windowSize.Height)));
	});

	Duration lastFrameBegin = application->Uptime();
	while (window->PollEvents())
	{
		Duration frameBegin = application->Uptime();

		cameraController->Update(Duration::Between(lastFrameBegin, frameBegin));

		viewer->Render();

		graphicEngine->Present();
		lastFrameBegin = frameBegin;
	}

	return Main::ExitCode::Success;
}
