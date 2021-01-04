#pragma once
#include "ViewBase.h"

void glUniformProcess(const GLuint& location, const GLfloat& data);

void glUniformProcess(const GLuint& location, const glm::vec2& data);

void glUniformProcess(const GLuint& location, const glm::vec3& data);

void glUniformProcess(const GLuint& location, const glm::vec4& data);

void glUniformProcess(const GLuint& location, const GLint& data);

void glUniformProcess(const GLuint& location, const glm::mat4& data);


// base class of UniformData
class UniformDataBase
{
public:
	// transfer data to GPU in given program
	virtual void Transfer(const GLuint& programHandle) const = 0;
	// set data name
	void SetName(const std::string& name)
	{
		_name = name;
	}
	// get data name
	std::string GetName() const
	{
		return this->_name;
	}
	UniformDataBase(const std::string& name)
		: _name(name) {}

protected:
	std::string _name;
};

// data type: T
template <typename T>
class UniformData : public UniformDataBase
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

	// transfer data to GPU in given program
	virtual void Transfer(const GLuint& programHandle) const
	{
		GLuint location = glGetUniformLocation(programHandle, _name.c_str());
		::glUniformProcess(location, _data);
	}

	UniformData(const std::string& name, const T& data)
		:UniformDataBase(name), _data(data)
	{}

protected:
	T _data;
};


// base class of BufferData
class BufferDataBase
{
	// pure virtual function zone
public:
	// bind this data to given VAO
	virtual void BindVAO(const GLuint& VAO) = 0;
	BufferDataBase(const std::vector<GLsizei>& length)
		: _lengths(length)
	{}
protected:
	std::vector<GLsizei> _lengths;
};

// data type: T, point number: N, each point size: M
template <typename T, GLsizei N, GLsizei M>
class BufferData : public BufferDataBase
{
public:
	// return reference of data to be operated
	std::array<T, N * M>& Data()
	{
		return this->_data;
	}
	// bind this data to given VAO
	virtual void BindVAO(const GLuint& VAO)
	{
		::glBindVertexArray(VAO);
		::glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		GLuint position = 0;
		GLsizei offset = 0;
		auto it = _lengths.begin();
		for (; it != _lengths.end(); position++, offset += *it, it++)
		{
			::glVertexAttribPointer(position, *it, GL_FLOAT, GL_FALSE, M * sizeof(T), (void*)(((T*)0) + offset));
			//			std::cout << M * sizeof(T) << " " << (int)(void*)(((T*)0) + offset) << std::endl;
			::glEnableVertexAttribArray(position);
		}
		// the _lengths is not correct
		if (offset != M)
			throw("Buffer Data Lengths NOT Correct!");

		::glBindVertexArray(0);
		::glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	// set data[pos]
	virtual void SetData(const GLsizei& pos, const T& data)
	{
		this->_data[pos] = data;
	}
	// set data[posX, posY]
	virtual void SetData(const GLsizei& posX, const GLsizei& posY, const T& data)
	{
		this->_data[(long long)(posX)*M + posY] = data;
	}

	// data: the data array, length: the vector of lengths of every location data
	// e.g. for length {3, 4}, means location = 0 has 3 floats, location = 1 has 4 floats
	BufferData(const std::array<T, N* M>& data, const std::vector<GLsizei>& lengths)
		: BufferDataBase(lengths), _data(data)
	{
		glCreateBuffers(1, &_vbo);
		glNamedBufferStorage(_vbo, N * M * sizeof(T), _data.data(), 0);
	}

protected:

	GLuint _vbo = 0;
	std::array<T, N* M> _data;
};

// data type: T
template <typename T>
class BufferDataVector : public BufferDataBase
{
public:
	// return reference of data to be operated
	std::vector<T>& Data()
	{
		return this->_data;
	}
	// bind this data to given VAO
	virtual void BindVAO(const GLuint& VAO)
	{
		::glBindVertexArray(VAO);
		::glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		GLuint position = 0;
		GLsizei offset = 0;
		auto it = _lengths.begin();
		for (; it != _lengths.end(); position++, offset += *it, it++)
		{
			::glVertexAttribPointer(position, *it, GL_FLOAT, GL_FALSE, M * sizeof(T), (void*)(((T*)0) + offset));
			//			std::cout << M * sizeof(T) << " " << (int)(void*)(((T*)0) + offset) << std::endl;
			::glEnableVertexAttribArray(position);
		}
		// the _lengths is not correct
		if (offset != M)
			throw("Buffer Data Lengths NOT Correct!");

		::glBindVertexArray(0);
		::glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	// set data[pos]
	virtual void SetData(const GLsizei& pos, const T& data)
	{
		this->_data[pos] = data;
	}
	// set data[posX, posY]
	virtual void SetData(const GLsizei& posX, const GLsizei& posY, const T& data)
	{
		this->_data[(long long)(posX)*M + posY] = data;
	}

	// data: the data array, length: the vector of lengths of every location data
	// e.g. for length {3, 4}, means location = 0 has 3 floats, location = 1 has 4 floats
	BufferDataVector(const std::vector<T>& data, const GLsizei & M, const std::vector<GLsizei>& lengthInfo)
		: BufferDataBase(lengthInfo), _data(data), N((GLsizei)data.size() / M), M(M)
	{
		glCreateBuffers(1, &_vbo);
		glNamedBufferStorage(_vbo, (short)N * (short)M * sizeof(T), _data.data(), 0);
	}

protected:

	GLuint _vbo = 0;
	GLsizei N;
	GLsizei M;
	std::vector<T> _data;
};

class UniformDataPool
{
public:
	template <typename T>
	// get global uniform data pointer by its name
	static UniformData<T>* Get(const std::string& name)
	{
		auto target = _namePointerMap.find(name);
		if (target != _namePointerMap.end())
			return dynamic_cast<UniformData<T>*>(target->second.get());
		else
			throw(std::string("No Global Uniform Data Name = ") + name);
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
		return Get<T>(name)->SetData(data);
	}
	// add a global uniform data with name(alternative)
	template <typename T>
	static void Add(const std::string& name, const T& data)
	{
		std::shared_ptr<UniformData<T>> ptr(new UniformData<T>(name, data));
		_namePointerMap.insert(std::pair<std::string, std::shared_ptr<UniformDataBase>>(name, ptr));
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
	static std::map<std::string, std::shared_ptr<UniformDataBase>> _namePointerMap;
};
