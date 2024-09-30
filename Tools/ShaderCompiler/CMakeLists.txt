project(ShaderCompiler)

cocktail_source_directory(SHADER_COMPILER_FILES    
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/Compiler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/CompilerCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/CompilerFactory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/Language.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/Program.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/Project.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/ShaderBinaryFormat.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/ShaderCompilerServiceProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/ShaderObject.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/ShaderCompiler/TargetApi.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/CompilerFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Project.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/ShaderCompilerServiceProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/main.cpp
)

cocktail_source_directory(SHADER_COMPILER_FILES    
    FOLDER "Glslang"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/GlslangServiceProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/GlslangServiceProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/GlslangUtils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/GlslangUtils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/GlslShaderIncluder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/GlslShaderIncluder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/SpirvCompiler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/SpirvCompiler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/SpirvProgram.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/SpirvProgram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/SpirvShaderObject.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/ShaderCompiler/Glslang/SpirvShaderObject.cpp
)

cocktail_add_tool(
    NAME 
        shader-compiler
    DEPENDS 
        Core 
        Main
        Renderer 
    LINK 
        glslang SPIRV glslang-default-resource-limits
    SOURCES 
        ${SHADER_COMPILER_FILES}
)