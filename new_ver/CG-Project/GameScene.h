#pragma once
#include "ViewGroup.h"
constexpr GLfloat GroundY = 10.0f;
constexpr GLfloat DoorY = GroundY + 30.0f;
constexpr GLfloat DoorThick = 5.0f;
constexpr GLfloat BaseY = -10.0f;
class GameSceneBase
{
public:
	std::vector<std::shared_ptr<ViewObject>> _objects;

	std::shared_ptr<TransparentPlane> _startDoor;
	std::shared_ptr<TexturedPlane> _roadGround;
	
	bool _successFlag = false;
	bool _startFlag = false;
	GLfloat _distance;
	GLfloat _width;
	GLfloat _height;
	GameSceneBase(const GLfloat& width = 30.0f, const GLfloat& height = 30.0f, const GLfloat& distance = 400.0f);
	virtual void Start() = 0;
	virtual void End() = 0;
};

class DesertScene : public GameSceneBase
{
public:
	virtual void Start();
	virtual void End()
	{

	}
	std::shared_ptr<FitTexturedPlane> _areaGround;
};