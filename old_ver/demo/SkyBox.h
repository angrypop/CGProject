#pragma once

class SkyBox {
public:
	enum { VAO, NumVAO };
	enum { ArrayBuffer, ElementBuffer, NumBuffer };
	enum { position = 0, texCoord = 1 };

	GLuint VAOs[NumVAO];
	GLuint Buffers[NumBuffer];
	GLuint Program;
	GLuint GBufferProgram;
	GLuint GrassProgram;
	GLuint updateProgram;
	glm::mat4 modelMat;
	GLuint cubemapTexture;

public:
	SkyBox() {};
	SkyBox(int k) {
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		glGenVertexArrays(NumVAO, this->VAOs);
		glCreateBuffers(NumBuffer, this->Buffers);

		glNamedBufferStorage(this->Buffers[ArrayBuffer], sizeof(skyboxVertices), skyboxVertices, 0);

		glBindVertexArray(this->VAOs[VAO]);
		glBindBuffer(GL_ARRAY_BUFFER, this->Buffers[ArrayBuffer]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);

		ShaderInfo shadersGBuffer[] = {
		   {GL_VERTEX_SHADER,"SkyBoxV.shader"} ,
		   {GL_FRAGMENT_SHADER,"gBufferSkyBoxF.shader"} ,
		   {GL_NONE,""} };

		GBufferProgram = glCreateProgram();
		for (int i = 0; shadersGBuffer[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shadersGBuffer[i]);
			glAttachShader(GBufferProgram, shader);
		}
		glLinkProgram(GBufferProgram);

		cubemapTexture = loadCubemap(faces);
	}

	void translate(glm::mat4 translate) { this->modelMat = translate * modelMat; }

	void gBufferRender(glm::mat4 uniV, glm::mat4 uniP) {
		glUseProgram(GBufferProgram);

		GLint location;
		location = glGetUniformLocation(GBufferProgram, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMat));
		location = glGetUniformLocation(GBufferProgram, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(GBufferProgram, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		location = glGetUniformLocation(GBufferProgram, "skybox");
		glUniform1i(location, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glBindVertexArray(this->VAOs[VAO]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void oitRender(GLuint program, glm::mat4 uniV, glm::mat4 uniP) {
		glUseProgram(program);

		GLint location;
		location = glGetUniformLocation(program, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMat));
		location = glGetUniformLocation(program, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(program, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		glBindVertexArray(this->VAOs[VAO]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}


	void render(glm::mat4 uniV, glm::mat4 uniP) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);

		glUseProgram(GBufferProgram);

		GLint location;
		location = glGetUniformLocation(GBufferProgram, "uniM");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMat));
		location = glGetUniformLocation(GBufferProgram, "uniV");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniV));
		location = glGetUniformLocation(GBufferProgram, "uniP");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniP));
		location = glGetUniformLocation(GBufferProgram, "skybox");
		glUniform1i(location, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glBindVertexArray(this->VAOs[VAO]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

};