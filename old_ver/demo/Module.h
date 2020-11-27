#pragma once
#include <memory>

template <class T>
class ModuleBase
{
public:
	ModuleBase() {}
	virtual ~ModuleBase() {}

	void Bind(std::shared_ptr<T> groupPtr)
	{
		this->_parentPtr = groupPtr;
	}
	std::weak_ptr<T> Parent()
	{
		return this->_parentPtr;
	}
private:
	std::weak_ptr<T> _parentPtr;

};

