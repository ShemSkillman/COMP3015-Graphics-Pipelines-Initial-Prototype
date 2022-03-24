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

Scene_Initial_Prototype::Scene_Initial_Prototype() : plane(100.0f, 100.0f, 1, 1), teapot(14, glm::mat4(1.0f)), torus(1.75f * 0.75f, 0.75f * 0.75f, 50, 50)
{
}

void Scene_Initial_Prototype::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,
		1.0f, 0.0f));
	projection = mat4(1.0f);

	prog.setUniform("DirLight.Direction", vec4(0.8f, 0.5f, 1.0f, 1.0f));
	prog.setUniform("DirLight.La", vec3(0.8f));
	prog.setUniform("DirLight.L", vec3(0.8f));

	tPrev = 0.0f;
	angle = 0.0f;

	prog.setUniform("Fog.MaxDist", 20.0f);
	prog.setUniform("Fog.MinDist", 10.0f);
	prog.setUniform("Fog.Color", vec3(0.7f));
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
}

void Scene_Initial_Prototype::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	prog.setUniform("Material.Ka", torusCol * 0.3f);
	prog.setUniform("Material.Shininess", 100.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(-1.0f, 0.75f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

	setMatrices();
	torus.render();

	vec3 cubeCol = vec3(0.2f, 0.55f, 0.9f);

	prog.setUniform("Material.Kd", cubeCol);
	prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
	prog.setUniform("Material.Ka", cubeCol * 0.3f);
	prog.setUniform("Material.Shininess", 100.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(3.0f, 1.75f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

	setMatrices();
	cube.render();

	vec3 planeCol = vec3(0.8);

	prog.setUniform("Material.Kd", planeCol);
	prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
	prog.setUniform("Material.Ka", planeCol * 0.2f);
	prog.setUniform("Material.Shininess", 180.0f);

	model = mat4(1.0f);

	setMatrices();
	plane.render();
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