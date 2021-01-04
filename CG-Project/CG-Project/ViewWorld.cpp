#include "ViewHeader.h"

void ViewWorld::Init()
{
	// basic initial
	glfwInit();
	_window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// basic GL settings
	this->InitGLSetting();

	// texture buffer initial
	this->InitTexture();

	// global uniform data initial
	this->InitGlobalUniformData();

	// world class modules initial
	this->InitModules();

	// groups initial
	this->InitGroups();

	// key & mouse & etc callback initial
	this->InitCallback();


}

void ViewWorld::Render()
{
	// background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static const float BLACK[] = { 0.2f, 0.2f, 0.2f, 0.0f };
	::glClearBufferfv(GL_COLOR, 0, BLACK);

	// run every group
	for (const auto& group : _groups)
	{
		static int cnt = 0;
		cnt++;
		if (cnt % 10 == 0 && PERFORMANCETEST)
			group->PrintInfo();
		if (cnt % 5 == 0 && PERFORMANCETEST)
			dynamic_cast<TriangleGroup *>(group.get())->AddTriangle(30);
		group->Render();
		if (!PERFORMANCETEST)
		{
			using namespace std::chrono_literals;
			//std::this_thread::sleep_for(750ms);
		}
	}

	// run top module for every obj in this world
	for (const auto& group : _groups)
		for (const auto& obj : group->GetObjectList())
			for (const auto& module : _modules)
			{
				//std::cout << "World Render Part 2" << std::endl;
				obj->Render(*module);
			}

}

std::shared_ptr<ViewObject> ViewWorld::GetObject(const HandleT& ID) const
{
	for (const auto& group : this->_groups)
	{
		auto objs = group->GetObjectList();
		for (const auto& obj : objs)
			if (obj->GetHandle() == ID)
				return obj;
	}
	throw(std::string("No object has ID = ") + std::to_string(ID));
}

void ViewWorld::Loop()
{
	constexpr double FPSsamplingRate = 0.25;

	time_t start = clock(), end;
	int cnt = 0, index = 0;
	static double fps[5] {0.0};
	while (!glfwWindowShouldClose(_window)) {
		//gameWorldModel->Update();
		this->UpdateData();
		this->Render();
		::glfwSwapBuffers(_window);
		::glfwPollEvents();
		cnt++;
		end = clock();
		if ((double)(end - start) / CLOCKS_PER_SEC >= FPSsamplingRate)
		{
			start = end;
			fps[index] = (double)cnt;
			index = (index + 1) % 5;
			cnt = 0;
			double sum = 0.0;
			for (int i = 0; i < 5; i++)
				sum += fps[i];
			std::cout << "fps = " << sum / (5 * FPSsamplingRate) << std::endl;
		}

	}

}

HandleT ViewWorld::AddGroup(std::shared_ptr<ViewGroup>&& group)
{
	this->_groups.push_back(group);
	group->BindObject();
	return group->GetHandle();
}

void ViewWorld::RemoveGroup(const int& groupID)
{
	for (auto it = std::begin(_groups); it != std::end(_groups); it++)
		if ((*it)->GetHandle() == groupID)
		{
			_groups.erase(it);
			break;
		}
}

GLuint ViewWorld::AddModule(std::shared_ptr<ViewModule>&& mod)
{
	this->_modules.push_back(mod);
	//mod->Bind(this->shared_from_this());
	return mod->GetProgramHandle();
}

void ViewWorld::RemoveModule(int modID)
{
	// TODO
}

std::string ViewWorld::GetName() const
{
	return this->_name;
}

void ViewWorld::SetName(const std::string& name)
{
	this->_name = name;
}

ViewWorld::~ViewWorld()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

GameWorld::GameWorld()
{
	this->Init();
	std::cout << "Game world successfully initialized" << std::endl;
}

void GameWorld::InitGLSetting()
{
	::glEnable(GL_DEPTH_TEST);
	::glDepthFunc(GL_LEQUAL);
	::glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GameWorld::InitModules()
{
	// add modules
}

void GameWorld::InitGroups()
{
	GameObjects.push_back(std::shared_ptr<GameObject>(new GameObject()));
	GameObjects[0]->loadFromObj("resources/wolf.obj");
	GameObjects[0]->scale({ 20, 20, 20 });
	GameObjects.push_back(std::shared_ptr<GameObject>(new GameObject()));
	GameObjects[1]->loadFromObj("resources/cottage.obj");
	GameObjects[1]->scale({ 0.1, 0.1, 0.1 });
	GameObjects[1]->translate({ -5, 0, 0 });
	auto ptrGrp = std::shared_ptr<MyGroup>(new MyGroup());
	for (auto i : GameObjects) {
		ptrGrp->AddGameObj(i);
	}
	this->AddGroup(ptrGrp);
}

void GameWorld::InitCallback()
{
	// bind callback function
	glfwSetKeyCallback(_window, Interaction::KeyCallback);
	glfwSetCursorPosCallback(_window, Interaction::MouseCallback);
	glfwSetMouseButtonCallback(_window, Interaction::MouseButtonCallback);
	glfwSetScrollCallback(_window, Interaction::ScrollCallback);
}

void GameWorld::UpdateData()
{
	glm::mat4 Vnow = Interaction::camera.GetViewMatrix();
	UniformDataPool::SetData<glm::mat4>("V", Vnow);
	double delta = 0.1;
	if (Interaction::key_w_pressed) {
		GameObjects[0]->translate({ 0, 0, delta });
	}
	if (Interaction::key_s_pressed) {
		GameObjects[0]->translate({ 0, 0, -delta });
	}
	if (Interaction::key_a_pressed) {
		GameObjects[0]->translate({ delta, 0, 0 });
	}
	if (Interaction::key_d_pressed) {
		GameObjects[0]->translate({ -delta, 0, 0 });
	}
	if (Interaction::spaceFlag == false)
		GameObjects[0]->rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void GameWorld::InitTexture()
{
	const std::string TexturePath = "..\\texture\\";
	std::shared_ptr<ViewTexture> ptr;
	ptr.reset(new ViewTexture(TexturePath + "Ground003_4K-JPG\\" + "Ground003_4K_Color.jpg", "uniColorSampler"));
	ViewTexturePool::Add(ptr, "ColorTexture");

}

void GameWorld::InitGlobalUniformData()
{
	// V and P matrix
	UniformDataPool::Add("V", glm::mat4(1.0f));
	UniformDataPool::Add("P", glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 500.0f));
}

//void GameWorld::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
//		printf("W\n");
//	}
//	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
//		printf("A\n");
//	}
//	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
//		printf("S\n");
//	}
//	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
//		printf("D\n");
//	}
//}