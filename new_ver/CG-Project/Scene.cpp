#include "Scene.h"


namespace Scene
{
	//std::vector<TexturedPlane> plane;
	//std::vector<TransparentPlane> testTransPlane;
	ViewGroup<TexturedPlane> planeGroup;
	ViewGroup<TransparentPlane> transparentGroup;
	ViewGroup<FitTexturedPlane> fitPlaneGroup;
	ViewGroup<WaterPlane> waterPlaneGroup;
	ViewGroup<SkyBox> skyBoxGroup;

	std::shared_ptr<Airplane> airplane;
	std::shared_ptr<Player> player;
	std::shared_ptr<DesertScene> desertScene;
	std::vector<std::shared_ptr<GameObject>> GameObjects;

	int width = 1280;
	int height = 768;
	GLfloat groundSize = 15.0f;

	GLfloat ambientStrength = 0.1f;
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition(0.0f, 20.0f, 0.0f);
	GLfloat lightSize = 0.5f;
	//obj scene
	GLfloat radius = 2;


	//light scene
	//dir light
	DirectionalLight directionalLight = DirectionalLight(glm::vec3(-1.0, 1.0, -1.0));


	ViewCamera camera(glm::vec3(40.0f, 100.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	//glm::mat4 lightProjection = glm::ortho(SHADOW_WIDTH * -0.5f * e, SHADOW_WIDTH * 0.5f * e,
	//	SHADOW_HEIGHT * -0.5f * e, SHADOW_HEIGHT * 0.5f * e, 0.1f, 2000.0f);
	//glm::mat4 lightView = glm::lookAt(directionalLight.LightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	GLuint GBuffers[NumBuffer];
	GLuint GTextures[NumGTexture];

	int StroageSize = 2048 * 2048;
	GLuint OITBuffers[NumOITBuffer];
	GLuint OITTextures[NumOITTexture];
	GLuint OITPrograms[NumOITProgram];

	GLuint cubemapTexture;



}

void SendAllLightUniformsToShader(GLuint program, Light light)
 {
	 GLuint location;

	 location = glGetUniformLocation(program, "lightColor");
	 glUniform3fv(location, 1, glm::value_ptr(light.LightColor));
	 location = glGetUniformLocation(program, "light.ambient");
	 glUniform3fv(location, 1, glm::value_ptr(light.ambient));
	 location = glGetUniformLocation(program, "light.diffuse");
	 glUniform3fv(location, 1, glm::value_ptr(light.diffuse));
	 location = glGetUniformLocation(program, "light.specular");
	 glUniform3fv(location, 1, glm::value_ptr(light.specular));
	 location = glGetUniformLocation(program, "light.constant");
	 glUniform1f(location, light.constant);
	 location = glGetUniformLocation(program, "light.linear");
	 glUniform1f(location, light.linear);
	 location = glGetUniformLocation(program, "light.quadratic");
	 glUniform1f(location, light.quadratic);
	 location = glGetUniformLocation(program, "viewPos");
	 glUniform3fv(location, 1, glm::value_ptr(camera.GetViewPosition()));
	 location = glGetUniformLocation(program, "light.position");
	 glUniform3fv(location, 1, glm::value_ptr(light.LightPos));
}

void Scene::AddGroupObject(const std::shared_ptr<ViewObject>& ptr)
{
	ViewObjectEnum type = ptr->GetType();
	switch (type)
	{
	case ViewObjectEnum::TextureObject:
		planeGroup.AddObject(std::dynamic_pointer_cast<TexturedPlane>(ptr));
		break;
	case ViewObjectEnum::TransparentObject:
		transparentGroup.AddObject(std::dynamic_pointer_cast<TransparentPlane>(ptr));
		break;
	case ViewObjectEnum::FitTextureObject:
		fitPlaneGroup.AddObject(std::dynamic_pointer_cast<FitTexturedPlane>(ptr));
		break;
	case ViewObjectEnum::WaterObject:
		waterPlaneGroup.AddObject(std::dynamic_pointer_cast<WaterPlane>(ptr));
		break;
	case ViewObjectEnum::SkyBoxObject:
		skyBoxGroup.AddObject(std::dynamic_pointer_cast<SkyBox>(ptr));
		break;
	}
}

void Scene::InitScene() {

	//plane set
	GLfloat planePoints1[] = {
		-groundSize , 0.0f , -groundSize , 0.0f , 0.0f ,
		 groundSize , 0.0f , -groundSize , 1.0f , 0.0f ,
		-groundSize , 0.0f ,  groundSize , 0.0f , 1.0f ,
		 groundSize , 0.0f ,  groundSize , 1.0f , 1.0f };
	GLuint planeIndices1[] = {
		 0, 1, 2, // first triangle
		 1, 2, 3  // second triangle
	};
	ShaderInfo planeShaders[] = {
		{GL_VERTEX_SHADER,"Ground.vert"} ,
		{GL_FRAGMENT_SHADER,"Ground.frag"} ,
		{GL_NONE,""} };
	TextureInfo planeTextures[] = {
		{AmbientTexture,"Bricks036_2K-JPG/Bricks036_2K_Color.jpg"},
		{NormalTexture,"Bricks036_2K-JPG/Bricks036_2K_Normal.jpg"},
		{RoughnessTexture,"Bricks036_2K-JPG/Bricks036_2K_Roughness.jpg"},
		{Texture_NONE,""} };
	TextureInfo grassTextures[] = {
		{AmbientTexture,"Ground003_4K-JPG/Ground003_4K_Color.jpg"},
		{NormalTexture,"Ground003_4K-JPG/Ground003_4K_Normal.jpg"},
		{RoughnessTexture,"Ground003_4K-JPG/Ground003_4K_Roughness.jpg"},
		{Texture_NONE,""} };
	TextureInfo sandTextures[] = {
		{AmbientTexture,"Ground033_4K-JPG/Ground033_4K_Color.jpg"},
		{NormalTexture,"Ground033_4K-JPG/Ground033_4K_Normal.jpg"},
		{RoughnessTexture,"Ground033_4K-JPG/Ground033_4K_Roughness.jpg"},
		{Texture_NONE,""} };
	GLfloat planePoints2[] = {
	-groundSize , groundSize * 2 , -groundSize , 0.0f , 0.0f ,
	 groundSize , groundSize * 2 , -groundSize , 1.0f , 0.0f ,
	-groundSize , 0 , -groundSize , 0.0f , 1.0f ,
	 groundSize , 0 , -groundSize , 1.0f , 1.0f };
	GLuint planeIndices2[] = {
		 0, 1, 2, // first triangle
		 1, 2, 3  // second triangle
	};
	GLfloat planePoints3[] = {
	 -groundSize , 0 , -groundSize , 0.0f , 0.0f ,
	 -groundSize , 2 * groundSize , -groundSize , 0.0f , 1.0f ,
	 -groundSize , 0 ,  groundSize , 1.0f , 0.0f ,
	 -groundSize , 2 * groundSize ,  groundSize , 1.0f , 1.0f };
	GLuint planeIndices3[] = {
		 0, 1, 2, // first triangle
		 1, 2, 3  // second triangle
	};
	GLfloat planePoints4[] = {
	groundSize , groundSize * 3 / 2 , groundSize , 0.0f , 0.0f ,
	groundSize , groundSize * 3 / 2 , groundSize * 3 , 0.0f , 1.0f ,
	groundSize * 3, groundSize * 3 / 2 , groundSize , 1.0f, 0.0f ,
	groundSize * 3 , groundSize * 3 / 2 ,  groundSize * 3 , 1.0f , 1.0f 
	};

	GLfloat planePoints5[] = {
		-groundSize , groundSize * 3 / 2, groundSize  , 0.0f , 0.0f ,
		-groundSize , groundSize * 3 / 2, groundSize * 3 , 0.0f , 1.0f ,
		groundSize , groundSize * 3 / 2, groundSize , 1.0f, 0.0f ,
		groundSize , groundSize * 3 / 2,  groundSize * 3, 1.0f , 1.0f 
	};
	planeGroup.AddObject(std::shared_ptr<TexturedPlane>(new TexturedPlane(glm::vec3(0, 0, 0), planePoints1, sizeof(planePoints1),
		planeIndices1, sizeof(planeIndices1), planeShaders, planeTextures)));
	planeGroup.AddObject(std::shared_ptr<TexturedPlane>(new TexturedPlane(glm::vec3(0, 0, 0), planePoints2, sizeof(planePoints2),
		planeIndices2, sizeof(planeIndices2), planeShaders, planeTextures)));
	planeGroup.AddObject(std::shared_ptr<TexturedPlane>(new TexturedPlane(glm::vec3(0, 0, 0), planePoints3, sizeof(planePoints3),
		planeIndices3, sizeof(planeIndices3), planeShaders, planeTextures)));
	planeGroup.AddObject(std::shared_ptr<TexturedPlane>(new TexturedPlane(glm::vec3(0, 0, 0), planePoints4, sizeof(planePoints4),
		planeIndices3, sizeof(planeIndices3), planeShaders, grassTextures)));
	planeGroup.GetObjectList()[3]->renderGrassFlag = true;
	std::vector<GLfloat> planeVertice;
	for (int i = 0; i < 3 * 10; i++)
	{
		for (int j = 0; j < 5; j++)
			planeVertice.push_back(::RandomReal<GLfloat>(-groundSize, groundSize));
	}
	//planeGroup.AddObject(std::shared_ptr<TexturedPlane>(new TexturedPlane(planeVertice)));

	int data;
	glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &data); printf("%d\n", data);

	//auto fitPlane = std::shared_ptr<FitTexturedPlane>(new FitTexturedPlane(
	//	glm::vec3(0, 0, 0), planePoints5, sizeof(planePoints5), planeIndices3, sizeof(planeIndices3), planeShaders, sandTextures));
	//glm::vec3 uniObjPos = GlobalDataPool::GetData<glm::vec3>("uniObjPos");
	//glm::vec3 uniObjVel = GlobalDataPool::GetData<glm::vec3>("uniObjVel");
	//fitPlane->UpdateHeight(0, 1, uniObjPos, uniObjVel);
	//fitPlaneGroup.AddObject(fitPlane);



	GLfloat planePoints6[] = {
		-groundSize , groundSize * 3 , groundSize  , 0.0f , 1.0f , 0.0f,
		-groundSize , groundSize * 3 , groundSize * 3 , 0.0f , 1.0f , 0.0f,
		groundSize , groundSize * 3 , groundSize , 0.0f , 1.0f , 0.0f,
		groundSize , groundSize * 3 ,  groundSize * 3, 0.0f , 1.0f , 0.0f
	};

	float temp = 1.5;
	for (int i = 0; i < 6; i++) {
		float r = temp * temp + 0.3f; r = r - floor(r); temp = sin(temp * temp) + 1.3f;
		float g = temp * temp + 0.3f; g = g - floor(g); temp = cos(temp * temp) + 1.3f;
		float b = temp * temp + 0.3f; b = b - floor(b); temp = sin(temp * temp) + 1.3f;
		float a = temp * temp + 0.3f; a = a - floor(a); temp = cos(temp * temp) + 1.3f;
		auto ptr = std::shared_ptr<TransparentPlane>(new TransparentPlane(
			glm::vec3(0, i, 0), planePoints6, sizeof(planePoints6), planeIndices3, sizeof(planeIndices3), glm::vec4(r, g, b, a)));
		transparentGroup.AddObject(ptr);
	}

	std::vector<GLfloat> transTestVertice;
	std::vector<GLuint> transTestIndice;
	constexpr int transTestVerNum = 3;
	for (int i = 0; i < transTestVerNum; i++)
	{
		transTestVertice.push_back(::RandomReal<GLfloat>(-groundSize, groundSize));
		transTestVertice.push_back(::RandomReal<GLfloat>(-groundSize, groundSize) + groundSize * 3.5f);
		transTestVertice.push_back(::RandomReal<GLfloat>(-groundSize, groundSize) + groundSize * 1.5f);
		transTestVertice.push_back(0.0f);
		transTestVertice.push_back(1.0f);
		transTestVertice.push_back(0.0f);
		if (i > transTestVerNum - 3)
			continue;
		transTestIndice.push_back(i);
		transTestIndice.push_back(i + 1);
		transTestIndice.push_back(i + 2);
	}
	//for (const auto & it : transTestIndice)
	//	std::cout << it << " " ;
	//std::cout << std::endl << transTestVertice.size() << " " << sizeof(GLuint) << std::endl;
	auto transTestPolygon(std::shared_ptr<TransparentPlane>(new TransparentPlane(
		glm::vec3(0.0f, 0.0f, 0.0f), transTestVertice.data(), (GLint)transTestVertice.size() * sizeof(GLfloat),
		transTestIndice.data(), (GLint)transTestIndice.size() * sizeof(GLuint),
		glm::vec4(::RandomReal<GLfloat>(0.0f, 1.0f), ::RandomReal<GLfloat>(0.0f, 1.0f),
			::RandomReal<GLfloat>(0.0f, 1.0f), ::RandomReal<GLfloat>(0.0f, 1.0f))
	)));
	transparentGroup.AddObject(transTestPolygon);

	skyBoxGroup.AddObject(std::shared_ptr<SkyBox>(new SkyBox(1)));

	//light
	GLfloat lightPoints[] = {
		lightSize , lightSize, lightSize,
		lightSize , lightSize, -lightSize,
		lightSize , -lightSize, lightSize,
		lightSize , -lightSize, -lightSize,
		-lightSize , lightSize, lightSize,
		-lightSize , lightSize, -lightSize,
		-lightSize , -lightSize, lightSize,
		-lightSize , -lightSize, -lightSize };
	GLuint lightIndices[] = {
		0,1,4,	1,4,5,
		0,1,2,  1,2,3,
		4,5,6,  5,6,7,
		2,3,6,  3,6,7,
		0,2,4,  2,4,6 };
	//water 
	constexpr GLfloat waterPlaneSizeRatio = 100.0;
	waterPlaneGroup.AddObject(std::shared_ptr<WaterPlane>(new WaterPlane(
		-groundSize * waterPlaneSizeRatio, -groundSize * waterPlaneSizeRatio, groundSize * waterPlaneSizeRatio,
		groundSize * waterPlaneSizeRatio, groundSize, 2000, 2000)));
	


}

void Scene::InitGameObject()
{
	player = std::shared_ptr<Player>(new Player());
	player->loadFromObj("../resources/soldier.obj");
	player->setPosition({0, 20, 0});
	GameObject::allObjs.push_back(std::static_pointer_cast<GameObject>(player));
	airplane = std::shared_ptr<Airplane>(new Airplane({ 0, -1, 0 }, { 0, 0, 1 }));
	airplane->loadFromObj("../resources/jet.obj");
	airplane->scale({ 0.2, 0.2, 0.2 });
	airplane->translate({ 0, 200, 0 });
	airplane->rotate(-90, { 1, 0, 0 });
	GameObject::allObjs.push_back(std::static_pointer_cast<GameObject>(airplane));
	//GameObject::allObjs[0]->getRenderData()->renderGrassFlag = true;
	/*for (int i = 2; i <= 0; i++) {
		GameObject::allObjs.push_back(std::shared_ptr<GameObject>(new GameObject()));
		GameObject::allObjs[i]->loadFromObj("../resources/wolf.obj");
		GameObject::allObjs[i]->rotate(RandomReal(0.0f, 180.0f), {0.0f, 1.0f, 0.0f});
		GameObject::allObjs[i]->scale({ RandomReal(50.0f, 200.0f), RandomReal(50.0f, 200.0f), RandomReal(50.0f, 200.0f) });
		GameObject::allObjs[i]->setPosition({ RandomReal(-200.0f, 200.0f), RandomReal(100.0f, 200.0f), RandomReal(-200.0f, 200.0f) });
		GameObject::allObjs[i]->fixed = false;
	}*/
	for (const auto& i : GameObject::allObjs) {
		AddGroupObject(i->getRenderData());
	}
	desertScene.reset(new DesertScene);
}

void Scene::InitGlobalData()
{
	constexpr float e = 0.03f;

	GlobalDataPool::Add<glm::mat4>("cameraView", camera.GetViewMatrix());
	GlobalDataPool::Add<glm::mat4>("cameraProjection", glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 5000.0f));
	GlobalDataPool::Add<glm::vec3>("cameraPosition", camera.GetViewPosition());
	GlobalDataPool::Add<glm::vec3>("cameraFront", camera.GetViewFront());
	GlobalDataPool::Add<glm::mat4>("lightProjection",
		glm::ortho(Shadow::SHADOW_WIDTH * -0.5f * e, Shadow::SHADOW_WIDTH * 0.5f * e,
			Shadow::SHADOW_HEIGHT * -0.5f * e, Shadow::SHADOW_HEIGHT * 0.5f * e, 0.1f, 2000.0f));
	GlobalDataPool::Add<glm::mat4>("lightView", glm::lookAt(directionalLight.LightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	GlobalDataPool::Add<GLfloat>("systemTime", (float)glfwGetTime());
	GlobalDataPool::Add<glm::vec3>("uniObjPos", glm::vec3(0.0f, groundSize * 3 / 2, groundSize * 2));
	GlobalDataPool::Add<glm::vec3>("uniObjVel", glm::vec3(0.0f, 0.0f, 0.0f));
	GlobalDataPool::Add<GLfloat>("uniNear", 0.1f);
	GlobalDataPool::Add<GLfloat>("uniFar", 5000.0f);
}

void Scene::InitGBuffer() {
	glGenFramebuffers(NumBuffer, GBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffers[GBuffer]);

	glGenTextures(NumGTexture, GTextures);
	// - 位置颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GTextures[GPositionTexture], 0);

	// - 法线颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GTextures[GNormalTexture], 0);

	// - 颜色 + 镜面颜色缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture], 0);

	// - 深度缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, GTextures[GDepthIDTexture], 0);

	// - 半透明颜色透明度缓冲
	glBindTexture(GL_TEXTURE_2D, GTextures[GTransColorTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, GTextures[GTransColorTexture], 0);

	// - 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
	GLuint attachments[NumGTexture] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 ,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(NumGTexture, attachments);

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << " " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::InitOIT() {
	GLuint* data;

	glGenBuffers(NumOITBuffer, OITBuffers);
	glGenTextures(NumOITTexture, OITTextures);

	glBindTexture(GL_TEXTURE_2D, OITTextures[HeadPointerTexture]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindImageTexture(0, OITTextures[HeadPointerTexture], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

	constexpr int MemorySize = 12;
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, OITBuffers[HeadPointerInit]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	data = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	memset(data, 0x00, width * height * sizeof(GLuint));
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, OITBuffers[AtomicCounter]);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

	glBindBuffer(GL_TEXTURE_BUFFER, OITBuffers[Storage]);
	glBufferData(GL_TEXTURE_BUFFER, MemorySize * width * height * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	glBindTexture(GL_TEXTURE_BUFFER, OITTextures[StorageTexture]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, OITTextures[StorageTexture]);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	glBindBuffer(GL_TEXTURE_BUFFER, OITBuffers[StorageWorldPos]);
	glBufferData(GL_TEXTURE_BUFFER, MemorySize * width * height * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	glBindTexture(GL_TEXTURE_BUFFER, OITTextures[StorageWorldPosTexture]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, OITTextures[StorageWorldPosTexture]);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	glBindBuffer(GL_TEXTURE_BUFFER, OITBuffers[StorageNormal]);
	glBufferData(GL_TEXTURE_BUFFER, MemorySize * width * height * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	glBindTexture(GL_TEXTURE_BUFFER, OITTextures[StorageNormalTexture]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, OITTextures[StorageNormalTexture]);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	OITPrograms[WaterOIT] = GenerateProgram({
	{GL_VERTEX_SHADER,"GBufferWater.vert"} ,
	{GL_FRAGMENT_SHADER,"WaterOIT.frag"}
		});

	OITPrograms[PlaneOIT] = GenerateProgram({
	{GL_VERTEX_SHADER,"GBufferTrans.vert"} ,
	{GL_FRAGMENT_SHADER,"OIT.frag"} 
		});

	OITPrograms[RankOIT] = GenerateProgram({
		{GL_VERTEX_SHADER,"RankOIT.vert"} ,
		{GL_FRAGMENT_SHADER,"RankOIT.frag"} ,
	});

	OITPrograms[DisplayOIT] = GenerateProgram({
		{GL_VERTEX_SHADER,"DisplayOIT.vert"} ,
		{GL_FRAGMENT_SHADER,"DisplayOIT.frag"} ,
	});

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}


