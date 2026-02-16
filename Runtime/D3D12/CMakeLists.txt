project(D3D12)

cocktail_source_directory(D3D12_FILES
        INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/D3D12/D3D12.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/D3D12/Export.hpp
        SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/New.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/D3D12.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/RenderDevice.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/RenderDevice.cpp
)

cocktail_source_directory(D3D12_FILES
        FOLDER "Buffer"
        SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Buffer/Buffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Buffer/Buffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Buffer/BufferView.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Buffer/BufferView.cpp
)

cocktail_source_directory(D3D12_FILES
        FOLDER "Command"
        SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Command/CommandList.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Command/CommandList.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Command/Fence.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Command/Fence.cpp
)

cocktail_source_directory(D3D12_FILES
        FOLDER "Context"
        SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Context/RenderContext.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Context/RenderContext.cpp
)

cocktail_source_directory(D3D12_FILES
        FOLDER "Texture"
        SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Texture/Texture.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Texture/Texture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Texture/Sampler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/D3D12/Texture/Sampler.cpp
)

cocktail_add_runtime_library(
        NAME
        D3D12
        DEPENDS
        Core
        Renderer
        LINK
        d3d12
        dxgi
        dxguid
        SOURCES
        ${D3D12_FILES}
)
