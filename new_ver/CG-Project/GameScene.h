#pragma once
#include "ViewGroup.h"
#include "GameObject.h"
constexpr GLfloat GroundY = 10.0f;
constexpr GLfloat DoorY = GroundY + 30.0f;
constexpr GLfloat DoorThick = 5.0f;
constexpr GLfloat BaseY = -10.0f;
constexpr GLfloat AirWallHeight = 30.0f;
constexpr bool DisplayAirWall = false;
class GameSceneBase
{
public:
	enum class GameState { IdleState = 0, HintState, PlayState, SuccessState };


	GameSceneBase();
	void Rotate(const GLfloat& angle, const glm::vec3& axis);
	void Translate(const glm::vec3& displacement);
	void Scale(const glm::vec3& scaler);
	virtual void Update() = 0;
	virtual void Idle() = 0;
	virtual void Hint() = 0;
	virtual void Play() = 0;
protected:
	// generate air wall, vertices should be (x1, z1, x2, z2, ...) in anticlockwise order 
	std::vector<std::shared_ptr<GameObject>> _objects;
	GameState _state = GameState::IdleState;
	void GenerateAirWall(const std::vector<GLfloat>& vertices);

	float _lastTime;
};

class GroundGameScene : public GameSceneBase
{
public:
	std::shared_ptr<GameObject> _startDoor;
	bool _startDoorFlag = true;
	std::shared_ptr<TexturedPlane> _roadGround;
	GroundGameScene(const GLfloat& roadWidth = 30.0f, const GLfloat& distance = 400.0f);
protected:
	void SetDoorShowFlag(const bool& showFlag);
	GLfloat _roadWidth;
	GLfloat _distance;
};

constexpr GLfloat DesertShowDuration = 1.0f;
constexpr GLfloat DesertPuzzleBarHeight = 25.0f;
constexpr GLfloat PuzzleRadius = 100.0f;
constexpr GLfloat PuzzleBarRadius = 10.0f;
constexpr GLfloat ActiveDistance = 25.0f;

class DesertScene final : public GroundGameScene
{
public:
	DesertScene(const GLfloat& width = 300.0f, const GLfloat& height = 300.0f);
	virtual void Idle()
	{
		if (this->_state == GameState::SuccessState)
			return;
		this->_state = GameState::IdleState;
		GenerateRandomList(0);
		this->SetDoorShowFlag(true);
	}
	virtual void Hint()
	{
		if (this->_state == GameState::SuccessState)
			return;
		this->_state = GameState::HintState;
		GenerateRandomList(5);
		_lastTime = (float)glfwGetTime();
		_startShowIndex = 0;
		this->SetDoorShowFlag(true);
	}
	virtual void Play()
	{
		if (this->_state == GameState::SuccessState)
			return;
		this->_state = GameState::PlayState;
		this->SetDoorShowFlag(false);
		_userList.clear();
		for (int i = 0; i < (int)_targetList.size(); i++)
		{
			_activeBarMap[i] = false;
		}
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
			for (int i = 0; i < (int)_targetList.size(); i++)
			{
				if (_startShowIndex < _targetList.size() && i == _targetList[_startShowIndex]) // hint
					this->ChangeBarState(i, BarState::ActiveBar);
				else
					this->ChangeBarState(i, BarState::IdleBar);
			}
			break;
		case GameState::PlayState:
			// TODO check player distance with bars
			glm::vec3 viewPos = GlobalDataPool::GetData<glm::vec3>("cameraPosition");
			for (int i = 0; i < (int)_puzzleBars.size(); i++)
			{
				if (_activeBarMap[i]) // skip actived bar
					continue;
				if (_puzzleBars[i]->getDist(viewPos) <= ActiveDistance)
				{
					_activeBarMap[i] = true;
					_userList.push_back(i);
				}
			}

			if (this->_userList.size() == this->_targetList.size())
			{
				bool success = true;
				for (int i = 0; i < (int)_targetList.size(); i++)
					if (_userList[i] != _targetList[i])
					{
						success = false;
						break;
					}
				if (success)
				{
					this->_state = GameState::SuccessState;
					break;
				}
			}

			for (int i = 0; i < (int)_targetList.size(); i++)
			{
				if (this->_userList.size() != this->_targetList.size()) // not finish
				{
					if (_activeBarMap[i])
						this->ChangeBarState(i, BarState::ActiveBar);
					else
						this->ChangeBarState(i, BarState::IdleBar);
				}
				else // failed
				{
					this->ChangeBarState(i, BarState::WrongBar);
				}

			}
			break;
		case GameState::IdleState:
			for (int i = 0; i < (int)_targetList.size(); i++)
			{
				this->ChangeBarState(i, BarState::IdleBar);
			}
			break;
		case GameState::SuccessState:
			for (int i = 0; i < (int)_targetList.size(); i++)
			{
				this->ChangeBarState(i, BarState::SuccessBar);
			}
			break;
		}
	}
	enum class BarState { IdleBar, ActiveBar, WrongBar, SuccessBar};


private:

	// generate random target list and puzzle bars
	void GenerateRandomList(const int& num);
	void GenerateBars(const int& num);
	std::shared_ptr<FitTexturedPlane> _areaGround;
	std::vector<std::shared_ptr<GameObject>> _puzzleBars;
	void ChangeBarState(const int& index, const BarState& state);
	std::vector<int> _targetList;
	std::vector<int> _userList;
	std::array<bool, 256> _activeBarMap;
	GLfloat _groundWidth;
	GLfloat _groundHeight;
	int _startShowIndex = 0;
};