#include "GameObject.h"
#include <fstream>


std::vector<std::shared_ptr<GameObject>> GameObject::allObjs;
bool Airplane::enableGravity = false;

void GameObject::loadFromObj(std::string filename) {
	// WARNING: IGNORING MATERIALS (.mtl FILES)!
	std::ifstream fin(filename);
	if (!fin) error("Error opening file " + filename);
	std::string op;
	std::vector<vertex> vertices;
	int cnt = 0;
	while (fin >> op) {
		cnt++;
		// std::cout << " line: " << cnt << std::endl;
		if (op == "v") {
			// read 1 vertex
			vertex v = {0, 0, 0, 0, 0};
			fin >> v[0] >> v[1] >> v[2];
			vertices.push_back(v);
			if (v[0] * v[0] + v[1] * v[1] + v[2] * v[2] > hitRadius * hitRadius)
				hitRadius = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			for (int i = 0; i < 3; i++) {
				if (v[i] > maxVertexCoord[i]) maxVertexCoord[i] = v[i];
				if (v[i] < minVertexCoord[i]) minVertexCoord[i] = v[i];
			}
		}
		else if (op == "f") {
			std::vector<std::string> vertex_info;
			std::vector<int> coord_index;
			std::string tmp = "";
			char line[127];
			fin.getline(line, 127);
			for (int i = 0; line[i] != '\0'; i++) {
				char c = line[i];
				if (c != ' ') {
					tmp += c;
				}
				else {
					if(tmp != "") // omit white spaces
						vertex_info.push_back(tmp);
					tmp = "";
				}
			}
			if (tmp != "") vertex_info.push_back(tmp);
			for (std::string s : vertex_info) {
				int coord_index_this = std::stoi(s.substr(0, s.find('/'))) - 1;
				coord_index.push_back(coord_index_this);
			}
			if (coord_index.size() == 4)
			{
				faces.push_back(face({ vertices[coord_index[0]], vertices[coord_index[2]], vertices[coord_index[1]] }));
				faces.push_back(face({ vertices[coord_index[0]], vertices[coord_index[2]], vertices[coord_index[3]] }));
			}
			else
			{
				faces.push_back(face({ vertices[coord_index[0]], vertices[coord_index[1]], vertices[coord_index[2]] }));
			}
		}
		else {
			// ignore the line
			char line[127];
			fin.getline(line, 127);
		}
	}
	// create view polygon
	std::vector<GLfloat> polydata;
	for (auto f : faces) {
		for (size_t i = 0; i < 3; i++) {
			for (size_t j = 0; j < COORDSIZE; j++)
				polydata.push_back(f[i][j]);
			for (size_t j = COORDSIZE; j < POINTSIZE; j++)
				polydata.push_back(RandomReal<GLfloat>(-1.0f, 1.0f)); // texture coord
		}
	}
	viewObj = std::shared_ptr<ViewPolygon>(new ViewPolygon(polydata));
}

bool GameObject::collisionPossible(GameObject& obj) {
	// fast collision dectection
	// treat all objects as balls
	glm::vec3 coord = this->getPosition();
	glm::vec3 coord2 = obj.getPosition();
	GLfloat dist_sq = (coord[0] - coord2[0]) * (coord[0] - coord2[0])
		+ (coord[1] - coord2[1]) * (coord[1] - coord2[1])
		+ (coord[2] - coord2[2]) * (coord[2] - coord2[2]);
	// printf("thisR: %.1f    thatR: %.1f    dist: %.1f", hitRadius, hitRadius, sqrt(dist_sq));
	return sqrt(dist_sq) <= (hitRadius + obj.hitRadius);
}

bool GameObject::collision(GameObject& obj) {
	if(!collisionPossible(obj)) return false;// fast detection
	// determine if this is in obj
	glm::vec4 coord = glm::vec4(getPosition(), 1);
	coord = glm::inverse(obj.viewObj->GetM()) * coord;
	coord /= coord[3];
	/*std::cout << "coord ";  printVec(coord);
	std::cout << "min ";  printVec(minVertexCoord);
	std::cout << "max ";  printVec(maxVertexCoord);*/
	for (int i = 0; i < 3; i++)
		if (!(coord[i] > obj.minVertexCoord[i] && coord[i] < obj.maxVertexCoord[i]))
			return false;
	// determine if obj is in this
	// not implemented
	return true;
}

void GameObject::scale(const glm::vec3& vec) {
	hitRadius *= (GLfloat(vec.x) + GLfloat(vec.y) + GLfloat(vec.z)) / 3.0f;
	this->viewObj->Scale(vec);
}

void GameObject::translate(const glm::vec3& vec, bool detectCollision) {
	this->viewObj->Translate(vec);
	if (detectCollision) {
		for (auto ptr : allObjs) {
			if (this->viewObj->GetHandle() == ptr->viewObj->GetHandle()) continue;
			if (collision(*ptr)) { // fast dectection
				if (ptr->fixed) {
					// undo translation
					this->viewObj->Translate(-vec);
					return;
				}
				else {
					static const GLfloat delta = 1e-3f;
					int minStepNeeded = 500;
					glm::vec3 optimizedDir = { 0, 0, delta };
					for(GLfloat x = -delta; x <= delta; x+= delta / 2)
						for (GLfloat y = -delta; y <= delta; y += delta / 2)
							for (GLfloat z = -delta; z <= delta; z += delta / 2) {
								glm::vec3 deltaDir = { x, y, z };
								int steps = 1;
								ptr->translate(deltaDir * GLfloat(1));
								while (collision(*ptr) && steps < minStepNeeded) {// ����
									ptr->translate(deltaDir * GLfloat(steps));
									steps *= 2;
								}
								ptr->translate(-deltaDir * GLfloat(steps));
								if (steps < minStepNeeded) {
									minStepNeeded = steps;
									optimizedDir = deltaDir;
								}
							}
					ptr->translate(optimizedDir * GLfloat(minStepNeeded));
					std::cout << "minsteps = " << minStepNeeded << "\n";
				}
			}
		}
	}
}

void GameObject::rotate(const GLfloat& angle, const glm::vec3& vec) {
	this->viewObj->Rotate(angle, vec);
}

void GameObject::setDir(const glm::vec3& currentDir, const glm::vec3& dir)
{
	glm::vec3 rotateAxis = glm::normalize(glm::inverse(viewObj->GetM()) * glm::vec4(glm::cross(currentDir, dir), 1));
	GLfloat rotateAngle = acos(glm::dot(currentDir, dir)) * 180.f / 3.1416f;
	/*std::cout << "rotateAngle: " << rotateAngle << std::endl;
	std::cout << "rotateAxks: "; printVec(rotateAxis);
	std::cout << std::endl;*/
	if (rotateAngle < 5.0) return;
	if (rotateAxis == glm::vec3({ 0, 0, 0 })) return;
	this->viewObj->Rotate(rotateAngle, rotateAxis);
}

void GameObject::setVelocity(const glm::vec3 in_v)
{
	velocity = in_v;
}

void GameObject::setPosition(const glm::vec3 in_pos)
{
   	glm::vec4 tagPos = glm::inverse(viewObj->GetM()) * glm::vec4(in_pos, 1);
	tagPos /= tagPos[3];
	glm::vec3 delta = glm::vec3(tagPos) - glm::vec3(0, 0, 0);
	translate(delta);
}

glm::vec3 GameObject::getPosition()
{
	glm::vec4 coord = this->viewObj->GetM() * glm::vec4({ 0, 0, 0, 1 });
	return glm::vec3(coord) / coord[3];
}

glm::vec3 GameObject::getFrontDir()
{
	glm::vec4 coord_front = this->viewObj->GetM() * glm::vec4(localFront, 1);
	glm::vec4 coord_center = this->viewObj->GetM() * glm::vec4({ 0, 0, 0, 1 });
	glm::vec3 delta = coord_front - coord_center;
	return glm::normalize(delta);
}

glm::vec3 GameObject::getUpDir()
{
	glm::vec4 coord_up = this->viewObj->GetM() * glm::vec4(localUp, 1);
	glm::vec4 coord_center = this->viewObj->GetM() * glm::vec4({ 0, 0, 0, 1 });
	glm::vec3 delta = coord_up - coord_center;
	return glm::normalize(delta);
}

glm::vec3 GameObject::getLeftDir()
{
	return glm::cross(getUpDir(), getFrontDir());
}

std::shared_ptr<ViewPolygon> GameObject::getRenderData() {
	return this->viewObj;
}

GameObject::GameObject(glm::vec3 _Front, glm::vec3 _Up) {
	fixed = true;
	mass = 1;
	velocity = { 0, 0, 0 };
	localFront = _Front;
	localUp = _Up;
	hitRadius = 0;
	maxVertexCoord = { 0, 0, 0 };
	minVertexCoord = { 0, 0, 0 };
}

bool GameObject::checkMoveConstraints()
{
	// if (this->getPosition().y < 0) return false;
	return true;
}

void GameObject::error(std::string msg, bool fatal) {
	if (fatal)
		throw(std::exception(msg.c_str()));
	else
		throw(std::logic_error(msg.c_str()));
}

void Airplane::changePower(GLfloat delta_power)
{
	power += delta_power;
	if (power > 1.0) power = 1.0;
	else if (power < -1.0) power = -1.0;
}

void Airplane::changeRoll(GLfloat angle)
{
	rotate(angle, localFront);
}

void Airplane::changePitch(GLfloat angle)
{
	rotate(angle, glm::cross(localUp, localFront));
}

void Airplane::changeYaw(GLfloat angle)
{
	rotate(angle, localUp);
}

void Airplane::setPower(GLfloat _power)
{
	power = _power;
}

void Airplane::simulate(GLfloat delta_time)
{
	// constants
	static const GLfloat liftFactor = 1e-2f;
	static const GLfloat resisFactor = 2e-3f;
	static const GLfloat vFollowFactor = 0.1f;
	static const GLfloat maxPower = 5e4f;
	// static const GLfloat maxSpeed = 1e2;
	// let velocity follow front dir
	velocity = vFollowFactor * localFront * glm::length(velocity) + (1 - vFollowFactor) * velocity;
	// directions
	glm::vec3 dir_front = localFront;
	glm::vec3 dir_up = localUp;
	glm::vec3 dir_v = glm::normalize(velocity);
	if (velocity == glm::vec3({ 0,0,0 })) dir_v = dir_front;
	// temp values
	GLfloat v = glm::length(velocity);
	GLfloat v_front = glm::length(velocity * dir_front);
	// forces
	glm::vec3 gravity = { 0, -9.8 * mass, 0 };
	if (!enableGravity) gravity = { 0, 0, 0 };
	glm::vec3 lift = glm::vec3({ 0, 1, 0 }) * liftFactor * (v_front * v_front) * glm::dot(dir_up, glm::vec3(0, 1, 0));
	glm::vec3 thrust = dir_front * maxPower * power;
	if (glm::length(thrust) * v > maxPower) { // P = FV
		thrust = dir_front * maxPower * power / v;
	}
	glm::vec3 resistance = -dir_v * resisFactor * (v * v);
	// check lift vs gravity
	if (glm::length(lift) > glm::length(gravity)) {
		lift = gravity;
	}
	else {
		// rotate
		changePitch(glm::length(lift - gravity) * (1 + dir_front.y));
	}
	// apply acceleration
	glm::vec3 acceleration = (thrust + lift + gravity + resistance) / mass;
	velocity += acceleration * delta_time;
	// move
	translate(velocity * delta_time, true);
	// debug info
	static int cnt = 0; // print every 100 calls
	if (cnt++ % 100) return;
	printf("%-10s ", "pos"); printVec(getPosition());
	//printf("%-10s ", "front"); printVec(getFrontDir());
	printf("%-10s ", "acceleration"); printVec(acceleration);
	printf("%-10s %f\n", "velocity", v);
	printf("%-10s %f\n", "power", power);
	printf("%-10s %f\n", "thrust", glm::length(thrust));
	printf("%-10s %f\n", "resistance", glm::length(resistance));
	printf("%-10s %f\n", "lift", glm::length(lift));
	printf("%-10s %f\n", "v_front", v_front);
	std::cout << std::endl;
}

Airplane::Airplane(glm::vec3 _Front, glm::vec3 _Up): GameObject(_Front, _Up)
{
	power = 0.0;
}

void printVec(glm::vec3 vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << "\n";
}