#include "ViewHeader.h"
#include <fstream>

std::vector<std::shared_ptr<ViewTriangle>> getAllRenderData() {
	GameObject wolf;
	wolf.loadFromObj("resources/wolf.obj");
	return wolf.getRenderData();
}

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
		if (cnt == 16255) {
			std::cout << "ÔÚÕâÍ£¶Ù£¡";
		}
		if (op == "v") {
			vertex v = {0, 0, 0, 0, 0};
			fin >> v[0] >> v[1] >> v[2];
			vertices.push_back(v);
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
			vertex_info.push_back(tmp);
			for (std::string s : vertex_info) {
				int coord_index_this = std::stoi(s.substr(0, s.find('/'))) - 1;
				coord_index.push_back(coord_index_this);
			}
			for (int i = 2; i < coord_index.size(); i++) {
				// create view triangle
				face f = { vertices[coord_index[0]], vertices[coord_index[1]], vertices[coord_index[i]] };
				std::array<GLfloat, POINTSIZE * 3> data{};
				for (size_t i = 0; i < 3; i++) {
					for (size_t j = 0; j < COORDSIZE; j++)
						data[i * POINTSIZE + j] = f[i][j];
					for (size_t j = COORDSIZE; j < POINTSIZE; j++)
						data[i * POINTSIZE + j] = 0; // texture coord
				}
				faces.push_back(std::shared_ptr<ViewTriangle>(new ViewTriangle(std::move(data))));
			}
		}
		else {
			// ignore the line
			char line[127];
			fin.getline(line, 127);
		}
	}
}

bool GameObject::intersect(GameObject& obj) {
	// treat all objects as balls
	
	GLfloat dist_sq = (this->coordinate[0] - obj.coordinate[0]) * (this->coordinate[0] - obj.coordinate[0])
		+ (this->coordinate[1] - obj.coordinate[1]) * (this->coordinate[1] - obj.coordinate[1])
		+ (this->coordinate[2] - obj.coordinate[2]) * (this->coordinate[2] - obj.coordinate[2]);
	// GLfloat r1_sq = 
	return false;
}

void GameObject::collide(GameObject& obj) {
}

std::vector<std::shared_ptr<ViewTriangle>> GameObject::getRenderData() {
	return faces;
}

GameObject::GameObject() {
	fixed = true;
	mass = 100;
	coordinate = { 0, 0, 0 };
	velocity = { 0, 0, 0 };
	acceleration = { 0,0,0 };
}

void GameObject::error(std::string msg, bool fatal) {
	if (fatal)
		throw(std::exception(msg.c_str()));
	else
		throw(std::logic_error(msg.c_str()));
}
