#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/Keyboard/Keyboard.hpp>
#include <Cocktail/Core/System/Mouse/Mouse.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>

#include <Cocktail/Graphic/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Camera/Controller/FreeFlyCameraController.hpp>
#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/SceneLoader.hpp>

#include <Cocktail/Main/ExitCode.hpp>

#include <Cocktail/Vulkan/Vulkan.hpp>

using namespace Ck;

Main::ExitCode ApplicationMain(Ref<Application> application)
{
	Extent2D windowSize = MakeExtent(800u, 600u);
	Vulkan::ApiVersion vulkanVersion = Vulkan::GetSupportedApiVersion();
	Log::Info("Starting sponza sampler using vulkan {}", Enum<Vulkan::ApiVersion>::ToString(vulkanVersion));

	Ref<Window> window = application->Invoke([&](const Ref<WindowFactory>& windowFactory) {
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Size = windowSize;
		windowCreateInfo.Title = "Hello Sponza";
		windowCreateInfo.Style = Flags<WindowStyle>::All();
		windowCreateInfo.Position = MakeExtent(100, 100);

		return windowFactory->CreateWindow(windowCreateInfo);
	});

	Ref<GraphicEngine> graphicEngine = GraphicEngine::New(*application, Renderer::GraphicApi::Vulkan);
	Ref<Scene> scene = Scene::New(graphicEngine);
	
	Ref<SceneNode> sceneNode = application->Invoke([&](Ref<SceneLoader> sceneLoader) {
		return sceneLoader->LoadFromPath("./resources/sponza.obj", {})->AddToScene(scene);
	});

	Vector2<float> zBounds(0.1f, 4500.f);
	Ref<PerspectiveCamera> camera = PerspectiveCamera::Create(scene, Angle<float>::Degree(60.f), static_cast<float>(windowSize.Width) / static_cast<float>(windowSize.Height), zBounds);
	Ref<FreeFlyCameraViewController> cameraController = FreeFlyCameraViewController::New(camera);
		
	float move = 2.f;
	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Q), [&](KeyboardEvent event) {
		cameraController->Translate(FreeFlyCameraViewController::TranslationAxis::Right, -move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::D), [&](KeyboardEvent event) {
		cameraController->Translate(FreeFlyCameraViewController::TranslationAxis::Right, move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Z), [&](KeyboardEvent event) {
		cameraController->Translate(FreeFlyCameraViewController::TranslationAxis::Front, -move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::S), [&](KeyboardEvent event) {
		cameraController->Translate(FreeFlyCameraViewController::TranslationAxis::Front, move);
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Add), [&](KeyboardEvent event) {
		move *= 2.f;
	});

	application->Connect(Keyboard::OnKeyPressed(KeyboardKey::Subtract), [&](KeyboardEvent event) {
		move /= 2.f;
	});

	application->Connect(Mouse::OnMouseMovedEvent(), [&](MouseMovedEvent event) {
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

	application->Connect(window->OnResizedEvent(), [&](WindowResizedEvent event) {
		windowSize = event.Size;
		camera->SetAspectRatio((float)windowSize.Width / (float)windowSize.Height);
		viewport->SetArea(Rectangle(Vector2<unsigned int>(0, 0), Vector2(windowSize.Width, windowSize.Height)));
	});

	while (window->PollEvents())
	{
		cameraController->Update();

		viewer->Render();

		graphicEngine->Present();
	}

	return Main::ExitCode::Success;
}
