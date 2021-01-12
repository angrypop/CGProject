#pragma once
#include "ViewHeader.h"

template <typename T>
void SendUniformData(const GLuint& program, const std::string& name);

// base class of UniformData
class GlobalDataBase
{
public:
	// set data name
	virtual void SetName(const std::string& name)
	{
		_name = name;
	}
	// get data name
	std::string GetName() const
	{
		return this->_name;
	}
	GlobalDataBase(const std::string& name)
		: _name(name) {}

protected:
	std::string _name;
};

// data type: T
template <typename T>
class GlobalData : public GlobalDataBase
{
public:
	// get data
	T GetData() const
	{
		return this->_data;
	}
	// set data
	void SetData(const T& data)
	{
		this->_data = data;
	}

	GlobalData(const std::string& name, const T& data)
		:GlobalDataBase(name), _data(data)
	{}

protected:
	T _data;
};

// pool of global data
class GlobalDataPool
{
public:
	template <typename T>
	// get global uniform data pointer by its name
	static GlobalData<T>* Get(const std::string& name)
	{
		auto target = _namePointerMap.find(name);
		if (target != _namePointerMap.end())
			return dynamic_cast<GlobalData<T>*>(target->second.get());
		else
			throw(std::string("No Global Data Name = ") + name);
	}
	// get global uniform data itself by its name
	template <typename T>
	static T GetData(const std::string& name)
	{
		return Get<T>(name)->GetData();
	}
	// get global uniform data itself by its name
	template <typename T>
	static void SetData(const std::string& name, const T& data)
	{
		Get<T>(name)->SetData(data);
	}
	// add a global uniform data with name(alternative)
	template <typename T>
	static void Add(const std::string& name, const T& data)
	{
		std::shared_ptr<GlobalData<T>> ptr(new GlobalData<T>(name, data));
		_namePointerMap.insert(std::pair<std::string, std::shared_ptr<GlobalDataBase>>(name, ptr));
	}
	// delete a global uniform data by its name
	static void Delete(const std::string& name)
	{
		auto target = _namePointerMap.find(name);
		if (target != _namePointerMap.end())
			_namePointerMap.erase(target);
		else
			throw(std::string("No Global Uniform Data Name = ") + name);
	}
protected:
	static std::map<std::string, std::shared_ptr<GlobalDataBase>> _namePointerMap;
};

void glUniformProcess(const GLuint& location, const GLfloat& data);

void glUniformProcess(const GLuint& location, const glm::vec2& data);

void glUniformProcess(const GLuint& location, const glm::vec3& data);

void glUniformProcess(const GLuint& location, const glm::vec4& data);

void glUniformProcess(const GLuint& location, const GLint& data);

void glUniformProcess(const GLuint& location, const glm::mat4& data);


template <typename T>
void SendUniformData(const GLuint & program, const std::string & name)
{
	GLuint location = glGetUniformLocation(program, name);
	glUniformProcess(location, GlobalDataPool::GetData(name));
}

