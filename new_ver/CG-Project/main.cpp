#include "Scene.h"
#include "Interaction.h"

constexpr bool UseOITFlag = true;

GLFWwindow* Init();
void RenderShadow();
void RenderGBuffer();
void RenderOITBufferTM();
void RenderBlendOIT();
void RenderGBufferLight();

void RenderAllObject(const ViewPassEnum & pass);

void UpdateTakeoffCG() {
	static int tick = 0;
	static const int tickPerSec = 60;
	// percentage of CG displayed
	// begin = 0.0, finished = 1.0
	GLfloat progress = GLfloat(tick) / (10.0f * tickPerSec);
	if (progress > 1.0) {
		tick = 0;
		Interaction::displayTakeOffCGFlag = false;
		airplane->setPower(1.0);
		airplane->setVelocity(airplane->localFront * 10.0f);
		return;
	}
	tick++;
	
	// initialize
	if (tick == 1) {
		Interaction::camera.SetWorldUpDir({ 0, 1, 0 });
		glm::vec3 cameraPos =
			airplane->getPosition() +
			airplane->getFrontDir() * 200.0f
			- airplane->getLeftDir() * 100.0f;
		if (cameraPos.y < 10.0f + 1.0f) cameraPos.y = 10.0f + 1.0f;
		Interaction::camera.SetPosition(cameraPos);
	}

	// update airplane
	if(progress > 0.2f && progress < 0.7f)
		airplane->changePitch(0.2f * progress);
	else if (progress > 0.7f) {
		airplane->changePitch(-0.2f * (1.0f -progress));
	}
	airplane->translate(airplane->localFront * 20.0f * (progress));

	// update camera
	glm::vec3 cameraPos = Interaction::camera.GetViewPosition();
	if (progress < 0.3f) {
		cameraPos += airplane->getLeftDir() * 0.3f;
		Interaction::camera.SetPosition(cameraPos);
	}
	else if (progress < 0.5f) {
		cameraPos += airplane->getFrontDir();
		Interaction::camera.SetPosition(cameraPos);
	}
	else {
		static const GLfloat followRate = 0.10f;
		glm::vec3 targetPos = airplane->getPosition() - airplane->getFrontDir() * 100.0f;
		cameraPos = followRate * targetPos + (1 - followRate) * cameraPos;
		Interaction::camera.SetPosition(cameraPos);
	}
	Interaction::camera.SetFrontDir(airplane->getPosition() - Interaction::camera.GetViewPosition());
}

void UpdateCrashCG(glm::vec3 targetPos) {
	static int tick = 0;
	static const int tickPerSec = 60;
	static glm::vec3 lookat = targetPos;
	
	// percentage of CG displayed
	// begin = 0.0, finished = 1.0
	GLfloat progress = GLfloat(tick) / (20.0f * tickPerSec);
	if (progress > 1.0) {
		// game end
		tick = 0;
		airplane->setPower(0.0f);
		airplane->setVelocity({ 0, 0, 0 });
		Interaction::displayCrashCGFlag = false;
		Interaction::displayRestartCGFlag = true;
		Scene::desertScene->Idle();
		Scene::planeGameScene->Idle();
		Scene::player->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
		Scene::player->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		airplane->setPower(0);
		airplane->setVelocity({ 0, 0, 0 });
		airplane->setPosition({ 0, 40, 0 });
		airplane->resetDir();
		return;
	}
	tick++;
	
	// initialize
	if (tick == 1) {
		airplane->setPosition(targetPos + glm::vec3{ 0, 1000, 1000 });
	}

	// update airplane
	glm::vec3 currentPos = airplane->getPosition();
	glm::vec3 delta1 = glm::normalize(targetPos - currentPos) * 2.7f * progress;
	glm::vec3 delta2 = glm::vec3({sin(tick / 30.0f) , 0, cos(tick / 30.0f)}) * 5.0f * (1 - progress);
	airplane->changePitch(-5.0f * (0.2f + 0.8f * progress));
	airplane->changeRoll(5.0f * (0.5f + 0.5f * progress));
	airplane->setPosition(currentPos + delta1 + delta2);
	
	// shake the plane
	airplane->translate(
		glm::vec3({ RandomReal(-1.0, 1.0),
		RandomReal(-1.0, 1.0),
		RandomReal(-0.1, 0.1)
			}) * 5.0f * progress);
	
	// update camera
	if (tick % (2 * tickPerSec) == 0) {
		glm::vec3 cameraPos = airplane->getPosition();
		cameraPos += glm::vec3(RandomReal(-1.0, 1.0), RandomReal(-0.1, 0.1), RandomReal(-1.0, 1.0)) * 500.0f * (1 - progress);
		Interaction::camera.SetPosition(cameraPos);
		Interaction::camera.SetWorldUpDir({ 0, 1, 0 });
		lookat = airplane->getPosition();
	}
	lookat += delta1 + delta2 * progress; // soft follow
	Interaction::camera.SetFrontDir(lookat - Interaction::camera.GetViewPosition());
}

void UpdateRestartCG() {
	static int tick = 0;
	static const int tickPerSec = 60;
	static const glm::vec3 targetPos = { 0, 10, 0 };
	// percentage of CG displayed
	// begin = 0.0, finished = 1.0
	GLfloat progress = GLfloat(tick) / (10.0f * tickPerSec);
	if (progress > 1.0) {
		tick = 0;
		Interaction::displayRestartCGFlag = false;
		return;
	}
	tick++;
	// initialize
	if (tick == 1) {
		Interaction::camera.SetPosition(airplane->getPosition() + glm::vec3{ 20, -5, 20 });
	}
	// update airplane
	glm::vec3 airplanePos = airplane->getPosition();
	glm::vec3 delta = { 0, 20.0f, 0 };
	if (progress < 0.05f) /*do nothing*/;
	else if (progress < 0.2f) airplane->setPosition(airplanePos + delta);
	else airplane->setPosition(targetPos + glm::vec3(10, 0, 10));

	// update camera
	glm::vec3 cameraPos = Interaction::camera.GetViewPosition();
	if (progress < 0.2f) {
		Interaction::camera.SetFrontDir(airplanePos - cameraPos, true);
	}
	else if (progress < 0.6f) {
		Interaction::camera.SetFrontDir(targetPos - cameraPos);
		glm::vec3 cameraDelta = { 
			(targetPos.x - cameraPos.x) * 0.01f,
			(0.6f - progress) * 50.0f,
			(targetPos.z - cameraPos.z) * 0.01f};
		Interaction::camera.SetPosition(cameraPos + cameraDelta);
	}
	else{
		Interaction::camera.SetFrontDir(targetPos - cameraPos);
		glm::vec3 cameraDelta = {
			(targetPos.x - cameraPos.x) * 0.01f,
			(0.6f - progress) * 50.0f,
			(targetPos.z - cameraPos.z) * 0.01f };
		Interaction::camera.SetPosition(cameraPos + cameraDelta);
	}
}

void UpdateCamera() {
	if (!Interaction::operationMode) {
		// follow ground player
		Interaction::camera.SetPosition(player->getPosition());
		Interaction::camera.SetFrontDir(player->getViewDir(), true);
		Interaction::camera.SetWorldUpDir({0, 1, 0});
	}
	else {
		// follow airplane
		// locate camera at the back of the airplane
		glm::vec3 objFront = airplane->getFrontDir();
		glm::vec3 objPos = airplane->getPosition();
		glm::vec3 cameraDelta = objFront * (-100.0f) + glm::vec3({ 0, 1, 0 });
		Interaction::camera.SetPosition(objPos + cameraDelta);
		if (!(Interaction::left_button_pressed)) {
			// force camera to look at the airplane
			Interaction::camera.SetFrontDir(objFront);
			Interaction::camera.SetWorldUpDir(airplane->getUpDir());
		}
		//std::cout << "camera front"; printVec(objFront);
		//std::cout << "camera pos"; printVec(camera.GetViewPosition());
	}
}

void UpdateAirplane() {
	if (!Interaction::operationMode) {
		// control ground player
		player->rotate(-Interaction::ReadXoffset() * 2.25f, { 0, 1, 0 });
		player->changePitch(-Interaction::ReadYoffset() * 2.25f);
		glm::vec3 front = {0, 0, 1};
		glm::vec3 left = {1, 0, 0};
		if (Interaction::key_w_pressed) {
			player->move(front);
		}
		if (Interaction::key_s_pressed) {
			player->move(-front);
		}
		if (Interaction::key_a_pressed) {
			player->move(left);
		}
		if (Interaction::key_d_pressed) {
			player->move(-left);
		}
		if (Interaction::key_o_pressed) {
			player->saveToObj("../resources/out.obj");
		}
	}
	else {
		// control airplane
		GLfloat deltaPower = 0.01f;
		GLfloat deltaYaw = 0.5f;
		airplane->changePitch(Interaction::ReadYoffset());
		airplane->changeRoll(Interaction::ReadXoffset());
		if (Interaction::key_w_pressed) {
			airplane->changePower(deltaPower);
		}
		if (Interaction::key_s_pressed) {
			airplane->changePower(-deltaPower);
		}
		if (Interaction::key_a_pressed) {
			airplane->changeYaw(deltaYaw);
		}
		if (Interaction::key_d_pressed) {
			airplane->changeYaw(-deltaYaw);
		}
		if (Interaction::key_space_pressed) {
			airplane->setPower(0);
			airplane->setVelocity({ 0, 0, 0 });
			airplane->setPosition({ 0, 40, 0 });
			airplane->resetDir();
		}

		if (Interaction::key_F2_pressed) {
			airplane->saveToObj("../resources/out.obj");
			Interaction::key_F2_pressed = false;
		}
		if (Interaction::key_F5_pressed) {
			Interaction::displayTakeOffCGFlag = !Interaction::displayTakeOffCGFlag;
			Interaction::key_F5_pressed = false;
		}
		if (Interaction::key_F6_pressed) {
			Interaction::displayCrashCGFlag = !Interaction::displayCrashCGFlag;
			Interaction::key_F6_pressed = false;
		}
		airplane->simulate();
	}
}

void UpdateLight()
{
	if (!Interaction::key_F3_pressed)
		return;
	constexpr GLfloat posStep = 0.01f;
	constexpr GLfloat colorStep = 0.01f;
	constexpr float e = 0.03f;
	static bool posDir = false;
	static glm::vec3 pos = glm::vec3(-1.0f, 1.0f, -1.0f);
	static bool colorRDir = false;
	static bool colorGDir = false;
	static bool colorBDir = false;
	static glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	if ((posDir == false && pos.x + posStep >= 1.00f) || (posDir == true && pos.x - posStep <= -1.00f))
		posDir = !posDir;
	if (posDir)
		pos = pos - glm::vec3(posStep, 0.0f, posStep);
	else
		pos = pos + glm::vec3(posStep, 0.0f, posStep);


	if ((colorRDir == false && color.r + colorStep >= 1.00f) || (colorRDir == true && color.r - colorStep <= -1.00f))
		colorRDir = !colorRDir;
	if ((colorGDir == false && color.g + colorStep * 2 >= 1.00f) || (colorGDir == true && color.g - colorStep * 2<= -1.00f))
		colorGDir = !colorGDir;
	if ((colorBDir == false && color.r + colorStep * 3 >= 1.00f) || (colorBDir == true && color.b - colorStep * 3<= -1.00f))
		colorBDir = !colorBDir;
	if (colorRDir)
		color.r -= colorStep;
	else
		color.r += colorStep;
	if (colorGDir)
		color.g -= colorStep * 2;
	else
		color.g += colorStep * 2;
	if (colorBDir)
		color.b -= colorStep * 3;
	else
		color.b += colorStep * 3;

	Scene::directionalLight.setPosition(normalize(pos) * 800.0f);
	Scene::directionalLight.setColor(color);
	//std::cout << "light pos = " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	std::cout << "light color = " << color.r << " " << color.g << " " << color.b << std::endl;

	GlobalDataPool::SetData<glm::mat4>("lightProjection",
		glm::ortho(Shadow::SHADOW_WIDTH * -0.5f * e, Shadow::SHADOW_WIDTH * 0.5f * e,
			Shadow::SHADOW_HEIGHT * -0.5f * e, Shadow::SHADOW_HEIGHT * 0.5f * e, 0.1f, 2000.0f));
	GlobalDataPool::SetData<glm::mat4>("lightView", glm::lookAt(directionalLight.LightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
}

void UpdateData()
{
	UpdateLight();
	// if the game is not paused
	if (Interaction::key_p_flag == false) {
		if (Interaction::displayTakeOffCGFlag)
			UpdateTakeoffCG();
		else if (Interaction::displayCrashCGFlag)
			UpdateCrashCG(glm::vec3(0.0f, 10.0f, -75.0f));
		else if (Interaction::displayRestartCGFlag)
			UpdateRestartCG();
		else {
			UpdateAirplane();
			UpdateCamera();
		}
	}
	GlobalDataPool::SetData<GLfloat>("systemTime", (GLfloat)glfwGetTime());
	GlobalDataPool::SetData<glm::vec3>("cameraFront", camera.GetViewFront());
	for (const auto& plane : fitPlaneGroup.GetObjectList())
	{
		//glm::vec3 uniObjPos = GlobalDataPool::GetData<glm::vec3>("uniObjPos");
		//glm::vec3 uniObjVel = GlobalDataPool::GetData<glm::vec3>("uniObjVel");
		//plane->UpdateHeight(radius, 0, uniObjPos, uniObjVel);
		glm::vec3 pos = GlobalDataPool::GetData<glm::vec3>("cameraPosition");
		//pos.y -= 5.0f;
		plane->UpdateHeight(radius, 0, pos);
	}

	if (Interaction::key_r_pressed)
	{
		Scene::desertScene->Play();
		Interaction::key_r_pressed = false;
	}
	if (Interaction::key_h_pressed)
	{
		Scene::desertScene->Hint();
		Interaction::key_h_pressed = false;
	}
	if (Interaction::key_q_pressed)
	{
		Scene::desertScene->Idle();
		Interaction::key_q_pressed = false;
	}
	desertScene->Update();
	planeGameScene->Update();
	if (Interaction::screenShotFlag)
	{
		try
		{
			time_t now = time(nullptr);
			struct tm info;
			localtime_s(&info, &now);
			char timeBuf[256];
			strftime(timeBuf, 256, "%Y-%m-%d_%H-%M-%S", &info);
			std::cout << "Time Now = " << timeBuf << std::endl;
			//std::string filename = std::string("IMG_") + "test" + ".jpg";
			std::string filename = std::string("IMG_") + timeBuf + ".jpg";
			ScreenShot(Scene::width, Scene::height, filename);
		}
		catch (std::string msg)
		{
			std::cout << msg << std::endl;
		}
		Interaction::screenShotFlag = false;
	}
}

int main(int argc, char** argv) {
	GLFWwindow* window = Init();

	while (!glfwWindowShouldClose(window)) {
		UpdateData();
		
		RenderShadow();
		RenderGBuffer();
		if (UseOITFlag)
		{
			RenderOITBufferTM();
			RenderBlendOIT();
		}
		RenderGBufferLight();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
GLuint gBufferShowProgram;
GLuint gBufferLightProgram;
GLFWwindow* Init() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "CG Project", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	InitGlobalData();
	if (UseOITFlag)
	{
		InitOIT();
	}
	InitShadow();
	InitGBuffer();
	InitScene();
	InitGameObject();

	gBufferShowProgram = GenerateProgram({
		{GL_VERTEX_SHADER,"GBufferShow.vert"} ,
		{GL_FRAGMENT_SHADER,"GBufferShow.frag"}
	});

	gBufferLightProgram = GenerateProgram({
		{ GL_VERTEX_SHADER,"GBufferLight.vert" },
		{ GL_FRAGMENT_SHADER,"GBufferLight.frag" }
	});

	glfwSetKeyCallback(window, Interaction::KeyCallback);
	glfwSetCursorPosCallback(window, Interaction::MouseCallback);
	glfwSetMouseButtonCallback(window, Interaction::MouseButtonCallback);
	glfwSetScrollCallback(window, Interaction::ScrollCallback);
	return window;
}

void RenderAllObject(const ViewPassEnum& pass)
{
	for (const auto& object : skyBoxGroup.GetObjectList())
	{
		object->Render(pass);
	}
	for (const auto& object : planeGroup.GetObjectList())
	{
		object->Render(pass);
	}
	for (const auto& object : transparentGroup.GetObjectList())
	{
		object->Render(pass);
	}
	for (const auto& object : fitPlaneGroup.GetObjectList())
	{
		object->Render(pass);
	}
	for (const auto& object : waterPlaneGroup.GetObjectList())
	{
		object->Render(pass);
	}

}

void RenderGBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffers[GBuffer]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderAllObject(ViewPassEnum::GBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RenderOITBufferTM() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glClearBufferfv(GL_COLOR, 0, bgColor); 
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, OITBuffers[HeadPointerInit]);
	glBindTexture(GL_TEXTURE_2D, OITTextures[HeadPointerTexture]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

	// Bind head-pointer image for read-write
	glBindImageTexture(0, OITTextures[HeadPointerTexture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
	// Bind linked-list buffer for write
	glBindImageTexture(1, OITTextures[StorageTexture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32UI);
	glBindImageTexture(2, OITTextures[StorageWorldPosTexture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glBindImageTexture(3, OITTextures[StorageNormalTexture], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, OITBuffers[AtomicCounter]);
	const GLuint zero = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);

	RenderAllObject(ViewPassEnum::OITBuffer);

	glEnable(GL_DEPTH_TEST);

}
unsigned int quadVAO = 0;
unsigned int quadVBO;
void RenderBlendOIT() {
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffers[GBuffer]);
	glUseProgram(OITPrograms[RankOIT]);
	GLuint location;
	location = glGetUniformLocation(OITPrograms[RankOIT], "gPosition");
	glUniform1i(location, GPositionTexture);
	location = glGetUniformLocation(OITPrograms[RankOIT], "gNormal");
	glUniform1i(location, GNormalTexture);
	location = glGetUniformLocation(OITPrograms[RankOIT], "gAlbedoSpec");
	glUniform1i(location, GAlbedoSpecTexture);
	location = glGetUniformLocation(OITPrograms[RankOIT], "gDepthID");
	glUniform1i(location, GDepthIDTexture);
	location = glGetUniformLocation(OITPrograms[RankOIT], "gTransColor");
	glUniform1i(location, GTransColorTexture);

	GLfloat uniNear = GlobalDataPool::GetData<GLfloat>("uniNear");
	location = glGetUniformLocation(OITPrograms[RankOIT], "uniNear");
	glUniform1f(location, uniNear);
	GLfloat uniFar = GlobalDataPool::GetData<GLfloat>("uniFar");
	location = glGetUniformLocation(OITPrograms[RankOIT], "uniFar");
	glUniform1f(location, uniFar);
	glm::vec3 uniViewPosition = GlobalDataPool::GetData<glm::vec3>("cameraPosition");
	location = glGetUniformLocation(OITPrograms[RankOIT], "uniViewPosition");
	glUniform3fv(location, 1, glm::value_ptr(uniViewPosition));
	//glm::vec3 uniViewFront = GlobalDataPool::GetData<glm::vec3>("cameraFront");
	//location = glGetUniformLocation(OITPrograms[RankOIT], "uniViewFront");
	//glUniform3fv(location, 1, glm::value_ptr(uniViewFront));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, GTextures[GTransColorTexture]);


	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	RenderAllObject(ViewPassEnum::BlendOIT);

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);


}

void RenderGBufferLight() {
	glm::mat4 projection = GlobalDataPool::GetData<glm::mat4>("cameraProjection");
	glm::mat4 lightProjection = GlobalDataPool::GetData<glm::mat4>("lightProjection");
	glm::mat4 lightView = GlobalDataPool::GetData<glm::mat4>("lightView");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (quadVAO == 0) {
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	glUseProgram(gBufferLightProgram);
	GLuint location;
	location = glGetUniformLocation(gBufferLightProgram, "gPosition");
	glUniform1i(location, GPositionTexture);
	location = glGetUniformLocation(gBufferLightProgram, "gNormal");
	glUniform1i(location, GNormalTexture);
	location = glGetUniformLocation(gBufferLightProgram, "gAlbedoSpec");
	glUniform1i(location, GAlbedoSpecTexture);
	location = glGetUniformLocation(gBufferLightProgram, "gDepthID");
	glUniform1i(location, GDepthIDTexture);
	location = glGetUniformLocation(gBufferLightProgram, "shadowMap");
	glUniform1i(location, 4);
	location = glGetUniformLocation(gBufferLightProgram, "gTransColor");
	glUniform1i(location, GTransColorTexture + 1);

	GLfloat uniNear = GlobalDataPool::GetData<GLfloat>("uniNear");
	location = glGetUniformLocation(gBufferLightProgram, "uniNear");
	glUniform1f(location, uniNear);
	GLfloat uniFar = GlobalDataPool::GetData<GLfloat>("uniFar");
	location = glGetUniformLocation(gBufferLightProgram, "uniFar");
	glUniform1f(location, uniFar);

	location = glGetUniformLocation(gBufferLightProgram, "lightSpaceMatrix");
	glm::mat4 temp = lightProjection * lightView;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(temp));
	location = glGetUniformLocation(gBufferLightProgram, "uniP");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
	location = glGetUniformLocation(gBufferLightProgram, "uniV");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GTextures[GPositionTexture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GTextures[GNormalTexture]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GTextures[GAlbedoSpecTexture]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GTextures[GDepthIDTexture]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, Shadow::shadowBuffers[Shadow::ShadowBuffer]);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, GTextures[GTransColorTexture]);


	SendAllLightUniformsToShader(gBufferLightProgram, directionalLight);


	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	RenderAllObject(ViewPassEnum::GBufferIlluminate);

	glBindVertexArray(0);

	glUseProgram(0);
	glBindVertexArray(0);
}

void RenderShadow() {
	using namespace Shadow;
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffers[ShadowFrameBuffer]);
	glClear(GL_DEPTH_BUFFER_BIT);

	RenderAllObject(ViewPassEnum::ShadowBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	glViewport(0, 0, width, height);
}
