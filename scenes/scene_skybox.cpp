#include "scene_skybox.h"

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

using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::mat3;

Scene_Skybox::Scene_Skybox() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f), sky(100.0f)
{
	tPrev = 0.0f;
	angle = 0.0f;
}

void Scene_Skybox::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);

	projection = mat4(1.0f);

	angle = glm::radians(90.0f); //set the initial angle

	//extract the cube texture
	GLuint cubeTex =
		Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");

	//activate and bindtexture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
}

void Scene_Skybox::compile()
{
	try {
		prog.compileShader("shader/skybox_shader.vert");
		prog.compileShader("shader/skybox_shader.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Skybox::update( float t )
{
	float deltaT = t - tPrev;
	if (tPrev == 0.0f)
		deltaT = 0.0f;

	tPrev = t;

	angle += rotSpeed * deltaT;

	if (angle > glm::two_pi<float>())
		angle -= glm::two_pi<float>();
}

void Scene_Skybox::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
	view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,0.0f));
	
	// Draw sky
	prog.use();
	model = mat4(1.0f);
	setMatrices();

	sky.render();
}

void Scene_Skybox::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h,
		0.3f, 100.0f);

}

void Scene_Skybox::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
		vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}