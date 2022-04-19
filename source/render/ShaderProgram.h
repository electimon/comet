#pragma once

#include <comet.pch>
#include <glad/gl.h>

class ShaderProgram
{
public:
  ShaderProgram();
  ShaderProgram(const char *vertFile, const char *fragFile);
  ~ShaderProgram();

  void Bind();
  void Unbind();

  void Create(const char *vertFile, const char *fragFile);
  void Delete();

  unsigned int GetID();

  int GetUniformLocation(const std::string &name);

private:
  unsigned int m_ID;
  std::unordered_map<std::string, int> m_UniformMap;
};