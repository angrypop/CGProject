#include "ViewHeader.h"

HandleT ViewBase::_handleCnt = 0;

HandleT ViewBase::GetHandle() const
{
	return this->_handle;
}

ViewBase::ViewBase()
{
	this->_handle = _handleCnt++;
}
