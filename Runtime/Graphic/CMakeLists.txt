project(Graphic)

cocktail_source_directory(GRAPHIC_FILES    
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Export.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/SceneGraph.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/SceneNode.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/SceneGraph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/SceneNode.cpp
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
    FOLDER "Material/Shading"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Shading/MaterialProgram.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Material/Shading/VertexInterface.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/MaterialProgram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Material/Shading/VertexInterface.cpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Renderable.hpp
    SRC
)

cocktail_source_directory(GRAPHIC_FILES  
    FOLDER "Rendering/Queue"  
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Graphic/Rendering/Queue/RenderRecord.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/StaticMeshRecord.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/RecordDrawContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Graphic/Rendering/Queue/RenderQueue.cpp
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

cocktail_add_runtime_library(
    NAME Graphic
    DEPENDS Core Renderer
    SOURCES ${GRAPHIC_FILES}
)