#pragma once
#include "ViewBase.h"

// object with SINGLE VBO
class ViewObject : public ViewBase
{
// pure virtual function zone
public:
	// update uniform data in GPU
	virtual void UpdateData(const GLuint & programHandle) = 0;

// non-pure virutal function zone
public:


// other function zone
public:
	// bind this object to VAO in given module, return size of VBO
	virtual void Render(const ViewModule& module) final;
	// rotate this object around certain axis(x, y, z) and angle degree
	void Rotate(const GLfloat& angle, const glm::vec3& axis = glm::vec3(0.0f, 0.0f, 1.0f));
	// translate this object by displacement vector
	void Translate(const glm::vec3& displacement);
	// scale this object with (x, y, z)
	void Scale(const glm::vec3& scaler);
	// get M(model matrix) of this object
	glm::mat4 GetM() const;
	// set model matrix of this object
	void SetM(const glm::mat4& M);

	// add uniform data
	void AddUniformData(std::shared_ptr<UniformDataBase>&& uniformData);
	// get uniform data by name, NEVER FREE this pointer
	UniformDataBase* GetUniformData(std::string && name) const;

	// add buffer data
	void AddBufferData(std::shared_ptr<BufferDataBase>&& bufferData);
	// get buffer data by index, NEVER FREE this pointer
	BufferDataBase* GetBufferData(const GLuint & position) const;

	// add texture buffer
	void AddTexture(const std::string & name);

	// bind this object to its group
	void Bind(const std::weak_ptr<ViewGroup>& groupPtr);
	// get its parent group pointer
	const std::weak_ptr<ViewGroup>& Parent() const;

	// hide this object
	void Hide();
	// show this object
	void Show();
	// get whether it is hidden
	bool IsHide() const;

	// set its object type
	void SetType(const ViewObjectEnum& type);
	// get its object type
	ViewObjectEnum GetType() const;
	// get number of its points
	GLsizei GetPointCount() const;
	// get its buffer data vector by index
	template <typename T>
	std::vector<T>& GetBufferData(const int& index = 0) const
	{
		if (index >= this->_bufferData.size() || index < 0)
			throw("ViewObject::GetBufferData:: Index OUT of range");
		BufferDataVector<T>* ptr = dynamic_cast<BufferDataVector<T>*>(this->_bufferData[index].get());
		if (ptr == nullptr)
			throw("ViewObject::GetBufferData:: Bad Index");
		return ptr->Data();
	}
	// get its buffer data array by index
	template <typename T, GLsizei N, GLsizei M>
	std::array<T, N * M>& GetBufferData(const int& index = 0) const
	{
		if (index >= this->_bufferData.size() || index < 0)
			throw("ViewObject::GetBufferData:: Index OUT of range");
		BufferData<T, N, M>* ptr = dynamic_cast<BufferData<T, N, M>*>(this->_bufferData[index].get());
		if (ptr == nullptr)
			throw("ViewObject::GetBufferData:: Bad Index");
		return ptr->Data();
	}

	// compare with its handle 
	bool operator == (const ViewObject& rhs);
	// compare with its handle
	bool operator < (const ViewObject& rhs);
	// compare with its handle
	bool operator > (const ViewObject& rhs);

	ViewObject(const ViewObjectEnum& type, const GLsizei & verNum)
		: _type(type), _verNum(verNum)
	{}

	virtual ~ViewObject();

// data zone
protected:
	ViewObjectEnum _type;
	std::weak_ptr<ViewGroup> _groupPtr;
	GLuint _VAO = 0;
	GLsizei _verNum;
	bool _showFlag = true;

	glm::mat4 _M = glm::mat4(1.0f);	// the model matrix

	std::vector<std::shared_ptr<UniformDataBase>> _uniformData;
	std::vector<std::shared_ptr<BufferDataBase>> _bufferData;
	std::vector<std::shared_ptr<ViewTexture>> _textures;
};


class ViewTriangle : public ViewObject
{
public:
	ViewTriangle(std::array<GLfloat, POINTSIZE * 3>&& vertices);
	virtual void UpdateData(const GLuint& programHandle);
	void InitData(std::array<GLfloat, POINTSIZE * 3>&& vertices);

	virtual ~ViewTriangle() {}
protected:

	//GLfloat _transAngle = 45.0f;
};

class ViewPolygon : public ViewObject
{
public:
	// sizeof vertices must be the multiple of (3 * POINTSIZE)
	ViewPolygon();
	ViewPolygon(const std::vector<GLfloat>& vertices);
	virtual void UpdateData(const GLuint& programHandle);
	void InitData(const std::vector<GLfloat>& vertices);

	virtual ~ViewPolygon() {}
};

class ViewRectangle : public ViewPolygon
{
public:
	ViewRectangle(const GLfloat& width = 1.0f, const GLfloat& height = 1.0f);
};
