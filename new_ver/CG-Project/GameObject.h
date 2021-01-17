#pragma once
#include "ViewHeader.h"
#include "ViewGroup.h"
constexpr int POINTSIZE = 5;
constexpr int COORDSIZE = 3;
typedef TexturedPlane ViewPolygon ;
// alias
typedef std::array<GLfloat, POINTSIZE> vertex;
typedef std::array<vertex, 3> face;

class GameObject {
// member variabels
protected:
	GLfloat		mass;
	glm::vec3	velocity;
	GLfloat		hitRadius;
	std::vector<vertex> vertices;
	std::vector<std::vector<int>> faceIndices;
	std::shared_ptr<ViewObject> viewObj;
	glm::vec3	maxVertexCoord;
	glm::vec3	minVertexCoord;

// public functions
public:
	bool		fixed;
	glm::vec3	localFront;
	glm::vec3	localUp;
	glm::vec3	center;
	void		loadFromObj(std::string filename);
	void		saveToObj(std::string filename);
	void		setHitbox(const std::vector<GLfloat>& vertex_data, const ViewObjectEnum & type);
	void		scale(const glm::vec3& vec);
	void		translate(const glm::vec3& vec, bool detectCollision = false);
	void		rotate(const GLfloat& angle, const glm::vec3& vec);
	void		resetDir();
	void		setVelocity(const glm::vec3 in_v);
	void		setPosition(const glm::vec3 in_pos);
	glm::vec3	getPosition();
	glm::vec3	getFrontDir();
	glm::vec3	getUpDir();
	glm::vec3	getLeftDir();
	bool		collisionPossible(GameObject& obj);
	GLfloat		getDist(const GameObject& obj) const;
	GLfloat		getDist(const glm::vec3& pos) const;

	GameObject(glm::vec3 _Front = { 0, 0, 1 }, glm::vec3 _Up = { 0, 1, 0 });
	GameObject(const std::shared_ptr<ViewObject> viewObject, const std::vector<GLfloat>& vertex_data,
		const bool& withHitBox = true, glm::vec3 _Front = { 0, 0, 1 }, glm::vec3 _Up = { 0, 1, 0 });

	std::shared_ptr<ViewObject> getRenderData()  const;

	static std::vector<std::shared_ptr<GameObject>> allObjs;
	static void AddGameObject(const std::shared_ptr <GameObject>& object);
	static std::vector<std::shared_ptr<GameObject>> GetGameObjects();

// helper functions
private:
	bool		collision(GameObject& obj);
	bool		checkMoveConstraints();
	void		error(std::string msg, bool fatal = true);
};

class Airplane: public GameObject {
private:
	GLfloat		power; // engine power factor, in [-1.0, 1.0]
public:
	void		changePower(GLfloat delta_power);
	void		changeRoll(GLfloat angle);
	void		changePitch(GLfloat angle, bool constraint = false);
	void		changeYaw(GLfloat angle);
	void		setPower(GLfloat _power);
	void		simulate(GLfloat delta_time = 0.016); // asuming 60 ticks per second
	static bool	enableGravity;
	Airplane(glm::vec3 _Front = { 0, 0, 1 }, glm::vec3 _Up = { 0, 1, 0 });
};

class Player : public GameObject {
private:
	GLfloat		viewPitch;
public:
	void		changePitch(GLfloat angle);
	glm::vec3	getViewDir();
	void		move(const glm::vec3& delta);
};

void printVec(glm::vec3);