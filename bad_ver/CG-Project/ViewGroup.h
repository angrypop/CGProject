#pragma once
#include "ViewHeader.h"

class ViewGroup : public ViewBase, public std::enable_shared_from_this<ViewGroup>
{
// pure virtual function zone
public:
	// initial group
	virtual void Init() = 0;
	// prepare GL settings, must be called in Render()
	virtual void Prepare() = 0;
	// release GL settings, must be called in Render()
	virtual void Release() = 0;

// non-pure virtual function zone
public:
	// render this group
	virtual void Render();

// other function zone
public:
	// add an object to run given module, return the handle id of object
	int AddObject(const std::shared_ptr<ViewObject>& obj, const std::string & moduleName);
	// return object pointer
	std::shared_ptr<ViewObject> GetObject(const HandleT & objID);
	// remove object
	void RemoveObject(const HandleT&objID);

	// return the handle id of module
	GLuint AddModule(const std::shared_ptr<ViewModule>& module, const std::string& name);
	// get module pointer by its name
	std::shared_ptr<ViewModule> GetModule(const std::string& name);
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
	std::vector<std::string> _objModuleName;
	std::vector<std::shared_ptr<ViewModule>> _modules;
	std::map<std::string, std::shared_ptr<ViewModule>> _nameModuleMap;
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

class MyGroup : public ViewGroup, public std::enable_shared_from_this<MyGroup>
{
public:
	// prepare GL settings, must be called in Render()
	virtual void Prepare();
	// release GL settings, must be called in Render()
	virtual void Release();

	MyGroup();
	virtual void Init();
	void AddGameObj(std::shared_ptr<GameObject> ptrGameObj);
	// add some triangles
	void AddTriangle(const GLsizei& num);
	// add a test polygon
	void AddTestPolygon(const GLsizei& triangleNum);
};

//class GBufferIlluminateGroup : public ViewGroup, public std::enable_shared_from_this<GBufferIlluminateGroup>
//{
//public:
//	GBufferIlluminateGroup();
//	virtual void Init();
//
//
//};

