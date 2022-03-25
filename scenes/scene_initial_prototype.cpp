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

Scene_Initial_Prototype::Scene_Initial_Prototype() : plane(100.0f, 100.0f, 1, 1), renderTex(0)
{
	crateOne = ObjMesh::load("media/prototype/crate.obj", false, false);
	bigTable = ObjMesh::load("media/prototype/big_table.obj", false, false);
	stoolOne = ObjMesh::load("media/prototype/stool_1.obj", false, false);
	stoolTwo = ObjMesh::load("media/prototype/stool_2.obj", false, false);
}

void Scene_Initial_Prototype::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	projection = mat4(1.0f);

	GLuint crateTexID =
		Texture::loadTexture("media/prototype/wood.jpg");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, crateTexID);

	prog.setUniform("DirLight.Direction", vec4(0.8f, 0.5f, 1.0f, 1.0f));
	prog.setUniform("DirLight.La", vec3(0.4f));
	prog.setUniform("DirLight.L", vec3(0.8f));

	view = glm::lookAt(vec3(60, 30, 60),
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	projection = glm::perspective(glm::radians(60.0f), (float)width / height,
		0.3f, 100.0f);
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
	prog.setUniform("RenderTex", 0);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
	prog.setUniform("Material.Shininess", 180.0f);

	model = mat4(1.0f);
	setMatrices();

	crateOne->render();
	bigTable->render();

	stoolOne->render();
	stoolTwo->render();
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