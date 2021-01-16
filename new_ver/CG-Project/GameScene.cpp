#include "GameScene.h"
#include "Scene.h"

GameSceneBase::GameSceneBase(const GLfloat& roadWidth, const GLfloat& distance)
	:_roadWidth(roadWidth), _distance(distance), _lastTime((float)glfwGetTime())
{
	std::vector<TextureInfo> planeTextures{
		{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Color.jpg"},
		{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
		{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
		{Texture_NONE,""} };
	std::vector<GLfloat> groudPoints{
		-roadWidth / 2.0f, GroundY, 0.0f, 0.0f, 0.0f,
		roadWidth / 2.0f, GroundY, 0.0f, 1.0f, 0.0f,
		-roadWidth / 2.0f, GroundY, _distance, 0.0f, 1.0f,
		roadWidth / 2.0f, GroundY, _distance, 1.0f, 1.0f,
		-roadWidth / 2.0f, BaseY, 0.0f, 0.0f, 0.0f,
		roadWidth / 2.0f, BaseY, 0.0f, 1.0f, 0.0f,
		-roadWidth / 2.0f, BaseY, _distance, 0.0f, 1.0f,
		roadWidth / 2.0f, BaseY, _distance, 1.0f, 1.0f
	};
	std::vector<GLuint> groudIndices{
		0, 1, 2,
		1, 2, 3,
		4, 5, 6,
		5, 6, 7,
		0, 2, 6,
		0, 4, 6,
		0, 1, 5,
		0, 4, 5,
		1, 5, 7,
		1, 3, 7,
		2, 3, 7,
		2, 6, 7
	};
	std::vector<GLfloat> doorPoints{
		-roadWidth / 2.0f, GroundY, distance, 0.0f, 0.0f, 1.0f,
		roadWidth / 2.0f, GroundY, distance, 0.0f, 0.0f, 1.0f,
		-roadWidth / 2.0f, DoorY, distance, 0.0f, 0.0f, 1.0f,
		roadWidth / 2.0f, DoorY, distance, 0.0f, 0.0f, 1.0f,
		-roadWidth / 2.0f, GroundY, distance + DoorThick, 0.0f, 0.0f, 1.0f,
		roadWidth / 2.0f, GroundY, distance + DoorThick, 0.0f, 0.0f, 1.0f,
		-roadWidth / 2.0f, DoorY, distance + DoorThick, 0.0f, 0.0f, 1.0f,
		roadWidth / 2.0f, DoorY, distance + DoorThick, 0.0f, 0.0f, 1.0f,
	};
	std::vector<GLuint> doorIndices{
		0, 1, 2,
		1, 2, 3,
		4, 5, 6,
		5, 6, 7,
		0, 2, 6,
		0, 4, 6,
		0, 1, 5,
		0, 4, 5,
		1, 5, 7,
		1, 3, 7,
		2, 3, 7,
		2, 6, 7
	};
	std::shared_ptr<GameObject> gameObj;

	this->_roadGround.reset(new TexturedPlane(groudPoints, groudIndices, planeTextures));
	gameObj = std::make_shared<GameObject>(GameObject(this->_roadGround, groudPoints));
	this->_objects.push_back(gameObj);
	GameObject::AddGameObject(gameObj);
	Scene::AddGroupObject(_roadGround);

	this->_startDoor.reset(new TransparentPlane(doorPoints, doorIndices, glm::vec4(1.0f, 0.3f, 0.0f, 0.7f)));
	gameObj = std::make_shared<GameObject>(GameObject(this->_startDoor, doorPoints));
	this->_objects.push_back(gameObj);
	GameObject::AddGameObject(gameObj);
	Scene::AddGroupObject(_startDoor);

}

void GameSceneBase::Rotate(const GLfloat& angle, const glm::vec3& axis)
{
	for (auto& obj : _objects)
	{
		obj->getRenderData()->Rotate(angle, axis);
	}
}

void GameSceneBase::Translate(const glm::vec3& displacement)
{
	for (auto& obj : _objects)
	{
		obj->getRenderData()->Translate(displacement);
	}
}

void GameSceneBase::Scale(const glm::vec3& scaler)
{
	for (auto& obj : _objects)
	{
		obj->getRenderData()->Scale(scaler);
	}
}

DesertScene::DesertScene(const GLfloat& width, const GLfloat& height)
{
	TextureInfo sandTextures[] = {
		{AmbientTexture,"Ground033_4K-JPG/Ground033_4K_Color.jpg"},
		{NormalTexture,"Ground033_4K-JPG/Ground033_4K_Normal.jpg"},
		{RoughnessTexture,"Ground033_4K-JPG/Ground033_4K_Roughness.jpg"},
		{Texture_NONE,""}
	};
	std::vector<GLfloat> planePoints{
		-width / 2.0f, GroundY, _distance, 0.0f , 0.0f ,
		-width / 2.0f, GroundY, _distance + height, 0.0f , 1.0f ,
		width / 2.0f, GroundY, _distance, 1.0f, 0.0f ,
		width / 2.0f, GroundY, _distance + height, 1.0f , 1.0f
	};
	GLuint planeIndices[] = {
		 0, 1, 2, // first triangle
		 1, 2, 3  // second triangle
	};
	ShaderInfo planeShaders[] = {
		{GL_VERTEX_SHADER,"Ground.vert"} ,
		{GL_FRAGMENT_SHADER,"Ground.frag"} ,
		{GL_NONE,""}
	};
	auto fitPlane = std::shared_ptr<FitTexturedPlane>(new FitTexturedPlane(
		glm::vec3(0, 0, 0), planePoints.data(), planePoints.size() * sizeof(GLfloat),
		planeIndices, sizeof(planeIndices), planeShaders, sandTextures));
	fitPlane->UpdateHeight(0, 1);
	auto gameObj = std::make_shared<GameObject>(GameObject(fitPlane, planePoints, false));
	this->_objects.push_back(gameObj);
	GameObject::AddGameObject(gameObj);
	Scene::AddGroupObject(fitPlane);
}

void DesertScene::GenerateRandomList(const int& num)
{
	_targetList.clear();
	while (_targetList.size() < num)
	{
		int res = ::RandomInt(0, num - 1);
		bool flag = true;
		for (const auto& it : _targetList)
			if (it == res)
			{
				flag = false;
				break;
			}
		if (flag)
			_targetList.push_back(res);
	}
}
