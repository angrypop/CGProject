#include "GameObject.h"
#include <fstream>


std::vector<std::shared_ptr<GameObject>> GameObject::allObjs;
bool Airplane::enableGravity = false;

void GameObject::loadFromObj(std::string filename) {
	if (!vertices.empty()) {
		error("This game object has already been loaded from another .obj file");
	}
	// WARNING: IGNORING MATERIALS (.mtl FILES)!
	std::ifstream fin(filename);
	if (!fin) error("Error opening file " + filename);
	std::vector<face> faces;
	std::string op;
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
			faceIndices.push_back(coord_index);
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
	std::vector<TextureInfo> infos
	{
		{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Color.jpg"},
		{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
		{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
	};
	std::dynamic_pointer_cast<TexturedPlane>(viewObj)->ChangeTexture(infos);
}

void GameObject::saveToObj(std::string filename)
{
	static bool working = false;
	if (working) return;
	working = true;
	auto task = [=]() {
		std::cout << "Writing obj begin" << std::endl;
		std::ofstream fout(filename);
		if (!fout) error("Error opening file " + filename);
		fout << "# Auto generated obj\n";
		for (const vertex& v : vertices) {
			glm::vec4 coord = { v[0], v[1], v[2], 1 };
			coord = viewObj->GetM()* glm::vec4(coord) ;
			coord /= coord.w;
			glm::vec3 delta = glm::vec3(coord) - getPosition();
			fout << "v " << delta[0] << " " << delta[1] << " " << delta[2] << "\n";
		}
		for (int i = 0; i < vertices.size(); i++) {
			fout << "vt 0.0 0.0\n";
		}
		for (int i = 0; i < vertices.size(); i++) {
			fout << "vn 0.0 1.0 0.0\n";
		}
		for (auto f : faceIndices) {
			fout << "f";
			for (int i = 0; i < f.size(); i++) {
				fout << " " << std::to_string(f[i] + 1) << "/1/1";
			}
			fout << "\n";
		}
		fout.close();
		std::cout << "Writing obj finished" << std::endl;
		working = false;
	};
	std::thread write(task);
	write.detach();
}

void GameObject::setHitbox(const std::vector<GLfloat>& vertex_data, const ViewObjectEnum& type)
{
	std::vector<vertex> vertices;
	int VerSize = 5;
	switch (type)
	{
	case ViewObjectEnum::TextureObject:
		VerSize = 5;
		break;
	case ViewObjectEnum::TransparentObject:
		VerSize = 6;
		break;
	}

	for (int i = 0; i < vertex_data.size() / VerSize; i++) {
		vertices.push_back({ vertex_data[VerSize * i + 0], vertex_data[VerSize * i + 1], vertex_data[VerSize * i + 2], 0,0 });
	}
	for (auto v : vertices) {
		if (v[0] * v[0] + v[1] * v[1] + v[2] * v[2] > hitRadius * hitRadius)
			hitRadius = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		for (int i = 0; i < 3; i++) {
			if (v[i] > maxVertexCoord[i]) maxVertexCoord[i] = v[i];
			if (v[i] < minVertexCoord[i]) minVertexCoord[i] = v[i];
		}
	}

}

std::shared_ptr<ViewObject> GameObject::getRenderData() const
{
	return this->viewObj;
}

void GameObject::AddGameObject(const std::shared_ptr<GameObject>& object)
{
	allObjs.push_back(object);
}

std::vector<std::shared_ptr<GameObject>> GameObject::GetGameObjects()
{
	return allObjs;
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

GLfloat GameObject::getDist(const GameObject& obj) const
{
	throw("no implement");
	glm::vec3 delta = this->center - obj.center;
	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

GLfloat GameObject::getDist(const glm::vec3& pos) const
{
	glm::vec4 delta = (this->getRenderData()->GetM() *  glm::vec4(this->center, 1.0f)) - glm::vec4(pos, 1.0f);
	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
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
					// set the plane velocity to zero
					this->setVelocity(glm::vec3(0.0f));
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
								while (collision(*ptr) && steps < minStepNeeded) {// ±¶Ôö
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
					// std::cout << "minsteps = " << minStepNeeded << "\n";
				}
			}
		}
	}
}

void GameObject::rotate(const GLfloat& angle, const glm::vec3& vec) {
	this->viewObj->Rotate(angle, vec);
}

void GameObject::resetDir()
{
	//return;
	glm::vec3 front = getFrontDir();
	GLfloat currentPitch = atan(front.y / sqrt(front.x * front.x + front.z * front.z)) * RADIAN_TO_ANGLE;
	this->viewObj->Rotate(-currentPitch, glm::cross(localFront, localUp));
	front = getFrontDir();
	if (front.x == 0) return;
	GLfloat currentRoll = atan(front.z / front.x) * RADIAN_TO_ANGLE;
	if (front.x < 0) currentRoll = (front.z / fabs(front.z)) * 90.0f;
	this->viewObj->Rotate(-currentRoll, localUp);
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

GameObject::GameObject(glm::vec3 _Front, glm::vec3 _Up) {
	fixed = true;
	mass = 1;
	velocity = { 0, 0, 0 };
	localFront = _Front;
	localUp = _Up;
	hitRadius = 0;
	maxVertexCoord = { -1e9, -1e9, -1e9 };
	minVertexCoord = { 1e9, 1e9, 1e9 };
	center = { 0, 0, 0 };
}

GameObject::GameObject(const std::shared_ptr<ViewObject> viewObject, const std::vector<GLfloat>& vertex_data,
	const bool& withHitBox, glm::vec3 _Front, glm::vec3 _Up)
	:GameObject(_Front, _Up)
{
	this->viewObj = viewObject;
	if (withHitBox)
		this->setHitbox(vertex_data, viewObject->GetType());
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

void Airplane::changePitch(GLfloat angle, bool constraint)
{
	if (constraint) {
		glm::vec3 front = getFrontDir();
		GLfloat currentPitch = atan(front.y / sqrt(front.x * front.x + front.z * front.z)) * RADIAN_TO_ANGLE;
		if (currentPitch + angle > 60.0f) 
			angle = 0;
		if (currentPitch + angle < -60.0f) 
			angle = 0;
		std::cout << currentPitch << "\n";
	}
	rotate(angle, glm::cross(localFront, localUp));
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
	static const GLfloat resisFactor = 1e-3f;
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
	glm::vec3 gravity = - dir_up * 9.8f * mass;
	if (!enableGravity) gravity = { 0, 0, 0 };
	glm::vec3 lift = dir_up * liftFactor * (v_front * v_front);
	glm::vec3 thrust = dir_front * maxPower * power;
	if (glm::length(thrust) * v > maxPower) { // P = FV
		thrust = dir_front * maxPower * power / v;
	}
	glm::vec3 resistance = -dir_v * resisFactor * (v * v);
	// check lift vs gravity
	if (glm::length(lift) > glm::length(gravity)) {
		lift = -gravity;
	}
	else {
		// rotate
		changePitch((glm::length(lift) - glm::length(gravity)));
	}
	// apply acceleration
	glm::vec3 acceleration = (thrust + lift + gravity + resistance) / mass;
	if (glm::dot(velocity, acceleration) < 0 &&
		glm::dot(velocity, velocity + acceleration * delta_time) < 0) {
		acceleration = {0, 0, 0};
	}
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

void Player::changePitch(GLfloat angle)
{
	viewPitch += angle;
	if (viewPitch > 89.0f) viewPitch = 89.0f;
	if (viewPitch < -89.0f) viewPitch = -89.0f;
}

glm::vec3 Player::getViewDir()
{
	glm::vec3 front = getFrontDir();
	front.y = tan(glm::length(front) * glm::radians(viewPitch));
	return front;
}

void Player::move(const glm::vec3& delta)
{
	translate({ delta.x, 0, delta.z }, true);
}
