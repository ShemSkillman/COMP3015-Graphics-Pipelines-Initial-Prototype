#include "scene_normal_maps.h"

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

Scene_Normal_Maps::Scene_Normal_Maps()
{
	ogre = ObjMesh::load("media/bs_ears.obj", false, true);
}

void Scene_Normal_Maps::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(vec3(-1.0f, 0.25f, 1.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	prog.setUniform("Light.Position", view * glm::vec4(5.0f, 5.2f, 5.0f, 1.0f));
	prog.setUniform("Light.L", vec3(1.0f));
	prog.setUniform("Light.La", vec3(0.4f));

	//Load all textures first before binding to avoid strange results!

	GLuint ogreTexID =
		Texture::loadTexture("media/texture/ogre_diffuse.png");	

	GLuint ogreNormalMapID =
		Texture::loadTexture("media/texture/ogre_normalmap.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ogreTexID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ogreNormalMapID);
}

void Scene_Normal_Maps::compile()
{
	try {
		prog.compileShader("shader/normal_map_shader.vert");
		prog.compileShader("shader/normal_map_shader.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Normal_Maps::update( float t )
{
	model = mat4(1.0f);
	model = glm::rotate(model, glm::radians(50.0f * t), glm::vec3(0, 1, 0));
}

void Scene_Normal_Maps::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
	prog.setUniform("Material.Shininess", 180.0f);

	setMatrices();

	ogre->render();
}

void Scene_Normal_Maps::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h,
		0.3f, 100.0f);

}

void Scene_Normal_Maps::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
		vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}