#pragma once
#include "Base.h"





class ViewObject
{

public:

	ViewObject();
	virtual ~ViewObject();

	void Bind(std::shared_ptr<ViewGroup> groupPtr)
	{
		this->_groupPtr = groupPtr;
	}

	const std::weak_ptr<ViewGroup> & Parent() const
	{
		return this->_groupPtr;
	}

	const std::shared_ptr<GLfloat> & GetPos() const
	{
		return this->_vertex;
	}

	const std::shared_ptr<GLfloat> & GetIndice() const
	{
		return this->_indice;
	}

	void Hide()
	{
		this->_showFlag = false;
	}
	void Show()
	{
		this->_showFlag = true;
	}

	ViewObjectEnum GetType() const
	{
		return this->_type;
	}
protected:
	ViewObjectEnum _type;
	std::weak_ptr<ViewGroup> _groupPtr;

	// data zone
	bool _showFlag = true;;
	std::shared_ptr<GLfloat> _vertex;
	std::shared_ptr<GLfloat> _indice;
};

class ViewPlane : public ViewObject
{
public:
	ViewPlane();
	virtual ~ViewPlane();
private:

};

class ViewBall : public ViewObject
{
public:
	ViewBall();
	virtual ~ViewBall();
private:

}; 