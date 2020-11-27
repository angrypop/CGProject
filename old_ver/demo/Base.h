#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <memory>
#include <string>
#include <vector>
#include "Module.h"

// View Class
	enum class ViewObjectEnum { PlaneObjectE, BallObjectE };
	// View Object
	class ViewObject;
		class PlaneObject;
		class BallObject;

	// View Group
	class ViewGroup;
		class DesertGroup;
		class SnowfieldGroup;

	// View World
		class ViewWorld;

// Model Class
	template <class T> class ModuleBase;

		typedef ModuleBase<ViewGroup> ModuleG;
		typedef ModuleBase<ViewWorld> ModuleW;



