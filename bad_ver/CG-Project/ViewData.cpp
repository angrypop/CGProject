#include "ViewData.h"

std::map<std::string, std::shared_ptr<UniformDataBase>> UniformDataPool::_namePointerMap;

void glUniformProcess(const GLuint& location, const GLfloat& data)
{
	::glUniform1f(location, data);
}

void glUniformProcess(const GLuint& location, const glm::vec2& data)
{
	::glUniform2fv(location, 1, glm::value_ptr(data));
}

void glUniformProcess(const GLuint& location, const glm::vec3& data)
{
	::glUniform3fv(location, 1, glm::value_ptr(data));
}

void glUniformProcess(const GLuint& location, const glm::vec4& data)
{
	::glUniform4fv(location, 1, glm::value_ptr(data));
}

void glUniformProcess(const GLuint& location, const GLint& data)
{
	::glUniform1i(location, data);
}

void glUniformProcess(const GLuint& location, const glm::mat4& data)
{
	::glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}


