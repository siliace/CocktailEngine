#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Keyboard/Keyboard.hpp>
#include <Cocktail/Core/System/Mouse/Mouse.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>

#include <Cocktail/Graphic/Mesh/MeshFactory.hpp>
#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Scene/Camera/Controller/FreeFlyCameraController.hpp>
#include <Cocktail/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>
#include <Cocktail/Graphic/Scene/Light/PointLight.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Shape/StaticMeshShape.hpp>

#include <Cocktail/Main/ExitCode.hpp>

using namespace Ck;

Main::ExitCode ApplicationMain(Application* application)
{
	Extent2D windowSize = MakeExtent(800u, 600u);
	std::shared_ptr<Window> window = application->Invoke([&](WindowFactory* windowFactory)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Size = windowSize;
		windowCreateInfo.Title = CK_TEXT("Frustum Culling");
		windowCreateInfo.Style = Flags<WindowStyle>::All();
		windowCreateInfo.Position = MakeExtent(100, 100);

		return windowFactory->CreateWindow(windowCreateInfo);
	});

	std::shared_ptr<GraphicEngine> graphicEngine = std::make_shared<GraphicEngine>(Renderer::GraphicApi::Vulkan);
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(graphicEngine);

	std::shared_ptr<Mesh> cubeMesh = MeshFactory::CreateCube(1.f, LinearColor::White);
	std::shared_ptr<Material> material = std::make_shared<Material>("default-cube", Material::ShadingMode::Unlit, true);
	material->SetEmissiveColor(LinearColor::White);
	std::shared_ptr<Shape> shape = std::make_shared<StaticMeshShape>(*graphicEngine, std::move(cubeMesh), Array<std::shared_ptr<Material>>{ std::move(material) });

	for (unsigned int i = 0; i < 30; i++)
	{
		for (unsigned int j = 0; j < 30; j++)
		{
			std::shared_ptr<SceneNode> sceneNode = scene->CreateSceneNode();
			sceneNode->SetPosition(Vector3<float>((i - 15.f) * 2.f, (j - 15.f) * 2.f, 0.f));
			sceneNode->AddShape(shape);
		}
	}

	float aspectRatio = static_cast<float>(windowSize.Width) / static_cast<float>(windowSize.Height);
	Vector2<float> zBounds(0.1f, 1000.f);

	PerspectiveCamera* camera = PerspectiveCamera::Create(scene, Angle<float>::Degree(45.f), aspectRatio, zBounds);
	camera->SetPosition(Vector3<float>(0.f, 0.f, 10.f));
	
    FreeFlyCameraViewController cameraController(camera);

    float move = 1.f;
    float rightTranslation = 0.f;
    float frontTranslation = 0.f;
    application->Connect(Keyboard::OnKey(KeyboardKey::Q), [&](KeyboardEvent event)
    {
        rightTranslation = event.Pressed ? -move : 0.f;
    });

    application->Connect(Keyboard::OnKey(KeyboardKey::D), [&](KeyboardEvent event)
    {
        rightTranslation = event.Pressed ? move : 0.f;
    });

    application->Connect(Keyboard::OnKey(KeyboardKey::Z), [&](KeyboardEvent event)
    {
        frontTranslation = event.Pressed ? -move : 0.f;
    });

    application->Connect(Keyboard::OnKey(KeyboardKey::S), [&](KeyboardEvent event)
    {
        frontTranslation = event.Pressed ? move : 0.f;
    });

    application->Connect(Keyboard::OnKey(KeyboardKey::Add), [&](KeyboardEvent event)
    {
        move *= 2.f;
    });

    application->Connect(Keyboard::OnKey(KeyboardKey::Subtract), [&](KeyboardEvent event)
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

	    cameraController.Translate(FreeFlyCameraViewController::TranslationAxis::Right, rightTranslation);
	    cameraController.Translate(FreeFlyCameraViewController::TranslationAxis::Front, frontTranslation);
		cameraController.Update(Duration::Between(lastFrameBegin, frameBegin));

		viewer->Render();

		graphicEngine->Present();
		lastFrameBegin = frameBegin;
	}

	return Main::ExitCode::Success;
}
