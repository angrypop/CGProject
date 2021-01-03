#pragma once
#include "ViewHeader.h"

class ViewGroup : public ViewBase, public std::enable_shared_from_this<ViewGroup>
{
// pure virtual function zone
public:
	// initial group
	virtual void Init() = 0;

// non-pure virtual function zone
public:

// other function zone
public:
	// render this group
	virtual void Render() final;
	// return the handle id of object
	int AddObject(const std::shared_ptr<ViewObject>& obj);
	// return object pointer
	std::shared_ptr<ViewObject> GetObject(const HandleT & objID);
	// remove object
	void RemoveObject(const HandleT&objID);

	// return the handle id of module
	GLuint AddModule(const std::shared_ptr<ViewModule>& module);
	// TODO: implement a module pool to manage modules by names
	GLuint AddModule(const std::string& name);
	// remove module
	void RemoveModule(const GLuint& modID);

	// print information of itself
	virtual void PrintInfo() const;

	// return object list
	const std::vector<std::shared_ptr<ViewObject>>& GetObjectList();
	// return module list
	const std::vector<std::shared_ptr<ViewModule>>& GetModuleList();
	// bind this group to its parent world
	void BindWorld(const std::shared_ptr<ViewWorld>& worldPtr);
	// bind objects to this group, this function MUST be called AFTER construction 
	void BindObject();
	// get its parent world pointer
	std::weak_ptr<ViewWorld>& Parent();

	// hide all objects
	void Hide();
	//show all objects
	void Show();

	ViewGroup();
	virtual ~ViewGroup();

// data zone
protected:
	std::weak_ptr<ViewWorld> _worldPtr;

	std::vector<std::shared_ptr<ViewObject>> _objs;
	std::vector<std::shared_ptr<ViewModule>> _modules;
	int _objID = 0;
	int _moduleID = 0;
};


class TriangleGroup : public ViewGroup, public std::enable_shared_from_this<TriangleGroup>
{
public:
	TriangleGroup();
	// add modules and objects
	virtual void Init();
	// add some triangles
	void AddTriangle(const GLsizei & num);
protected:

};

class MyGroup : public ViewGroup, public std::enable_shared_from_this<TriangleGroup>
{
public:
	MyGroup();
	virtual void Init();
};