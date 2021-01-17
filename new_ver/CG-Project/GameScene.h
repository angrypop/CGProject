#pragma once
#include "ViewGroup.h"
#include "GameObject.h"
constexpr GLfloat GroundY = 10.0f;
constexpr GLfloat DoorY = GroundY + 30.0f;
constexpr GLfloat DoorThick = 5.0f;
constexpr GLfloat BaseY = -10.0f;
class GameSceneBase
{
public:
	enum class GameState { IdleState = 0, HintState, PlayState, SuccessState };
	std::vector<std::shared_ptr<GameObject>> _objects;

	std::shared_ptr<TransparentPlane> _startDoor;
	std::shared_ptr<TexturedPlane> _roadGround;
	
	GameState _state = GameState::IdleState;

	GameSceneBase(const GLfloat& roadWidth = 30.0f, const GLfloat& distance = 400.0f);
	void Rotate(const GLfloat& angle, const glm::vec3& axis);
	void Translate(const glm::vec3& displacement);
	void Scale(const glm::vec3& scaler);
	virtual void Update() = 0;
	virtual void Idle() = 0;
	virtual void Hint() = 0;
	virtual void Play() = 0;
protected:
	GLfloat _roadWidth;
	GLfloat _distance;
	float _lastTime;
};

constexpr float DesertShowDuration = 1.0f;
class DesertScene final : public GameSceneBase 
{
public:
	DesertScene(const GLfloat& width = 300.0f, const GLfloat& height = 300.0f);
	virtual void Idle()
	{
		this->_state = GameState::IdleState;
		GenerateRandomList(0);
	}
	virtual void Hint()
	{
		this->_state = GameState::HintState;
		GenerateRandomList(5);
		_lastTime = (float)glfwGetTime();
		_startShowIndex = 0;
	}
	virtual void Play()
	{
		this->_state = GameState::PlayState;
	}
	virtual void Update()
	{
		float nowTime = (float)glfwGetTime();
		float duration = nowTime - _lastTime;
		switch (this->_state)
		{
		case GameState::HintState:
			if (_startShowIndex >= int(_targetList.size())) // take a break
			{
				if (duration >= DesertShowDuration * 3) // long duration
				{
					_startShowIndex = 0;
					_lastTime = nowTime;
				}
			}
			else if (duration >= DesertShowDuration) // show next
			{
				_startShowIndex++;
				_lastTime = nowTime;
			}
			break;
		case GameState::PlayState:
			// TODO check player distance with bars
			break;
		case GameState::IdleState:
			break;
		case GameState::SuccessState:
			break;
		}
	}
	enum class BarState { IdleBar, ActiveBar, WrongBar };


private:


	void GenerateRandomList(const int& num);
	std::shared_ptr<FitTexturedPlane> _areaGround;
	std::vector<std::shared_ptr<GameObject>> _puzzleBars;
	void ChangeBarState(const int& index, const BarState& state);
	std::vector<int> _targetList;
	int _startShowIndex = 0;
};