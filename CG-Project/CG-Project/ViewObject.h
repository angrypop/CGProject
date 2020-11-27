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
	// bind this object to VAO in given module, return size of VBO
	virtual void Render(const ViewModule& module);

// other function zone
public:
	// add uniform data
	void AddUniformData(std::shared_ptr<UniformDataBase>&& uniformData);
	// get uniform data by name, NEVER FREE this pointer
	UniformDataBase* GetUniformData(std::string && name) const;

	// add buffer data
	void AddBufferData(std::shared_ptr<BufferDataBase>&& bufferData);
	// get buffer data by index, NEVER FREE this pointer
	BufferDataBase* GetBufferData(const GLuint & position) const;

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

	std::vector<std::shared_ptr<UniformDataBase>> _uniformData;
	std::vector<std::shared_ptr<BufferDataBase>> _bufferData;
};


class ViewTriangle : public ViewObject
{
public:
	ViewTriangle(std::array<GLfloat, POINTSIZE * 3>&& vertices);
	virtual void UpdateData(const GLuint& programHandle);
	void InitData(std::array<GLfloat, POINTSIZE * 3>&& vertices);

	virtual ~ViewTriangle() {}
protected:
	GLfloat _transAngle = 45.0f;
};

class ViewPlane : public ViewObject
{
public:
	ViewPlane();
	virtual ~ViewPlane();
protected:

};

class ViewBall : public ViewObject
{
public:
	ViewBall();
	virtual ~ViewBall();
protected:

}; 


