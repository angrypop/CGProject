#pragma once
#include "ViewHeader.h"
#include "Shadow.h"
#include "Camera.h"
#include "Light.h"
#include "GlobalData.h"
#include "ViewGroup.h"
#include "GameObject.h"




//glm::mat4 view = glm::mat4(1.0f);
//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 500.0f);
namespace Scene
{
	extern ViewGroup<TexturedPlane> planeGroup;
	extern ViewGroup<TransparentPlane> transparentGroup;
	extern ViewGroup<FitTexturedPlane> fitPlaneGroup;
	extern ViewGroup<WaterPlane> waterPlaneGroup;
	extern ViewGroup<SkyBox> skyBoxGroup;

	extern std::shared_ptr<Airplane> airplane;
	// extern std::vector<std::shared_ptr<GameObject>> GameObjects;

	void InitGlobalData();
	void InitGameObject();
	void InitScene();
	void InitGBuffer();
	void InitOIT();

	extern DirectionalLight directionalLight;
	extern ViewCamera camera;

	extern int width;
	extern int height;
	extern GLfloat groundSize;

	extern GLfloat ambientStrength;
	extern glm::vec3 lightColor;
	extern glm::vec3 lightPosition;
	extern GLfloat lightSize;

	//obj scene
	extern GLfloat radius;

	enum { GBuffer, NumBuffer };
	enum { GPositionTexture, GNormalTexture, GAlbedoSpecTexture, GDepthIDTexture, GTransColorTexture, NumGTexture };
	extern GLuint GBuffers[NumBuffer];
	extern GLuint GTextures[NumGTexture];

	extern int StroageSize;
	enum { HeadPointerInit, Storage, StorageWorldPos, StorageNormal, AtomicCounter, NumOITBuffer };
	enum { HeadPointerTexture, StorageTexture, StorageWorldPosTexture, StorageNormalTexture, NumOITTexture };
	enum { WaterOIT, RankOIT, DisplayOIT, PlaneOIT, NumOITProgram };
	extern GLuint OITBuffers[NumOITBuffer];
	extern GLuint OITTextures[NumOITTexture];
	extern GLuint OITPrograms[NumOITProgram];

	extern GLuint cubemapTexture;

}
using namespace Scene;



void SendAllLightUniformsToShader(GLuint program, Light light);