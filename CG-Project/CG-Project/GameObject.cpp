#include "GameObject.h"
#include "ViewHeader.h"
#include <fstream>

std::vector<std::shared_ptr<ViewTriangle>> getAllRenderData() {
	GameObject wolf;
	wolf.loadFromObj("wolf.obj");
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
		if (op == "v") {
			std::cout << vertices.size();
			vertex v = {0, 0, 0, 0, 0};
			fin >> v[0] >> v[1] >> v[2];
			vertices.push_back(v);
			cnt++;
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
				int coord_index_this = std::stoi(s.substr(0, s.find('/')));
				coord_index.push_back(coord_index_this);
			}
			for (int i = 2; i < coord_index.size(); i++) {
				face f = { vertices[coord_index[0]], vertices[coord_index[1]], vertices[coord_index[i]] };
				faces.push_back(f);
			}
		}
		else {
			// ignore the line
			char line[127];
			fin.getline(line, 127);
		}
	}
	glm::vec3 coord();
	glm::vec4 color();
}

bool GameObject::intersect(GameObject& obj) {
	return false;
}

void GameObject::collide(GameObject& obj) {
}

std::vector<std::shared_ptr<ViewTriangle>> GameObject::getRenderData() {
	std::vector<std::shared_ptr<ViewTriangle>> ret;
	for (auto f : this->faces) {
		std::array<GLfloat, POINTSIZE * 3> vertex{};
		for (long long i = 0; i < 3; i++)
		{
			for (long long j = 0; j < COORDSIZE; j++)
				vertex[i * POINTSIZE + j] = f[i][j];
			for (long long j = COORDSIZE; j < POINTSIZE; j++)
				vertex[i * POINTSIZE + j] = ::RandomReal<GLfloat>(0.0f, 1.0f);
		}
		ret.push_back(std::shared_ptr<ViewTriangle>(new ViewTriangle(std::move(vertex))));
	}
	return ret;
}

GameObject::GameObject(): fixed(true){

}

void GameObject::error(std::string msg, bool fatal) {
	if (fatal)
		throw(std::exception(msg.c_str()));
	else
		throw(std::logic_error(msg.c_str()));
}
