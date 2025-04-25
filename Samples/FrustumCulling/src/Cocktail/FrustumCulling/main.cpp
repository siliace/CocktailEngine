#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Keyboard/Keyboard.hpp>
#include <Cocktail/Core/System/Mouse/Mouse.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>

#include <Cocktail/Graphic/Mesh/MeshFactory.hpp>
#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/SceneLoader.hpp>
#include <Cocktail/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Scene/Camera/Controller/FreeFlyCameraController.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>
#include <Cocktail/Graphic/Scene/Light/PointLight.hpp>

#include <Cocktail/Main/ExitCode.hpp>

using namespace Ck;

Main::ExitCode ApplicationMain(Application* application)
{
	Extent2D windowSize = MakeExtent(800u, 600u);
	Ref<Window> window = application->Invoke([&](WindowFactory* windowFactory)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Size = windowSize;
		windowCreateInfo.Title = "Frustum Culling";
		windowCreateInfo.Style = Flags<WindowStyle>::All();
		windowCreateInfo.Position = MakeExtent(100, 100);

		return windowFactory->CreateWindow(windowCreateInfo);
	});

	Ref<GraphicEngine> graphicEngine = GraphicEngine::New(Renderer::GraphicApi::Vulkan);
	Ref<Scene> scene = Scene::New(graphicEngine);

	Ref<Mesh> cubeMesh = MeshFactory::CreateCube(1.f, LinearColor(1.f, 1.f, 1.f));
	Ref<Material> material = Material::New("default-cube", Material::ShadingMode::Phong, SamplerType::NearestClamp, true);
	material->SetEmissiveColor(LinearColor(1.f, 1.f, 1.f));
	Ref<Shape> shape = Shape::New(*graphicEngine, std::move(cubeMesh), std::vector<Ref<Material>>{ std::move(material) });

	for (unsigned int i = 0; i < 30; i++)
	{
		for (unsigned int j = 0; j < 30; j++)
		{
			Ref<SceneNode> sceneNode = scene->CreateSceneNode();
			sceneNode->SetPosition(Vector3<float>((i - 15.f) * 2.f, (j - 15.f) * 2.f, 0.f));
			sceneNode->AddShape(shape);
		}
	}

	Vector3<float> lightDirection = Vector3<float>::Normalize(Vector3<float>::Down() - Vector3<float>::Right());
	Ref<DirectionalLight> directionalLight = DirectionalLight::Create(scene, LinearColor(1.f, 1.f, 1.f), lightDirection);

	float aspectRatio = static_cast<float>(windowSize.Width) / static_cast<float>(windowSize.Height);
	Vector2<float> zBounds(0.1f, 1000.f);

	Ref<PerspectiveCamera> camera = PerspectiveCamera::Create(scene, Angle<float>::Degree(45.f), aspectRatio, zBounds);
	camera->SetPosition(Vector3<float>(0.f, 0.f, 10.f));
	
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
