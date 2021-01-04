#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>

#include <memory>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <random>
#include <map>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "stb_image.h"

typedef long long HandleT;

constexpr int COORDSIZE = 3;							// every point has coordinate data like (x, y, z)
constexpr int COLORSIZE = 2;							// every point has color data like (r, g, b, a)
constexpr int POINTSIZE = COORDSIZE + COLORSIZE;		// every point has color and coordinate data

enum class ViewObjectEnum { PlaneObject, BallObject, TriangleObject, PolygonObject };
// Base Class
	class ViewBase;

// View Class
	// View Object
	class ViewObject;
		class PlaneObject;		// TODO
		class BallObject;		// TODO
		class TriangleObject;

	// View Group
	class ViewGroup;
		class DesertGroup;		// TODO
		class SnowfieldGroup;	// TODO
		class TriangleGroup;

	// View World
	class ViewWorld;
		class GameWorld;
// Texture class
	class ViewTexture;
	class ViewTexturePool;
// Texture class
	class ViewFrame;
	class ViewFramePool;

// Shader class
	class ShaderObject;
	class ShaderPool;
// Module Class
	class ViewModule;
		class ViewTriangle;

// Data Class
class UniformDataBase;
	template <typename T>
	class UniformData;

class BufferDataBase;
	template <typename T, GLsizei N, GLsizei M>
	class BufferData;

// get a random real number in [min, max]
template <typename T>
inline T RandomReal(const T& min, const T& max)
{
	static std::default_random_engine e((unsigned int)time(nullptr));
	std::uniform_real_distribution<T> u(min, max);
	return u(e);
}

// get a random int number in [min, max]
template <typename T>
inline T RandomInt(const T& min, const T& max)
{
	static std::default_random_engine e((unsigned int)time(nullptr));
	std::uniform_int_distribution<T> u(min, max);
	return u(e);
}

// default base class of all classes
// to provide a handle for binding
class ViewBase
{
// other function zone
public:
	// get handle of this object
	HandleT GetHandle() const;
	ViewBase();
	virtual ~ViewBase() {}

// data zone
protected:
	HandleT _handle;
private:
	static HandleT _handleCnt;
};

