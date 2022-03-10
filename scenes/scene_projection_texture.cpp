#include "scene_projection_texture.h"

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

Scene_Projection_Texture::Scene_Projection_Texture() : angle(0.0f), tPrev(0.0f),
	rotSpeed(glm::pi<float>() / 8.0f),
	teapot(14, mat4(1.0f)),
	plane(100.0f, 100.0f, 1, 1)
{
	//
}


void Scene_Projection_Texture::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);
	projection = mat4(1.0f);
	angle = glm::radians(90.0f);
	//set up things for the projector matrix
	vec3 projPos = vec3(2.0f, 5.0f, 5.0f);
	vec3 projAt = vec3(-2.0f, -4.0f, 0.0f);
	vec3 projUp = vec3(0.0f, 1.0f, 0.0f);
	mat4 projView = glm::lookAt(projPos, projAt, projUp);
	mat4 projProj = glm::perspective(glm::radians(30.0f), 1.0f, 0.2f,
		1000.0f);
	mat4 bias = glm::translate(mat4(1.0f), vec3(0.5f));
	bias = glm::scale(bias, vec3(0.5f));
	prog.setUniform("ProjectorMatrix", bias * projProj * projView);

	// Load texture file
	GLuint flowerTex =
		Texture::loadTexture("media/texture/flower.png");
	//set up and send the projected texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, flowerTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	prog.setUniform("Light.La", vec3(0.2f));
	prog.setUniform("Light.L", vec3(0.6f));
}

void Scene_Projection_Texture::compile()
{
	try {
		prog.compileShader("shader/projection_texture_shader.vert");
		prog.compileShader("shader/projection_texture_shader.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Projection_Texture::update( float t )
{
	float deltaT = t - tPrev;
	if (tPrev == 0.0f)
		deltaT = 0.0f;
	tPrev = t;
	angle += rotSpeed * deltaT;
	if (angle > glm::two_pi<float>())
		angle -= glm::two_pi<float>();
}

void Scene_Projection_Texture::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set up your camera
	vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
	view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
		0.0f));

	//set up your material uniforms
	prog.setUniform("Material.Kd", 0.5f, 0.2f, 0.1f);
	prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
	prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
	prog.setUniform("Material.Shininess", 100.0f);

	//set up and render the teapot
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f,
		0.0f));
	setMatrices();
	teapot.render();

	//set up the material uniforms for plane and render the plane
	prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
	prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
	prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
	prog.setUniform("Material.Shininess", 1.0f);
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -0.75f, 0.0f));
	setMatrices();
	plane.render();

}

void Scene_Projection_Texture::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h,
		0.3f, 100.0f);

}

void Scene_Projection_Texture::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelMatrix", model);
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]),
		vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}