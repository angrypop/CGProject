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
	void SetData(T&& data)
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
	virtual void SetData(GLsizei&& pos, T&& data)
	{
		this->_data[pos] = data;
	}
	// set data[posX, posY]
	virtual void SetData(GLsizei&& posX, GLsizei&& posY, T&& data)
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

class GlobalUniformDataPool
{
public:
	// get global uniform data pointer by its name
	static std::shared_ptr<UniformDataBase> Get(const std::string& name);
	// add a global uniform data with name(alternative)
	static void Add(const std::shared_ptr<UniformDataBase>& data);
	// delete a global uniform data by its name
	static void Delete(const std::string& name);
protected:
	static std::map<std::string, std::shared_ptr<UniformDataBase>> _namePointerMap;
};