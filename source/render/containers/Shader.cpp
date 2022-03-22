#include "Shader.h"

Shader::Shader(char *vertFile, char *fragFile)
{
    m_ID = glCreateProgram();

    unsigned int vertexID, fragmentID;
    int success;
    char infoLog[512];

    // Vertex Shader
    {
        std::ifstream file(vertFile);
        std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        const char *contentsChar = contents.c_str();
        vertexID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexID, 1, &contentsChar, NULL);
        glCompileShader(vertexID);
    }

    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    {
        std::ifstream file(fragFile);
        std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        const char *contentsChar = contents.c_str();
        fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentID, 1, &contentsChar, NULL);
        glCompileShader(fragmentID);
    }

    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(m_ID, vertexID);
    glAttachShader(m_ID, fragmentID);
    glLinkProgram(m_ID);

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

Shader::~Shader()
{
    std::cout << "Shader::~Shader()" << std::endl;

    std::cout << "Deleting shader (id: " << m_ID << ")" << std::endl;
    glDeleteProgram(m_ID);
}

void Shader::Bind()
{
    glUseProgram(m_ID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

unsigned int Shader::GetID()
{
    return m_ID;
}

int Shader::GetUniformLocation(const std::string &name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_ID, name.c_str());

    if (location == -1)
    {
        std::cout << "Uniform " << name << " not found. Ignoring..." << std::endl;
    }

    m_UniformLocationCache[name] = location;

    return location;
}