#include "Shader.hpp"
#include <iostream>

namespace OGL {

    Shader::Shader
    ( char const *vertexSourcePath
    , char const *fragmentSourcePath
    ) {
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;
        std::string vertexShaderCode;
        std::string fragmentShaderCode;

        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vertexShaderFile.open(vertexSourcePath);
            fragmentShaderFile.open(fragmentSourcePath);

            std::stringstream ssVertexCode, ssFragmentCode;
            ssVertexCode << vertexShaderFile.rdbuf();
            ssFragmentCode << fragmentShaderFile.rdbuf();

            vertexShaderCode = ssVertexCode.str();
            fragmentShaderCode = ssFragmentCode.str();
        }
        catch (std::ifstream::failure e) {
            throw Exception("Error reading shader code from file!");
        }

        unsigned int vertexShaderID = compileGLShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
        if (!shaderCorrectlyCompiled(vertexShaderID)) {
            reportShaderCompileError(vertexShaderID);
        }

        unsigned int fragmentShaderID = compileGLShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);
        if (!shaderCorrectlyCompiled(fragmentShaderID)) {
            reportShaderCompileError(fragmentShaderID);
        }

        m_programmID = glCreateProgram();
        glAttachShader(m_programmID, vertexShaderID);
        glAttachShader(m_programmID, fragmentShaderID);
        glLinkProgram(m_programmID);
        if (!programmCorrectlyLinked(m_programmID)) {
            reportProgrammLinkError(m_programmID);
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    Shader::~Shader
    (
    ) {
        glDeleteProgram(m_programmID);
    }

    unsigned int Shader::id
    (
    ) const {
        return m_programmID;
    }

    void Shader::use 
    (
    ) {
        glUseProgram(m_programmID);
    }

    void Shader::setUniformBool
    ( std::string const &name
    , bool val
    ) {
        glUniform1i(glGetUniformLocation(m_programmID, name.c_str()), (int)val);
    }

    void Shader::setUniformInt
    ( std::string const &name
    , int val
    ) {
        glUniform1i(glGetUniformLocation(m_programmID, name.c_str()), val);
    }

    void Shader::setUniformFloat
    ( std::string const &name
    , float val
    ) {
        glUniform1f(glGetUniformLocation(m_programmID, name.c_str()), val);
    }

    void Shader::setUniformMatrix4
    ( std::string const &name
    , glm::mat4 const &matrix
    , bool doTranspose
    ) {
        unsigned int matrLoc = glGetUniformLocation(m_programmID, name.c_str());
        glUniformMatrix4fv(matrLoc, 1, doTranspose, glm::value_ptr(matrix));
    }

    void Shader::setUniformVec3
    ( std::string const &name
    , glm::vec3 const &vec
    ) {
        unsigned int vecLocation = glGetUniformLocation(m_programmID, name.c_str());
        glUniform3fv(vecLocation, 1, &vec[0]);
    }

    unsigned int Shader::compileGLShader
    ( char const *sourceCode
    , int shaderType
    ) {
        unsigned int shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &sourceCode, nullptr);
        glCompileShader(shaderId);
        return shaderId;
    }

    bool Shader::shaderCorrectlyCompiled
    ( unsigned int shaderId
    ) {
        int correctness;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &correctness);
        return correctness == GL_TRUE;
    }

    void Shader::reportShaderCompileError
    ( unsigned int shaderId
    ) {
        std::string infoLog(512, ' ');
        glGetShaderInfoLog(shaderId, infoLog.size(), nullptr, infoLog.data());
        throw Exception(infoLog);
    }

    bool Shader::programmCorrectlyLinked
    ( unsigned int programmId
    ) {
        int correctness;
        glGetProgramiv(programmId, GL_LINK_STATUS, &correctness);
        return correctness == GL_TRUE;
    }

    void Shader::reportProgrammLinkError
    ( unsigned int programmId
    ) {
        std::string infoLog(512, ' ');
        glGetProgramInfoLog(programmId, infoLog.size(), nullptr, infoLog.data());
        throw Exception(infoLog);
    }

} // OGL