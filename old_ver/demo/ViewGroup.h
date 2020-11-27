#pragma once
#include "Base.h"

class ViewGroup : public std::enable_shared_from_this<ViewGroup>
{
public:
	ViewGroup()
	{
		this->init();
	}
	virtual ~ViewGroup();

	// initial group
	virtual void init() = 0;

	// render with its own modules and objects
	virtual void render() = 0;

	// return the handle id of object
	int AddObject(std::shared_ptr<ViewObject>& obj);
	// return object pointer
	std::shared_ptr<ViewObject> GetObject(int objID);
	// return whether successful
	bool RemoveObject(int objID);

	// return the handle id of module
	int AddModule(std::shared_ptr<ModuleG>& mod);
	// return whether successful
	bool RemoveModule(int modID);

	// return object list
	const std::vector<std::shared_ptr<ViewObject>>& GetObjectList()
	{
		return this->_obj;
	}
	// return module list
	const std::vector<std::shared_ptr<ModuleG>>& GetModuleList()
	{
		return this->_module;
	}

	void BindWorld(std::shared_ptr<ViewWorld> worldPtr)
	{
		this->_worldPtr = worldPtr;
	}
	std::weak_ptr<ViewWorld>& Parent()
	{
		return this->_worldPtr;
	}

	void Hide();

	void Show();

protected:
	std::weak_ptr<ViewWorld> _worldPtr;

	std::vector<std::shared_ptr<ViewObject>> _obj;
	std::vector<std::shared_ptr<ModuleG>> _module;
	int _objID = 0;
	int _moduleID = 0;
};


