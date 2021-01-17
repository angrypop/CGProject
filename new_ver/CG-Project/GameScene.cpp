#include "GameScene.h"
#include "Scene.h"

GameSceneBase::GameSceneBase()
	:_lastTime((float)glfwGetTime())
{}



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

void GameSceneBase::GenerateAirWall(const std::vector<GLfloat>& vertices)
{
	//return;
	constexpr int verSize = 2;
	int num = (int)vertices.size() / verSize;
	for (int i = 0; i < num - 1; i++)
	{
		GLfloat x1 = vertices[i * verSize + 0];
		GLfloat z1 = vertices[i * verSize + 1];
		GLfloat x2 = vertices[((i + 1) % num) * verSize + 0];
		GLfloat z2 = vertices[((i + 1) % num) * verSize + 1];
		GLfloat distance = sqrt((z2 - z1) * (z2 - z1) + (x2 - x1) * (x2 - x1));
		GLfloat k = DoorThick / distance;
		GLfloat xoff = (z2 - z1) * k;
		GLfloat zoff = (x2 - x1) * -k;
		GLfloat x3 = x1 + xoff;
		GLfloat z3 = z1 + zoff;
		GLfloat x4 = x2 + xoff;
		GLfloat z4 = z2 + zoff;

		std::vector<GLfloat> airWallVertices{
			x1, GroundY, z1, 0.0f, 0.0f,
			x2, GroundY, z2, 0.0f, 0.0f,
			x1, GroundY + AirWallHeight, z1, 0.0f, 0.0f,
			x2, GroundY + AirWallHeight, z2, 0.0f, 0.0f,
			x3, GroundY, z3, 0.0f, 0.0f,
			x4, GroundY, z4, 0.0f, 0.0f,
			x3, GroundY + AirWallHeight, z3, 0.0f, 0.0f,
			x4, GroundY + AirWallHeight, z4, 0.0f, 0.0f,
		};
		std::vector<GLuint> airWallIndices{
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
		std::vector<TextureInfo> planeTextures{
	{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Color.jpg"},
	{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
	{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
	{Texture_NONE,""} };
		auto wall = std::shared_ptr<TexturedPlane>(new TexturedPlane(
			airWallVertices, airWallIndices, planeTextures));
		Scene::AddGroupObject(wall);
		if (!DisplayAirWall)
			wall->Hide(); // hide this object
		auto gameObj = std::make_shared<GameObject>(GameObject(wall, airWallVertices));
		this->_objects.push_back(gameObj);
		GameObject::AddGameObject(gameObj);
	}
}

GroundGameScene::GroundGameScene(const GLfloat& roadWidth, const GLfloat& distance)
	:_roadWidth(roadWidth), _distance(distance)
{
	std::vector<TextureInfo> planeTextures{
		{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Color.jpg"},
		{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
		{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
		{Texture_NONE,""} };
	std::vector<GLfloat> roadPoints{
		-roadWidth / 2.0f, GroundY, 0.0f, 0.0f, 0.0f,
		roadWidth / 2.0f, GroundY, 0.0f, 1.0f, 0.0f,
		-roadWidth / 2.0f, GroundY, _distance, 0.0f, 1.0f,
		roadWidth / 2.0f, GroundY, _distance, 1.0f, 1.0f,
		-roadWidth / 2.0f, BaseY, 0.0f, 0.0f, 0.0f,
		roadWidth / 2.0f, BaseY, 0.0f, 1.0f, 0.0f,
		-roadWidth / 2.0f, BaseY, _distance, 0.0f, 1.0f,
		roadWidth / 2.0f, BaseY, _distance, 1.0f, 1.0f
	};
	std::vector<GLuint> roadIndices{
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

	this->_roadGround.reset(new TexturedPlane(roadPoints, roadIndices, planeTextures));
	gameObj = std::make_shared<GameObject>(GameObject(this->_roadGround, roadPoints));
	this->_objects.push_back(gameObj);
	GameObject::AddGameObject(gameObj);
	Scene::AddGroupObject(_roadGround);
	this->GenerateAirWall({
		roadWidth / 2.0f, 0.0f,
		roadWidth / 2.0f, distance,
		});
	this->GenerateAirWall({
		-roadWidth / 2.0f, 0.0f,
		-roadWidth / 2.0f,  distance,
		});

	auto viewDoor = std::shared_ptr<TransparentPlane>(new TransparentPlane(doorPoints, doorIndices, glm::vec4(1.0f, 0.3f, 0.0f, 0.3f)));
	Scene::AddGroupObject(viewDoor);
	this->_startDoor.reset(new GameObject(viewDoor, doorPoints, true));
	this->_objects.push_back(_startDoor);
	GameObject::AddGameObject(_startDoor);

}

void GroundGameScene::SetDoorShowFlag(const bool& showFlag)
{
	if (_startDoorFlag == true && showFlag == false)
	{
		this->_startDoor->translate(glm::vec3(0.0f, -10000.0f, 0.0f));
		this->_startDoorFlag = false;
	}
	if (_startDoorFlag == false && showFlag == true)
	{
		this->_startDoor->translate(glm::vec3(0.0f, 10000.0f, 0.0f));
		this->_startDoorFlag = true;
	}
}



DesertScene::DesertScene(const GLfloat& width, const GLfloat& height)
	:_groundWidth(width), _groundHeight(height)
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
		glm::vec3(0, 0, 0), planePoints.data(), (int)planePoints.size() * sizeof(GLfloat),
		planeIndices, (int)sizeof(planeIndices), planeShaders, sandTextures));

	glm::vec3 uniObjPos = GlobalDataPool::GetData<glm::vec3>("uniObjPos");
	glm::vec3 uniObjVel = GlobalDataPool::GetData<glm::vec3>("uniObjVel");
	fitPlane->UpdateHeight(0, 1, uniObjPos, uniObjVel);
	auto gameObj = std::shared_ptr<GameObject>(new GameObject(fitPlane, planePoints, false));
	this->_objects.push_back(gameObj);
	GameObject::AddGameObject(gameObj);
	Scene::AddGroupObject(fitPlane);
	this->GenerateAirWall({
		_roadWidth / 2.0f, _distance,
		width / 2.0f, _distance,
		width / 2.0f, _distance + height,
		-width / 2.0f, _distance + height,
		-width / 2.0f, _distance,
		-_roadWidth / 2.0f, _distance,
	});
}

void DesertScene::GenerateRandomList(const int& num)
{
	constexpr GLfloat PI = 3.1415926f;
	this->GenerateBars(num);

	_targetList.clear();
	int index = 0;
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
		{
			_targetList.push_back(res);
			// set bar position
			GLfloat barAngle = (float)index / num * 2 * PI;
			auto viewObj = _puzzleBars[index]->getRenderData();
			viewObj->SetM(glm::mat4(1.0f));
			//viewObj->Translate(glm::vec3(0.0f, 0.0f, ));
			viewObj->Translate(glm::vec3(PuzzleRadius * cos(barAngle), 0.0f,
				this->_distance + this->_groundHeight / 2.0 + PuzzleRadius * sin(barAngle)));
			index++;
		}

	}
}

void DesertScene::GenerateBars(const int& num)
{
	constexpr GLfloat PI = 3.1415926f;
	
	for (int index = (int)_puzzleBars.size(); index < num; index++)
	{
		int edgeNum = 8;
		std::vector<GLfloat> barVertices;
		std::vector<GLuint> barIndices;
		for (int i = 0; i < edgeNum; i++) // 0 ... n - 1
		{

			GLfloat angle = (float)i / edgeNum * 2 * PI;
			GLfloat x = cos(angle) * PuzzleBarRadius;
			GLfloat z = sin(angle) * PuzzleBarRadius;
			barVertices.push_back(x); barVertices.push_back(GroundY); barVertices.push_back(z);
			if (i % 2 == 0)
			{
				barVertices.push_back(0.0f);
				barVertices.push_back(1.0f);
			}
			else
			{
				barVertices.push_back(1.0f);
				barVertices.push_back(0.0f);
			}

		}
		for (int i = 0; i < edgeNum; i++) // n ... 2 * n - 1
		{
			constexpr GLfloat PI = 3.1415926f;
			GLfloat angle = (float)i / edgeNum * 2 * PI;
			GLfloat x = cos(angle) * PuzzleBarRadius;
			GLfloat z = sin(angle) * PuzzleBarRadius;
			barVertices.push_back(x); barVertices.push_back(GroundY + DesertPuzzleBarHeight); barVertices.push_back(z);
			if (i % 2 == 0)
			{
				barVertices.push_back(1.0f);
				barVertices.push_back(0.0f);
			}
			else
			{
				barVertices.push_back(0.0f);
				barVertices.push_back(1.0f);
			}
		}
		// 2 * n
		barVertices.push_back(0.0f); barVertices.push_back(GroundY); barVertices.push_back(0.0f);
		barVertices.push_back(0.5f); barVertices.push_back(0.5f);
		// 2 * n + 1
		barVertices.push_back(0.0f); barVertices.push_back(GroundY + DesertPuzzleBarHeight); barVertices.push_back(0.0f);
		barVertices.push_back(0.5f); barVertices.push_back(0.5f);
		// lower floor
		for (int i = 0; i < edgeNum; i++)
		{
			barIndices.push_back((i) % edgeNum);
			barIndices.push_back((i + 1) % edgeNum);
			barIndices.push_back(edgeNum * 2);
		}
		// upper floor
		for (int i = 0; i < edgeNum; i++)
		{
			barIndices.push_back((i) % edgeNum + edgeNum);
			barIndices.push_back((i + 1) % edgeNum + edgeNum);
			barIndices.push_back(edgeNum * 2 + 1);
		}
		// side
		for (int i = 0; i < edgeNum; i++)
		{
			barIndices.push_back((i) % edgeNum);
			barIndices.push_back((i + 1) % edgeNum);
			barIndices.push_back((i + 1) % edgeNum + edgeNum);
			barIndices.push_back((i) % edgeNum);
			barIndices.push_back((i) % edgeNum + edgeNum);
			barIndices.push_back((i + 1) % edgeNum + edgeNum);
		}

		auto viewObj = std::shared_ptr<TexturedPlane>(new TexturedPlane(barVertices, barIndices, { {Texture_NONE, ""} }));
		Scene::AddGroupObject(viewObj);
		auto gameObj = std::shared_ptr<GameObject>(new GameObject(viewObj, barVertices));
		this->_objects.push_back(gameObj);
		GameObject::AddGameObject(gameObj);
		_puzzleBars.push_back(gameObj);

	}
}


void DesertScene::ChangeBarState(const int& index, const BarState& state)
{
	if (index >= _puzzleBars.size())
		throw("Bar Index Out of Range");
	auto bar = std::dynamic_pointer_cast<TexturedPlane>(_puzzleBars[index]->getRenderData());
	switch (state)
	{
	case BarState::IdleBar:
		bar->ChangeTexture(std::vector<TextureInfo>({
			{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Gray.jpg"},
			{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
			{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
		}));
		break;
	case BarState::ActiveBar:
		bar->ChangeTexture(std::vector<TextureInfo>({
			{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Yellow.jpg"},
			{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
			{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
		}));
		break;
	case BarState::WrongBar:
		bar->ChangeTexture(std::vector<TextureInfo>({
			{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Red.jpg"},
			{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
			{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
			}));
		break;
	case BarState::SuccessBar:
		bar->ChangeTexture(std::vector<TextureInfo>({
			{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Green.jpg"},
			{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
			{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
		}));
		break;
	}

}
