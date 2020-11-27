#pragma once

enum { WaterVAO, WaterMeshVAO, WaterNumVAO };
enum { WaterArrayBuffer, WaterElementBuffer, WaterMeshElementBuffer, WaterNumBuffer };
enum {};

class WaterPlane {
public:
	GLuint VAOs[WaterNumVAO];
	GLuint Buffers[WaterNumBuffer];
	GLuint Program;
	GLuint ProgramMesh;
	GLuint gBufferProgram;
	GLuint numTri;
	//GLuint xNum;
	//GLuint 

public:
	WaterPlane() {}
	WaterPlane(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2, GLfloat y, GLuint xDivNum, GLuint zDivNum) : numTri(xDivNum* zDivNum * 2) {
		GLfloat* point = new GLfloat[(xDivNum + 1) * (zDivNum + 1) * 3];
		GLuint* index = new GLuint[3 * 2 * xDivNum * zDivNum];
		GLuint* indexLine = new GLuint[3 * 2 * xDivNum * zDivNum];

		for (int i = 0; i <= xDivNum; i++)
			for (int j = 0; j <= zDivNum; j++) {
				point[(i + j * (xDivNum + 1)) * 3 + 0] = (x2 - x1) * i / xDivNum + x1;
				point[(i + j * (xDivNum + 1)) * 3 + 1] = y;
				point[(i + j * (xDivNum + 1)) * 3 + 2] = (z2 - z1) * j / zDivNum + z1;
				//printf("point %d:( %.3f , %.3f , %.3f )\n", i + j * (xDivNum + 1), point[(i + j * (xDivNum + 1)) * 3 + 0], point[(i + j * (xDivNum + 1)) * 3 + 1], point[(i + j * (xDivNum + 1)) * 3 + 2]);
			}

		for (int i = 0; i < xDivNum; i++)
			for (int j = 0; j < zDivNum; j++) {
				index[(i + j * xDivNum) * 6 + 0] = i + j * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 1] = i + (j + 1) * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 2] = i + 1 + j * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 3] = i + 1 + j * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 4] = i + (j + 1) * (xDivNum + 1);
				index[(i + j * xDivNum) * 6 + 5] = i + 1 + (j + 1) * (xDivNum + 1);
				//printf("index %d:( %d , %d , %d )\nindex %d:( %d , %d , %d )\n", 
				//	(i + j * xDivNum) * 2, index[(i + j * xDivNum) * 6 + 0], index[(i + j * xDivNum) * 6 + 1], index[(i + j * xDivNum) * 6 + 2],
				//	(i + j * xDivNum) * 2 + 1, index[(i + j * xDivNum) * 6 + 3], index[(i + j * xDivNum) * 6 + 4], index[(i + j * xDivNum) * 6 + 5]);
			}

		for (int i = 0; i < xDivNum; i++)
			for (int j = 0; j < zDivNum; j++) {
				indexLine[(i + j * xDivNum) * 6 + 0] = i + 1 + (j + 1) * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 1] = i + 1 + j * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 2] = i + 1 + (j + 1) * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 3] = i + (j + 1) * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 4] = i + 1 + (j + 1) * (xDivNum + 1);
				indexLine[(i + j * xDivNum) * 6 + 5] = i + j * (xDivNum + 1);
			}

		glGenVertexArrays(WaterNumVAO, VAOs);
		glCreateBuffers(WaterNumBuffer, Buffers);
		
		glNamedBufferStorage(Buffers[WaterArrayBuffer], (xDivNum + 1) * (zDivNum + 1) * 3 * sizeof(GLfloat), point, 0);
		glNamedBufferStorage(Buffers[WaterElementBuffer], 3 * 2 * xDivNum * zDivNum * sizeof(GLuint), index, 0);
		glNamedBufferStorage(Buffers[WaterMeshElementBuffer], 3 * 2 * xDivNum * zDivNum * sizeof(GLuint), indexLine, 0);

		glBindVertexArray(VAOs[WaterVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[WaterArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[WaterElementBuffer]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);

		Program = glCreateProgram();
		ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER,"waterV.shader"} ,
		{GL_FRAGMENT_SHADER,"waterF.shader"} ,
		{GL_NONE,""} };
		for (int i = 0; shaders[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shaders[i]);
			glAttachShader(Program, shader);
		}
		glLinkProgram(Program);

		glBindVertexArray(VAOs[WaterMeshVAO]);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[WaterArrayBuffer]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[WaterMeshElementBuffer]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);

		ProgramMesh = glCreateProgram();
		ShaderInfo shadersMesh[] = {
		{GL_VERTEX_SHADER,"gBufferWaterV.shader"} ,
		{GL_FRAGMENT_SHADER,"WaterMeshF.shader"} ,
		{GL_NONE,""} };
		for (int i = 0; shadersMesh[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shadersMesh[i]);
			glAttachShader(ProgramMesh, shader);
		}
		glLinkProgram(ProgramMesh);

		gBufferProgram = glCreateProgram();
		ShaderInfo shadersGBuffer[] = {
		{GL_VERTEX_SHADER,"gBufferWaterV.shader"} ,
		{GL_FRAGMENT_SHADER,"gBufferWaterF.shader"} ,
		{GL_NONE,""} };
		for (int i = 0; shadersGBuffer[i].mode != GL_NONE; i++) {
			GLuint shader = LoadShader(shadersGBuffer[i]);
			glAttachShader(gBufferProgram, shader);
		}
		glLinkProgram(gBufferProgram);
	}

	void gBufferRender(glm::mat4 V, glm::mat4 P, float time) {
		glUseProgram(gBufferProgram);

		GLint location;
		location = glGetUniformLocation(gBufferProgram, "V");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
		location = glGetUniformLocation(gBufferProgram, "P");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));
		location = glGetUniformLocation(gBufferProgram, "time");
		glUniform1f(location, time);
		glBindVertexArray(this->VAOs[WaterVAO]);
		glDrawElements(GL_TRIANGLES, numTri * 3, GL_UNSIGNED_INT, 0);

		//glUseProgram(ProgramMesh);
		//location = glGetUniformLocation(ProgramMesh, "V");
		//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
		//location = glGetUniformLocation(ProgramMesh, "P");
		//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));
		//location = glGetUniformLocation(ProgramMesh, "time");
		//glUniform1f(location, time);
		//glBindVertexArray(this->VAOs[WaterMeshVAO]);
		//glColor3f(0, 0, 0);
		//glLineWidth(1);
		//glDrawElements(GL_LINES, numTri * 3, GL_UNSIGNED_INT, 0);
	}
	void shadowBufferRender(GLuint program, glm::mat4 V, glm::mat4 P, float time) {
		GLint location;
		glUseProgram(program);

		location = glGetUniformLocation(program, "M");
		//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(this->modelMat));
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		location = glGetUniformLocation(program, "V");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
		location = glGetUniformLocation(program, "P");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));
		location = glGetUniformLocation(program, "time");
		glUniform1f(location, time);
		glBindVertexArray(this->VAOs[WaterVAO]);
		glDrawElements(GL_TRIANGLES, numTri * 3, GL_UNSIGNED_INT, 0);

		//glUseProgram(ProgramMesh);
		//location = glGetUniformLocation(ProgramMesh, "V");
		//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
		//location = glGetUniformLocation(ProgramMesh, "P");
		//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));
		//location = glGetUniformLocation(ProgramMesh, "time");
		//glUniform1f(location, time);
		//glBindVertexArray(this->VAOs[WaterMeshVAO]);
		//glColor3f(0, 0, 0);
		//glLineWidth(1);
		//glDrawElements(GL_LINES, numTri * 3, GL_UNSIGNED_INT, 0);
	}
	void oitRender(GLuint program, glm::mat4 V, glm::mat4 P, float time) {
		glUseProgram(program);

		GLint location;
		location = glGetUniformLocation(program, "V");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
		location = glGetUniformLocation(program, "P");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));
		location = glGetUniformLocation(program, "time");
		glUniform1f(location, time);
		glBindVertexArray(this->VAOs[WaterVAO]);
		glDrawElements(GL_TRIANGLES, numTri * 3, GL_UNSIGNED_INT, 0);
	}

	void render(glm::mat4 V, glm::mat4 P, float time) {
		glUseProgram(Program);

		GLint location;
		location = glGetUniformLocation(Program, "V");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
		location = glGetUniformLocation(Program, "P");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));
		location = glGetUniformLocation(Program, "time");
		glUniform1f(location, time);
		glBindVertexArray(this->VAOs[WaterVAO]);
		glDrawElements(GL_TRIANGLES, numTri*3, GL_UNSIGNED_INT, 0);

		glUseProgram(ProgramMesh);
		location = glGetUniformLocation(ProgramMesh, "V");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
		location = glGetUniformLocation(ProgramMesh, "P");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));
		location = glGetUniformLocation(ProgramMesh, "time");
		glUniform1f(location, time);
		glBindVertexArray(this->VAOs[WaterMeshVAO]);
		glColor3f(0,0,0);
		glLineWidth(1);
		glDrawElements(GL_LINES, numTri * 3, GL_UNSIGNED_INT, 0);
	}

};