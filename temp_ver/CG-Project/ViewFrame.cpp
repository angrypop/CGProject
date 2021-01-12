#include "ViewFrame.h"

std::map<std::string, GLuint> ViewFramePool::_nameFBOMap;
std::map<GLuint, std::shared_ptr<ViewFrame>> ViewFramePool::_FBOFrameMap;

GLuint ViewFramePool::GetFrameFBO(const std::string& name)
{
	auto target = _nameFBOMap.find(name);
	if (target != _nameFBOMap.end())
		return target->second;
	else
		throw(std::string("No Frame Name = ") + name);
}

std::shared_ptr<ViewFrame> ViewFramePool::GetFrame(const GLuint& FBO)
{
	auto target = _FBOFrameMap.find(FBO);
	if (target != _FBOFrameMap.end())
		return target->second;
	else
		throw(std::string("No Frame FBO = ") + std::to_string(FBO));
}

std::shared_ptr<ViewFrame> ViewFramePool::GetFrame(const std::string& name)
{
	return GetFrame(GetFrameFBO(name));
}

GLuint ViewFramePool::AddFrame(const std::shared_ptr<ViewFrame> frame, const std::string& name)
{
	_FBOFrameMap.insert(std::pair<GLuint, std::shared_ptr<ViewFrame>>(frame->GetFBO(), frame));
	if (name == DefaultName)
	{
		std::string&& nameDefault = name + std::to_string(_nameFBOMap.size()) + "___";
		_nameFBOMap.insert(std::pair<std::string, GLuint>(nameDefault, frame->GetFBO()));
	}
	else
		_nameFBOMap.insert(std::pair<std::string, GLuint>(name, frame->GetFBO()));
	return frame->GetFBO();
}
