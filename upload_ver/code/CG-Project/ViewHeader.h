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
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class ViewPassEnum { ShadowBuffer, GBuffer, OITBuffer, BlendOIT, GBufferIlluminate };
enum class ViewObjectEnum { TextureObject, FitTextureObject, TransparentObject, WaterObject, SkyBoxObject};
constexpr std::string_view TexturePath = "..\\texture\\";
constexpr std::string_view ScreenShotPath = "..\\screenshot\\";
constexpr std::string_view ShaderPath = "..\\shader\\";


// get a random real number in [min, max]
template <typename T>
inline T RandomReal(const T& min, const T& max)
{
	static std::default_random_engine e((unsigned int)time(nullptr));
	std::uniform_real_distribution<T> u(min, max);
	return u(e);
}

// get a random int number in [min, max]s
template <typename T>
inline T RandomInt(const T& min, const T& max)
{
	static std::default_random_engine e((unsigned int)time(nullptr));
	std::uniform_int_distribution<T> u(min, max);
	return u(e);
}

typedef long long HandleT;

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

template <typename T>
void swap(T& x, T& y)
{
	auto temp = x;
	x = y;
	y = temp;
}