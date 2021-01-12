#include "ViewHeader.h"

ViewGroup::ViewGroup()
{
	//glGenVertexArrays(1, &_VAO);
	//glBindVertexArray(_VAO);
}

ViewGroup::~ViewGroup()
{
	//glDeleteVertexArrays(1, &_VAO);
}

int ViewGroup::AddObject(const std::shared_ptr<ViewObject>& obj)
{
	this->_objs.push_back(obj);
	//obj->Bind(this->shared_from_this());
    return this->_objID++;
}

std::shared_ptr<ViewObject> ViewGroup::GetObject(const HandleT & objID)
{
	for (const auto& it : this->_objs)
		if (it->GetHandle() == objID)
			return it;
	return nullptr;
}

void ViewGroup::RemoveObject(const HandleT& objID)
{
	for (auto it = std::begin(_objs); it != std::end(_objs); it++)
		if ((*it)->GetHandle() == objID)
		{
			_objs.erase(it);
			break;
		}
}

GLuint ViewGroup::AddModule(const std::shared_ptr<ViewModule>& mod)
{
	this->_modules.push_back(mod);
	//mod->Bind(this->shared_from_this());
	return this->_moduleID++;
}

GLuint ViewGroup::AddModule(const std::string& name)
{
	throw std::exception("NOT IMPLEMENT");
	return 0;
}

void ViewGroup::RemoveModule(const GLuint& modID)
{
	for (auto it = std::begin(_modules); it != std::end(_modules); it++)
		if ((*it)->GetProgramHandle() == modID)
		{
			_modules.erase(it);
			break;
		}
}

void ViewGroup::PrintInfo() const
{
	std::cout << "ViewGroup::Render: sizeof objs = " << _objs.size() << " sizeof modules = " << _modules.size() << std::endl;
}

const std::vector<std::shared_ptr<ViewObject>>& ViewGroup::GetObjectList()
{
	return this->_objs;
}

const std::vector<std::shared_ptr<ViewModule>>& ViewGroup::GetModuleList()
{
	return this->_modules;
}

void ViewGroup::BindWorld(const std::shared_ptr<ViewWorld>& worldPtr)
{
	this->_worldPtr = worldPtr;
}

void ViewGroup::BindObject()
{
	for (const auto& obj : _objs)
	{
		std::weak_ptr<ViewGroup> ptr = this->weak_from_this();
		obj->Bind(ptr);
	}
}

std::weak_ptr<ViewWorld>& ViewGroup::Parent()
{
	return this->_worldPtr;
}

void ViewGroup::Hide()
{
	for (auto && it : _objs)
		it->Hide();
}

void ViewGroup::Show()
{
	for (auto && it : _objs)
		it->Show();
}


void ViewGroup::Render()
{
	
	//glPointSize(5);
	for (const auto& module : _modules)
	{
		for (const auto& obj : _objs)
		{
			if (!(obj->IsHide()))
				obj->Render(*module);
		}
	}
}

TriangleGroup::TriangleGroup()
{
	this->Init();
}

void TriangleGroup::Init()
{
	// add objects
	constexpr int TESTNUMBER = 3;
	this->AddTriangle(TESTNUMBER);

	// add modules
	this->AddModule(std::shared_ptr<TestModule>(new TestModule()));
}

void TriangleGroup::AddTriangle(const GLsizei& num)
{
	//std::array<GLfloat, POINTSIZE * 3> vertex {
//	0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//	0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 
//	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
//};
	std::array<GLfloat, POINTSIZE * 3> vertex{};
	constexpr GLfloat SIZE = 10.0f;
	for (int k = 0; k < num; k++)
	{
		for (long long i = 0; i < 3; i++)
		{
			for (long long j = 0; j < COORDSIZE; j++)
				vertex[i * POINTSIZE + j] = ::RandomReal<GLfloat>(-SIZE, SIZE);
			for (long long j = COORDSIZE; j < POINTSIZE; j++)
				vertex[i * POINTSIZE + j] = ::RandomReal<GLfloat>(-1.0f, 1.0f);
		}
		this->AddObject(std::shared_ptr<ViewTriangle>(new ViewTriangle(std::move(vertex))));
	}
}

MyGroup::MyGroup()
{
	this->Init();
}


void MyGroup::Init()
{
	// initialize objects


	//auto vec = this->_objs[0]->GetBufferData<GLfloat>();

	//for (const auto& it : vec)
	//	std::cout << it << " ";
	//std::cout << std::endl;

	//this->AddTriangle(10);
	//auto vecc = this->_objs[1]->GetBufferData<GLfloat, 3, POINTSIZE>();
	//for (const auto& it : vecc)
	//	std::cout << it << " ";
	//std::cout << std::endl;
	// initialize modules
	this->AddModule(std::shared_ptr<TestModule>(new TestModule()));
}

void MyGroup::AddGameObj(std::shared_ptr<GameObject> ptrGameObj)
{
	this->AddObject(ptrGameObj->getRenderData());
}

void MyGroup::AddTriangle(const GLsizei& num)
{
	std::array<GLfloat, POINTSIZE * 3> vertex{};
	constexpr GLfloat SIZE = 10.0f;
	for (int k = 0; k < num; k++)
	{
		for (long long i = 0; i < 3; i++)
		{
			for (long long j = 0; j < COORDSIZE; j++)
				vertex[i * POINTSIZE + j] = ::RandomReal<GLfloat>(-SIZE, SIZE);
			for (long long j = COORDSIZE; j < POINTSIZE; j++)
				vertex[i * POINTSIZE + j] = ::RandomReal<GLfloat>(-1.0f, 1.0f);
		}
		this->AddObject(std::shared_ptr<ViewTriangle>(new ViewTriangle(std::move(vertex))));
	}
}

void MyGroup::AddTestPolygon(const GLsizei& triangleNum)
{
	std::vector<GLfloat> polyVertex;
	constexpr GLfloat SIZE = 3.0f;
	for (int k = 0; k < triangleNum; k++)
	{
		for (long long i = 0; i < 3; i++)
		{
			for (long long j = 0; j < COORDSIZE; j++)
				polyVertex.push_back(::RandomReal<GLfloat>(-SIZE, SIZE));
			for (long long j = COORDSIZE; j < POINTSIZE; j++)
				polyVertex.push_back(::RandomReal<GLfloat>(-1.0f, 1.0f));
		}
	}
	std::shared_ptr<ViewPolygon> ptr(new ViewPolygon(polyVertex));
	ptr->Translate(glm::vec3(0.0f, -10.0f, -10.0f));
	this->AddObject(ptr);
}
