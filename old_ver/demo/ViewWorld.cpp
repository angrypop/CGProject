#include "ViewWorld.h"
#include "ViewGroup.h"

int ViewWorld::AddGroup(std::shared_ptr<ViewGroup>& group)
{
	this->_group.push_back(group);
	group->BindWorld(this->shared_from_this());
	return this->_groupID++;
}

int ViewWorld::AddModule(std::shared_ptr<ModuleW>& mod)
{
	this->_module.push_back(mod);
	mod->Bind(this->shared_from_this());
	return this->_moduleID++;
}

void GameWorld::render()
{
	// execute preparing modules
	// ...

	// execute group modules
	for (auto it : this->_group)
	{
		it->render();
	}

	// execute ending modules (illumination etc.)
	// ...
}
