#include "scene_particle_fountain.h"

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

Scene_Particle_Fountain::Scene_Particle_Fountain() : time(0), particleLifetime(5.5f), nParticles(8000),
									 emitterPos(1, 0, 0), emitterDir(-1, 2, 0)
{
}

void Scene_Particle_Fountain::initScene()
{
	compile();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Enable alpha blending
	glEnable(GL_BLEND);

	prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
	prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
	angle = glm::half_pi<float>();
}

void Scene_Particle_Fountain::compile()
{
	try {
		prog.compileShader("shader/wave_anim_shader.vert");
		prog.compileShader("shader/wave_anim_shader.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Particle_Fountain::update( float t )
{
	time = t;
}

void Scene_Particle_Fountain::render()
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

void Scene_Particle_Fountain::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

void Scene_Particle_Fountain::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix",
		glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}