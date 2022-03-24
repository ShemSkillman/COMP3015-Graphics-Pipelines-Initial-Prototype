#include "scene_initial_prototype.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/texture.h"

using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::mat3;

Scene_Initial_Prototype::Scene_Initial_Prototype() : plane(100.0f, 100.0f, 1, 1), teapot(14, glm::mat4(1.0f)), torus(1.75f * 0.75f, 0.75f * 0.75f, 50, 50), 
	fsQuad(0), fboHandle(0), renderTex(0), angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f)
{
}

void Scene_Initial_Prototype::initScene()
{
	compile();

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	projection = mat4(1.0f);

	setupFBO();

	// Array for full-screen quad
	GLfloat verts[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f
	};

	GLfloat tc[] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f
	};

	// Set up the buffers

	unsigned int handle[2];
	glGenBuffers(2, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), tc, GL_STATIC_DRAW);

	// Set up the vertex array object

	glGenVertexArrays(1, &fsQuad);
	glBindVertexArray(fsQuad);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2); // Texture coordinates

	glBindVertexArray(0);

	prog.setUniform("EdgeThreshold", 0.01f);

	prog.setUniform("DirLight.Direction", vec4(0.8f, 0.5f, 1.0f, 1.0f));
	prog.setUniform("DirLight.La", vec3(0.8f));
	prog.setUniform("DirLight.L", vec3(0.8f));

	prog.setUniform("Fog.MaxDist", 20.0f);
	prog.setUniform("Fog.MinDist", 5.0f);
	prog.setUniform("Fog.Color", vec3(0.7f));
}

void Scene_Initial_Prototype::setupFBO()
{
	// Generate and bind the framebuffer
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	// Create the texture object

	glGenTextures(1, &renderTex);
	glBindTexture(GL_TEXTURE_2D, renderTex);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	// Bind the texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		renderTex, 0);

	// Create the depth buffer
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// Bind the depth buffer to the FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuf);

	// Set the targets for the fragment output variables
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	// Unbind the framebuffer, and revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene_Initial_Prototype::compile()
{
	try {
		prog.compileShader("shader/initial_prototype_shader.vert");
		prog.compileShader("shader/initial_prototype_shader.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Initial_Prototype::update( float t )
{
	float deltaT = t - tPrev;

	if (tPrev == 0.0f)
		deltaT = 0.0f;

	tPrev = t;

	angle += rotSpeed * deltaT;

	if (angle > glm::two_pi<float>())
		angle -= glm::two_pi<float>();
}

void Scene_Initial_Prototype::render()
{
	pass1();
	glFlush();
	pass2();
}

void Scene_Initial_Prototype::pass1()
{
	prog.setUniform("Pass", 1);

	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = glm::lookAt(vec3(7.0f * cos(angle), 4.0f, 7.0f * sin(angle)),
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(60.0f), (float)width / height,
		0.3f, 100.0f);

	vec3 teapotCol = vec3(0.2f, 0.55f, 0.9f);

	prog.setUniform("Material.Kd", teapotCol);
	prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
	prog.setUniform("Material.Ka", teapotCol * 0.3f);
	prog.setUniform("Material.Shininess", 100.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 0.0f, -2.0f));
	model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

	setMatrices();
	teapot.render();

	vec3 torusCol = vec3(0.95f, 0.69f, 0.4f);

	prog.setUniform("Material.Kd", torusCol);
	prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
	prog.setUniform("Material.Ka", torusCol);
	prog.setUniform("Material.Shininess", 100.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(-1.0f, 0.75f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

	setMatrices();
	torus.render();

	vec3 planeCol = vec3(0.8);

	prog.setUniform("Material.Kd", planeCol);
	prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
	prog.setUniform("Material.Ka", planeCol);
	prog.setUniform("Material.Shininess", 180.0f);

	model = mat4(1.0f);

	setMatrices();
	plane.render();
}

void Scene_Initial_Prototype::pass2()
{
	prog.setUniform("Pass", 2);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, renderTex);

	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	model = mat4(1.0f);
	view = mat4(1.0f);
	projection = mat4(1.0f);
	setMatrices();

	// Render the full-screen quad
	glBindVertexArray(fsQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Scene_Initial_Prototype::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h,
		0.3f, 100.0f);

}

void Scene_Initial_Prototype::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
		vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}