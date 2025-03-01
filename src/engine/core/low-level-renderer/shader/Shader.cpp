
#include "engine/core/low-level-renderer/graphic-api/Gapi.h"
#include "engine/core/low-level-renderer/shader/Shader.h"

#include "engine/core/low-level-renderer/shader/ShaderLoader.h"

using namespace GLESC::GAPI;

inline std::vector<std::string> macrosToString(const std::vector<Shader::ShaderMacros>& macros) {
    std::vector<std::string> result;
    for (auto macro : macros) {
        switch (macro) {
        case Shader::USE_COLOR:
            result.emplace_back("USE_COLOR");
            break;
        case Shader::USE_INSTANCE:
            result.emplace_back("USE_INSTANCE");
            break;
        }
    }
    return result;
}


Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource,
               const std::vector<ShaderMacros>& macros) :
    shaderProgram(ShaderLoader::loadShader(vertexShaderSource, fragmentShaderSource, macrosToString(macros))) {
}


Shader::Shader(const std::string& fileName, const std::vector<ShaderMacros>& macros) :
    shaderProgram(ShaderLoader::loadShader(fileName, macrosToString(macros))) {
}

Shader::~Shader() {
    getGAPI().deleteShaderProgram(shaderProgram);
}

void Shader::bind() const {
    getGAPI().useShaderProgram(shaderProgram);
}

void Shader::unbind() const {
    getGAPI().useShaderProgram(0);
}
