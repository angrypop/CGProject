#include "ViewGroup.h"
#include "ViewObject.h"
#include "Module.h"

int ViewGroup::AddObject(std::shared_ptr<ViewObject>& obj)
{
	this->_obj.push_back(obj);
	obj->Bind(this->shared_from_this());
    return this->_objID++;
}

int ViewGroup::AddModule(std::shared_ptr<ModuleG>& mod)
{
	this->_module.push_back(mod);
	mod->Bind(this->shared_from_this());
	return this->_moduleID++;
}

void ViewGroup::Hide()
{
	for (auto it : _obj)
		it->Hide();
}

void ViewGroup::Show()
{
	for (auto it : _obj)
		it->Show();
}




