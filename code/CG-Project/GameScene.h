#pragma once
#include "ViewGroup.h"
#include "GameObject.h"
constexpr GLfloat GroundY = 10.0f;
constexpr GLfloat DoorY = GroundY + 30.0f;
constexpr GLfloat DoorThick = 5.0f;
constexpr GLfloat BaseY = -10.0f;
constexpr GLfloat GroundX = 100.0f;
constexpr GLfloat AirWallHeight = 20.0f;
constexpr bool DisplayAirWall = false;
class GameSceneBase
{
public:
	enum class GameState { IdleState = 0, HintState, PlayState, SuccessState };


	GameSceneBase();
	void Rotate(const GLfloat& angle, const glm::vec3& axis);
	void Translate(const glm::vec3& displacement);
	void Scale(const glm::vec3& scaler);
	bool CheckSuccess() const;
	GameSceneBase::GameState GetState() const;
	virtual void Update() = 0;
	virtual void Idle() = 0;
	virtual void Hint() = 0;
	virtual void Play() = 0;
	virtual void Success() = 0;
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
	GroundGameScene(const GLfloat& roadWidth = 30.0f, const GLfloat& distance = 150.0f);
protected:
	void SetDoorShowFlag(const bool& showFlag);
	GLfloat _roadWidth;
	GLfloat _distance;
};

constexpr bool DebugCheatDesert = false;

constexpr GLfloat DesertShowDuration = 1.0f;
constexpr GLfloat DesertPuzzleBarHeight = 25.0f;
constexpr GLfloat PuzzleRadius = 100.0f;
constexpr GLfloat PuzzleBarRadius = 10.0f;
constexpr GLfloat ActiveDistance = 25.0f;

class DesertScene final : public GroundGameScene
{
public:
	DesertScene(const GLfloat& width = 300.0f, const GLfloat& height = 300.0f);
	virtual void Idle();
	virtual void Hint();
	virtual void Play();
	virtual void Success();
	virtual void Update();

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
	std::array<bool, 256> _activeBarMap{0};
	GLfloat _groundWidth;
	GLfloat _groundHeight;
	int _startShowIndex = 0;
};