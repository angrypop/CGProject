#pragma once
#include "Base.h"
class ViewWorld : public std::enable_shared_from_this<ViewWorld>
{
public:
	ViewWorld();
	virtual ~ViewWorld();

	// initial group
	virtual void init() = 0;
	// key render
	virtual void render() = 0;

	// return the handle id of object
	int AddGroup(std::shared_ptr<ViewGroup>& group);
	// return whether successful
	bool RemoveGroup(const int & groupID);

	// return the handle id of module
	int AddModule(std::shared_ptr<ModuleW>& mod);
	// return whether successful
	bool RemoveModule(int modID);


protected:
	std::vector<std::shared_ptr<ViewGroup>> _group;
	std::vector<std::shared_ptr<ModuleW>> _module;
	int _groupID = 0;
	int _moduleID = 0;
};

class GameWorld final : public ViewWorld
{
public:
	virtual void init()
	{
		// ...
	}
	virtual void render();
private:

};
