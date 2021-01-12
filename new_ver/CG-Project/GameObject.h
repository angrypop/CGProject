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
	bool		fixed;
	GLfloat		mass;
	glm::vec3	velocity;
	std::vector<face> faces;
	std::shared_ptr<ViewPolygon> viewObj;
	std::shared_ptr<ViewPolygon> hitBox;

// public functions
public:
	glm::vec3	localFront;
	glm::vec3	localUp;
	void		loadFromObj(std::string filename);
	bool		collisionPossible(GameObject& obj);
	bool		collision(GameObject& obj);
	void		scale(const glm::vec3& vec);
	void		translate(const glm::vec3& vec);
	void		rotate(const GLfloat& angle, const glm::vec3& vec);
	void		setDir(const glm::vec3& currentDir, const glm::vec3& dir);
	glm::vec3	getPosition();
	glm::vec3	getFrontDir();
	glm::vec3	getUpDir();
	glm::vec3	getLeftDir();
	std::shared_ptr<ViewPolygon> getRenderData();
	GameObject(glm::vec3 _Front = { 0, 0, 1 }, glm::vec3 _Up = { 0, 1, 0 });

// helper functions
private:
	bool		checkMoveConstraints();
	void		error(std::string msg, bool fatal = true);
};

class Airplane: public GameObject {
private:
	GLfloat		power; // engine power factor, in [-1.0, 1.0]
public:
	void		changePower(GLfloat delta_power);
	void		changeRoll(GLfloat angle);
	void		changePitch(GLfloat angle);
	void		changeYaw(GLfloat angle);
	void		simulate(GLfloat delta_time = 0.016); // asuming 60 ticks per second
	static bool	enableGravity;
	Airplane(glm::vec3 _Front = { 0, 0, 1 }, glm::vec3 _Up = { 0, 1, 0 });
};
