#ifndef COCKTAIL_SHADERCOMPILER_PROJECT_HPP
#define COCKTAIL_SHADERCOMPILER_PROJECT_HPP

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/ShaderCompiler/Compiler.hpp>

namespace Ck::Tools::ShaderCompiler
{
    class Project
    {
    public:

        class SourceFile
        {
        public:

            static SourceFile LoadFromPath(std::filesystem::path path, Renderer::ShaderType stage, Language language);

            SourceFile(std::string name, std::string content, std::filesystem::path location, Renderer::ShaderType stage, Language language);

            Ref<ShaderObject> Compile(Compiler& compiler) const;

        private:

            std::string mName;
            std::string mContent;
            std::filesystem::path mLocation;
            Renderer::ShaderType mStage;
            Language mLanguage;
        };

        static Project LoadFromPath(const std::filesystem::path& path);

        explicit Project(Renderer::ShaderProgramType programType);

        void AddSourceFile(const SourceFile& sourceFile);

        EnumMap<Renderer::ShaderType, std::vector<Uint32>> Build(Compiler& compiler);

    private:

        Renderer::ShaderProgramType mProgramType;
        std::vector<SourceFile> mSourceFiles;
    };
}

#endif // COCKTAIL_SHADERCOMPILER_PROJECT_HPP
