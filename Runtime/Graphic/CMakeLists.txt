project(Graphic)

cocktail_source_directory(GRAPHIC_FILES    
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Export.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/GraphicServiceProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/GraphicServiceProvider.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Camera"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Camera/Camera.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Camera/OrhtographicCamera.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Camera/PerspectiveCamera.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Camera/Camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Camera/OrhtographicCamera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Camera/PerspectiveCamera.cpp
)

cocktail_source_directory(GRAPHIC_FILES
    FOLDER "Camera/Controller"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Camera/Controller/CameraController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Camera/Controller/FreeFlyCameraController.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Camera/Controller/FreeFlyCameraController.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Geometry"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Geometry/GeometryBuffer.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Geometry/GeometryBuffer.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Geometry/Index"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Geometry/Index/IndexArray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Geometry/Index/IndexRef.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Geometry/Index/IndexArray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Geometry/Index/IndexRef.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Geometry/Vertex"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Geometry/Vertex/VertexAttribute.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Geometry/Vertex/VertexAttributeSemantic.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Geometry/Vertex/VertexRef.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Geometry/Vertex/VertexArray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Geometry/Vertex/VertexAttribute.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Geometry/Vertex/VertexLayout.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Geometry/Vertex/VertexRef.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Material"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Material.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Material.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Material/MipMaps"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/MipMaps/CubeMipMaps.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/MipMaps/MipMapLevel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/MipMaps/MipMaps.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/MipMaps/MipMapsLoader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/MipMaps/MipMapsServiceProvider.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/CubeMipMaps.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/MipMapLevel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/MipMaps.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/MipMapsLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/MipMapsServiceProvider.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Material/MipMaps/DDS"  
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/DDS/DDSHeader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/DDS/DDSHeaderDxt10.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/DDS/DDSImporter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/DDS/DDSImporter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/MipMaps/DDS/DDSPixelFormat.hpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Material/Shading"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Shading/MaterialProgram.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Shading/MaterialProgramCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Shading/MaterialProgramManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Shading/MaterialProgramSet.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Shading/MaterialProgramVariant.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialLibraryServiceProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialLibraryServiceProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialProgram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialProgramManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialProgramSet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialProgramVariant.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Material/Texture"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Texture/TextureSampler.hpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Mesh"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Mesh/Mesh.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Mesh/Mesh.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Rendering"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/HeadlessSceneViewer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Renderable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/RenderableType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/SceneViewer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Viewport.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/WindowSceneViewer.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/HeadlessSceneViewer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/SceneViewer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Viewport.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/WindowSceneViewer.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Rendering/Engine"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Engine/ResourceUploader.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Engine/GraphicEngine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Engine/ResourceUploader.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Rendering/Queue"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Queue/CustomRecordInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Queue/StaticMeshRecordInfo.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/CustomRecord.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/CustomRecord.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/RecordDrawContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/RenderQueue.cpp
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Rendering/Resource"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Resource/BufferResource.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Resource/GraphicResource.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Resource/IndexBuffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Resource/TextureResource.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Resource/VertexBuffer.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Resource/BufferResource.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Resource/IndexBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Resource/TextureResource.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Resource/VertexBuffer.cpp
)

cocktail_source_directory(GRAPHIC_FILES 
    FOLDER "Scene"     
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Scene/Scene.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Scene/SceneContainer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Scene/SceneGraph.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Scene/SceneNode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Scene/SceneLoader.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Scene.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/SceneContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/SceneGraph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/SceneLoaderServiceProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/SceneLoaderServiceProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/SceneNode.cpp
)

cocktail_source_directory(GRAPHIC_FILES 
    FOLDER "Scene/Gltf"     
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Gltf/GltfImporter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Gltf/GltfImporter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Gltf/GltfSceneContainer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Gltf/GltfSceneContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Gltf/TinyGltf.cpp
)

cocktail_source_directory(GRAPHIC_FILES 
    FOLDER "Scene/Obj"     
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Obj/ObjImporter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Obj/ObjImporter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Obj/ObjSceneContainer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Obj/ObjSceneContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Obj/ObjVertex.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Obj/ObjVertexCache.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Obj/ObjVertexCache.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Scene/Obj/TinyObjLoader.cpp
)

cocktail_source_directory(GRAPHIC_FILES
    FOLDER "Spatial"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Spatial/Transformable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Spatial/Transformation.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Spatial/TransformationGraph.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Spatial/TransformationNode.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Spatial/Transformable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Spatial/Transformation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Spatial/TransformationGraph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Spatial/TransformationNode.cpp
)

cocktail_add_shader(GRAPHIC_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/colored.vert.spv
    TYPE vert
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_COLOR
)

cocktail_add_shader(GRAPHIC_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/colored.frag.spv
    TYPE frag
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_COLOR
)

cocktail_add_shader(GRAPHIC_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/default.vert.spv
    TYPE vert
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/basic_mesh.vert
)

cocktail_add_shader(GRAPHIC_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/default.frag.spv
    TYPE frag
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/basic_mesh.frag
)

cocktail_add_shader(GRAPHIC_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/textured.vert.spv
    TYPE vert
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
)

cocktail_add_shader(GRAPHIC_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/textured.frag.spv
    TYPE frag
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
)

cocktail_resource_directory(GRAPHIC_FILES
    FOLDER "shaders/mesh"
    RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/basic_mesh.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/basic_mesh.vert
)

cocktail_add_runtime_library(
    NAME 
        Graphic
    DEPENDS 
        Core 
        Renderer
    LINK 
        tinyobjloader 
        tinygltf
    DEFINES
        TINYGLTF_NO_STB_IMAGE
        TINYGLTF_NO_STB_IMAGE_WRITE
    SOURCES 
        ${GRAPHIC_FILES}
    RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/colored.frag.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/colored.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/default.frag.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/default.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/textured.frag.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/mesh/textured.vert.spv
)