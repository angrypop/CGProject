#include "ViewData.h"

std::map<std::string, std::shared_ptr<UniformDataBase>> GlobalUniformDataPool::_namePointerMap;

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

std::shared_ptr<UniformDataBase> GlobalUniformDataPool::Get(const std::string& name)
{
	auto target = _namePointerMap.find(name);
	if (target != _namePointerMap.end())
		return target->second;
	else
		throw(std::string("No Global Uniform Data Name = ") + name);
}

void GlobalUniformDataPool::Add(const std::shared_ptr<UniformDataBase>& data)
{
	std::shared_ptr<UniformDataBase> ptr = data;
	_namePointerMap.insert(std::pair<std::string, std::shared_ptr<UniformDataBase>>(data->GetName(), ptr));
}

void GlobalUniformDataPool::Delete(const std::string& name)
{
	auto target = _namePointerMap.find(name);
	if (target != _namePointerMap.end())
		_namePointerMap.erase(target);
	else
		throw(std::string("No Global Uniform Data Name = ") + name);
}
