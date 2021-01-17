#pragma once
#include "GameScene.h"

constexpr float PI = 3.1415926f;
constexpr GLfloat RingAngleStep = 2 * PI / 16;
constexpr GLfloat RingHeightStep = 10.0f;
constexpr GLfloat RingGameRadius = 200.0f;
constexpr GLfloat RingOuterRadius = 25.0f;
constexpr GLfloat RingInnerRadius = 15.0f;
constexpr GLfloat PlaneStartX = 0.0f;
constexpr GLfloat PlaneStartY = 50.0f;
constexpr GLfloat PlaneStartZ = 0.0f;

constexpr GLfloat StartDistance = 50.0f;
constexpr GLfloat HitDistance = 40.0f;

class PlaneGameScene final : public GameSceneBase
{
public:
	PlaneGameScene(const int& ringNum = 16);
	int GetRingNumber() const
	{
		return int(this->_rings.size());
	}
	virtual void Update();
	virtual void Idle()
	{
		if (this->_state == GameState::SuccessState)
			return;
		this->_state = GameState::IdleState;
	}
	virtual void Hint() 
	{
		throw("No Hint in Plane Game Scene");
	}
	virtual void Play()
	{
		if (this->_state == GameState::SuccessState)
			return;
		this->_goaledNumber = 0;
		for (int i = 0; i < this->GetRingNumber(); i++)
			_goaledRings[i] = false;
		this->_state = GameState::PlayState;
		std::cout << "Plane Game Started" << std::endl;
	}

private:
	std::vector<std::shared_ptr<GameObject>> _rings;
	std::array<bool, 256> _goaledRings{ 0 };
	int _goaledNumber = 0;
};