#pragma once
#include <iostream>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set Uniforms
	void SetUniform4f(const std::string& name, float, float, float, float);
	void SetUniform3f(const std::string& name, float, float, float);
	void SetUniform1i(const std::string& name, int val);
	void SetUniform1f(const std::string& name, float val);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat4f(const std::string& name, const float* matrix);

private :
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);
};