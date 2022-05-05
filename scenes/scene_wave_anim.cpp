#include "scene_wave_anim.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"
#include "texture.h"
#include <time.h>

using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::mat3;

Scene_Wave_Anim::Scene_Wave_Anim() : time(0), plane(13.0f, 10.0f, 200, 2)
{
}

void Scene_Wave_Anim::initScene()
{
	compile();

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
	angle = glm::half_pi<float>();
}

void Scene_Wave_Anim::compile()
{
	try {
		prog.compileShader("shader/point_sprites_shader.vert");
		prog.compileShader("shader/point_sprites_shader.frag");
		prog.compileShader("shader/point_sprites_shader.geom");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Wave_Anim::update( float t )
{
	time = t;
}

void Scene_Wave_Anim::render()
{
	prog.setUniform("Time", time);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = glm::lookAt(vec3(10.0f * cos(angle), 4.0f, 10.0f * sin(angle)),
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);

	prog.setUniform("Material.Kd", 0.2f, 0.5f, 0.9f);
	prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
	prog.setUniform("Material.Ka", 0.2f, 0.5f, 0.9f);
	prog.setUniform("Material.Shininess", 100.0f);
	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(-10.0f), vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(50.0f), vec3(1.0f, 0.0f, 0.0f));
	setMatrices();
	plane.render();
}

void Scene_Wave_Anim::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h,
		0.3f, 100.0f);

}

void Scene_Wave_Anim::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("ProjectionMatrix", projection);
}