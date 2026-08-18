#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/System/Keyboard/Keyboard.hpp>
#include <CocktailEngine/Core/System/Mouse/Mouse.hpp>
#include <CocktailEngine/Core/System/Window/WindowFactory.hpp>

#include <CocktailEngine/Graphic/Mesh/MeshFactory.hpp>
#include <CocktailEngine/Graphic/Rendering/WindowSceneViewer.hpp>
#include <CocktailEngine/Graphic/Scene/Camera/Controller/FreeFlyCameraController.hpp>
#include <CocktailEngine/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <CocktailEngine/Graphic/Scene/Light/DirectionalLight.hpp>
#include <CocktailEngine/Graphic/Scene/Light/PointLight.hpp>
#include <CocktailEngine/Graphic/Scene/Scene.hpp>
#include <CocktailEngine/Graphic/Scene/Shape/StaticMeshShape.hpp>

#include <CocktailEngine/Main/ExitCode.hpp>

#include <ImGui/ImGuiIntegration/SceneMenuModule.hpp>
#include <ImGui/ImGuiIntegration/SystemMenuModule.hpp>
#include <ImGui/ImGuiIntegration/ImUi/Menu/MenuBarManager.hpp>
#include <ImGui/ImGuiIntegration/ImUi/WindowContainer.hpp>

using namespace Ck;

Main::ExitCode ApplicationMain(Application* application)
{
	Extent2D windowSize = MakeExtent(800u, 600u);
	UniquePtr<Window> window = application->Invoke([&](WindowFactory* windowFactory)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Size = windowSize;
		windowCreateInfo.Title = CK_TEXT("Frustum Culling");
		windowCreateInfo.Style = Flags<WindowStyle>::All();
		windowCreateInfo.Position = MakeExtent(100, 100);

		return windowFactory->CreateWindow(windowCreateInfo);
	});

	SharedPtr<GraphicEngine> graphicEngine = MakeShared<GraphicEngine>(Renderer::GraphicApi::Vulkan);
	SharedPtr<Scene> scene = MakeShared<Scene>(graphicEngine);

	SharedPtr<Mesh> cubeMesh = MeshFactory::CreateCube(1.f, LinearColor::White);
	SharedPtr<Material> material = MakeShared<Material>("default-cube", Material::ShadingMode::Unlit, true);
	material->SetEmissiveColor(LinearColor::White);
    SharedPtr<Shape> shape = MakeShared<StaticMeshShape>(*graphicEngine, Move(cubeMesh), Array<SharedPtr<Material>>{ Move(material) });

    for (unsigned int i = 0; i < 30; i++)
    {
        for (unsigned int j = 0; j < 30; j++)
        {
            SceneNode* sceneNode = scene->CreateSceneNode();
            sceneNode->SetPosition(Vector3<float>((i - 15.f) * 2.f, (j - 15.f) * 2.f, 0.f));
            sceneNode->AddShape(shape);
        }
    }

	float aspectRatio = static_cast<float>(windowSize.Width) / static_cast<float>(windowSize.Height);
	Vector2<float> zBounds(0.1f, 1000.f);
    Rectangle<float> viewportArea(0.f, 0.f, 1.f, 1.f);
	PerspectiveCamera* camera = PerspectiveCamera::Create(scene, CK_TEXT("MainCamera"), Angle<float>::Degree(45.f), aspectRatio, zBounds);
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
	SharedPtr<SceneViewer> viewer = MakeShared<WindowSceneViewer>(scene, window.Get(), viewerParameters, true);

    UniquePtr<SceneView> sceneView = MakeUnique<SceneView>(scene.Get(), camera);
	UniquePtr<Viewport> viewport = MakeUnique<Viewport>(Move(sceneView), viewportArea);
	viewer->AttachViewport(Move(viewport));

	application->Connect(window->OnResizedEvent(), [&](WindowResizedEvent event)
	{
	    camera->SetAspectRatio(static_cast<float>(event.Size.Width) / static_cast<float>(event.Size.Height));
	});

    ImUi::WindowContainer windowContainer(window.Get(), graphicEngine->GetRenderDevice());
    application->Connect(viewer->OnRendered(), [&](Renderer::RenderContext& renderContext, Renderer::Framebuffer& framebuffer) {
       windowContainer.Render(&renderContext, &framebuffer, Duration::Milliseconds(16));
    });

    ImUi::MenuBarManager* menuBarManager = windowContainer.GetMenuBarManager();
    menuBarManager->CreateMenu<SceneMenuModule>(scene.Get());
    menuBarManager->CreateMenu<SystemMenuModule>();

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
