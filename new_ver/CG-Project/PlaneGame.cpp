#include "PlaneGame.h"
#include "Scene.h"
#include "Interaction.h"

PlaneGameScene::PlaneGameScene(const int& ringNum)
{


}

int PlaneGameScene::GetRingNumber() const
{
	return int(this->_rings.size());
}

void PlaneGameScene::Update()
{
	float nowTime = (float)glfwGetTime();
	float duration = nowTime - _lastTime;
	switch (this->_state)
	{
	case GameState::IdleState:
		glm::vec3 viewPos = GlobalDataPool::GetData<glm::vec3>("cameraPosition");
		//std::shared_ptr<Airplane> plane = Scene::airplane;
		//std::cout << "plang dist = " << Scene::airplane->getDist(viewPos) << " state = " << Scene::desertScene->CheckSuccess() << std::endl;
		if (Scene::airplane->getDist(viewPos) <= StartDistance && Scene::desertScene->CheckSuccess())
		{
			this->Play();
		}
		break;
	case GameState::PlayState:
		viewPos = Scene::airplane->getPosition();
		for (int i = 0; i < _rings.size(); i++)
		{
			//std::cout << "index = " << i << " dist = " << _rings[i]->getDist(viewPos) << std::endl;
			if (_rings[i]->getDist(viewPos) <= HitDistance && !_goaledRings[i])
			{
				_goaledRings[i] = true;
				this->_goaledNumber++;
				std::cout << "goaledNumber = " << _goaledNumber << "/" << this->GetRingNumber() << std::endl;
			}
		}

		if (this->_goaledNumber >= this->GetRingNumber()) // success
		{
			this->Success();
			break;
		}
		for (int i = 0; i < _rings.size(); i++)
		{
			if (_goaledRings[i])
				std::dynamic_pointer_cast<TransparentPlane>(_rings[i]->getRenderData())->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
			else
				std::dynamic_pointer_cast<TransparentPlane>(_rings[i]->getRenderData())->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
		}
		break;
	case GameState::SuccessState:
		for (int i = 0; i < _rings.size(); i++)
		{
				std::dynamic_pointer_cast<TransparentPlane>(_rings[i]->getRenderData())->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
		}
		break;
	}
}

void PlaneGameScene::Idle()
{
	this->_state = GameState::IdleState;
	for (int i = 0; i < this->GetRingNumber(); i++)
	{
		_rings[i]->getRenderData()->Hide();
	}

}

void PlaneGameScene::Hint()
{
	throw("No Hint in Plane Game Scene");
}

void PlaneGameScene::Play()
{
	if (this->_state == GameState::SuccessState)
		return;

	this->_goaledNumber = 0;
	for (int i = 0; i < this->GetRingNumber(); i++)
		_goaledRings[i] = false;
	this->_state = GameState::PlayState;
	std::cout << "Plane Game Started" << std::endl;
	Interaction::operationMode = 1;
	this->GenerateRings();
	Interaction::displayTakeOffCGFlag = true;
}

void PlaneGameScene::Success()
{
	this->_state = GameState::SuccessState;
	Interaction::displayCrashCGFlag = true;
	std::cout << "Plane Game Scene Success!" << std::endl;
}

void PlaneGameScene::GenerateRings(const int& ringNum)
{
	for (int index = 0; index < ringNum; index++)
	{
		GLfloat angleRing = index * RingAngleStep;
		GLfloat ringY = PlaneStartY + index * RingHeightStep;

		int edgeNum = 256;
		std::vector<GLfloat> ringVertices;
		std::vector<GLuint> ringIndices;
		for (int i = 0; i < edgeNum; i++) // 0 ... n - 1
		{
			GLfloat angle = (float)i / edgeNum * 2 * PI;
			GLfloat x = cos(angle) * RingInnerRadius * cos(angleRing);
			GLfloat y = sin(angle) * RingInnerRadius;
			GLfloat z = cos(angle) * RingInnerRadius * sin(angleRing);
			ringVertices.push_back(x); ringVertices.push_back(y); ringVertices.push_back(z);
			ringVertices.push_back(0.0f); ringVertices.push_back(0.0f); ringVertices.push_back(1.0f);
		}
		for (int i = 0; i < edgeNum; i++) // n ... 2 * n - 1
		{
			GLfloat angle = (float)i / edgeNum * 2 * PI;
			GLfloat x = cos(angle) * RingOuterRadius * cos(angleRing);
			GLfloat y = sin(angle) * RingOuterRadius;
			GLfloat z = cos(angle) * RingOuterRadius * sin(angleRing);
			ringVertices.push_back(x); ringVertices.push_back(y); ringVertices.push_back(z);
			ringVertices.push_back(0.0f); ringVertices.push_back(0.0f); ringVertices.push_back(1.0f);
		}

		// indices
		for (int i = 0; i < edgeNum; i++)
		{
			ringIndices.push_back((i) % edgeNum);
			ringIndices.push_back((i + 1) % edgeNum);
			ringIndices.push_back((i) % edgeNum + edgeNum);
			ringIndices.push_back((i) % edgeNum + edgeNum);
			ringIndices.push_back((i + 1) % edgeNum + edgeNum);
			ringIndices.push_back((i) % edgeNum);
		}


		auto viewObj = std::shared_ptr<TransparentPlane>(new TransparentPlane(ringVertices, ringIndices, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f)));

		//viewObj->Rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
		viewObj->Translate(glm::vec3(RingGameRadius * cos(angleRing), ringY, RingGameRadius * sin(angleRing)));


		Scene::AddGroupObject(viewObj);
		auto gameObj = std::shared_ptr<GameObject>(new GameObject(viewObj, ringVertices, false));
		this->_objects.push_back(gameObj);
		GameObject::AddGameObject(gameObj);
		_rings.push_back(gameObj);

	}
}
