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
		if (cnt % 10 == 0)
			group->PrintInfo();
		// if (cnt % 5 == 0 && PERFORMANCETEST)
			// dynamic_cast<TriangleGroup *>(group.get())->AddTriangle(30);
		group->Render();
		if (!PERFORMANCETEST)
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(750ms);
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
	// add groups
	// this->AddGroup(std::shared_ptr<TriangleGroup>(new TriangleGroup()));
	this->AddGroup(std::shared_ptr<MyGroup>(new MyGroup()));
}

void GameWorld::InitCallback()
{
	// bind callback function
	glfwSetKeyCallback(_window, KeyCallback);
	glfwSetCursorPosCallback(_window, MouseCallback);
	glfwSetMouseButtonCallback(_window, MouseButtonCallback);
	glfwSetScrollCallback(_window, ScrollCallback);
}

void GameWorld::UpdateData()
{
	for (const auto& group : this->_groups)
		for (const auto& obj : group->GetObjectList()) {
			obj->Rotate(15.0f, glm::vec3(0.1f, 0.3f, 0.7f));
		}
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
	//std::shared_ptr<UniformData<glm::mat4>> m(new UniformData<glm::mat4>(std::string("M"), glm::mat4()));
	//GlobalUniformDataPool::Add(m);
	// V and P matrix
	std::shared_ptr<UniformData<glm::mat4>> v(new UniformData<glm::mat4>(std::string("V"), glm::mat4()));
	GlobalUniformDataPool::Add(v);
	std::shared_ptr<UniformData<glm::mat4>> p(new UniformData<glm::mat4>(std::string("P"), glm::mat4()));
	GlobalUniformDataPool::Add(p);
}

void GameWorld::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		printf("W\n");
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		printf("A\n");
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		printf("S\n");
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		printf("D\n");
	}
}