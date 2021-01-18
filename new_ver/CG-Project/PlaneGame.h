#pragma once
#include "GameScene.h"

constexpr float PI = 3.1415926f;
constexpr GLfloat RingAngleStep = 2 * PI / 16;
constexpr GLfloat RingHeightStep = 10.0f;
constexpr GLfloat RingGameRadius = 200.0f;
constexpr GLfloat RingOuterRadius = 25.0f;
constexpr GLfloat RingInnerRadius = 15.0f;
constexpr GLfloat PlaneStartX = 0.0f;
constexpr GLfloat PlaneStartY = 100.0f;
constexpr GLfloat PlaneStartZ = 0.0f;

constexpr GLfloat StartDistance = 50.0f;
constexpr GLfloat HitDistance = 40.0f;

class PlaneGameScene final : public GameSceneBase
{
public:
	PlaneGameScene(const int& ringNum = 16);
	int GetRingNumber() const;
	virtual void Update();
	virtual void Idle();
	virtual void Hint();
	virtual void Play();
	virtual void Success();

private:
	void GenerateRings(const int& ringNum = 16);
	std::vector<std::shared_ptr<GameObject>> _rings;
	std::array<bool, 256> _goaledRings{ 0 };
	int _goaledNumber = 0;
};