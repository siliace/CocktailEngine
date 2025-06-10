#include <cmrc/cmrc.hpp>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>
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

#include <Cocktail/Sponza/ImGuiOverlay.hpp>

using namespace Ck;

CMRC_DECLARE(sponza);

Main::ExitCode ApplicationMain(Application* application)
{
	application->Invoke([](EmbeddedFileSystemDriver* embeddedDriver) {
		embeddedDriver->Register(cmrc::sponza::get_filesystem());
	});

	Extent2D windowSize = MakeExtent(800u, 600u);
	std::shared_ptr<Window> window = application->Invoke([&](WindowFactory* windowFactory)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Size = windowSize;
		windowCreateInfo.Title = "Hello Sponza";
		windowCreateInfo.Style = Flags<WindowStyle>::All(); 
		windowCreateInfo.Position = MakeExtent(100, 100);

		return windowFactory->CreateWindow(windowCreateInfo);
	});

	std::shared_ptr<GraphicEngine> graphicEngine = std::make_shared<GraphicEngine>(Renderer::GraphicApi::Vulkan);
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(graphicEngine);

	std::shared_ptr<SceneNode> sceneNode = application->Invoke([&](SceneLoader* sceneLoader) {
		return sceneLoader->LoadFromPath("./resources/Models/Sponza/Sponza.gltf", {})->AddToScene(*scene);
	});

	Vector3<float> lightDirection = Vector3<float>::Normalize(Vector3<float>::Down() - Vector3<float>::Right());
	DirectionalLight* directionalLight = DirectionalLight::Create(scene, LinearColor(1.f, 1.f, 1.f), lightDirection);

	float aspectRatio = static_cast<float>(windowSize.Width) / static_cast<float>(windowSize.Height);
	Vector2<float> zBounds(0.1f, 4500.f);
	PerspectiveCamera* camera = PerspectiveCamera::Create(scene, Angle<float>::Degree(60.f), aspectRatio, zBounds);
	camera->SetPosition(Vector3<float>(0.f, 3.f, 0.f));
	FreeFlyCameraViewController cameraController(camera);

	float move = 1.f;
	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Q), [&](KeyboardEvent event)
	{
		cameraController.Translate(FreeFlyCameraViewController::TranslationAxis::Right, -move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::D), [&](KeyboardEvent event)
	{
		cameraController.Translate(FreeFlyCameraViewController::TranslationAxis::Right, move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Z), [&](KeyboardEvent event)
	{
		cameraController.Translate(FreeFlyCameraViewController::TranslationAxis::Front, -move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::S), [&](KeyboardEvent event)
	{
		cameraController.Translate(FreeFlyCameraViewController::TranslationAxis::Front, move);
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

		cameraController.Rotate(FreeFlyCameraViewController::RotationAxis::Yaw, Angle<float>::Degree(offset.X()));
		cameraController.Rotate(FreeFlyCameraViewController::RotationAxis::Pitch, Angle<float>::Degree(offset.Y()));
	});

	SceneViewerParameters viewerParameters;
	viewerParameters.DepthStencilFormat = PixelFormat::DepthStencil(24, 8);
	viewerParameters.Samples = Renderer::RasterizationSamples::e4;
	std::shared_ptr<SceneViewer> viewer = std::make_shared<WindowSceneViewer>(scene, window, viewerParameters, true);
	std::shared_ptr<Viewport> viewport = std::make_shared<Viewport>(camera, Rectangle(Vector2<unsigned int>(0, 0), Vector2(windowSize.Width, windowSize.Height)));
	viewer->AttachViewport(viewport, 0);

	ImGuiOverlay overlay(*window, *viewer, *graphicEngine->GetRenderDevice(), *graphicEngine->GetRenderContext());

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

		cameraController.Update(Duration::Between(lastFrameBegin, frameBegin));
		overlay.Update();

		viewer->Render();

		graphicEngine->Present();
		lastFrameBegin = frameBegin;
	}

	return Main::ExitCode::Success;
}
