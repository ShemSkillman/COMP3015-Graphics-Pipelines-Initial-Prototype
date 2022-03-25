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
	crateTwo = ObjMesh::load("media/prototype/crate2.obj", false, false);
	stones = ObjMesh::load("media/prototype/pebbles.obj", false, false);
	potOne = ObjMesh::load("media/prototype/pot1.obj", false, false);
	potTwo = ObjMesh::load("media/prototype/pot2.obj", false, false);
	tentSheets = ObjMesh::load("media/prototype/tent_sheets.obj", false, false);
	curtains = ObjMesh::load("media/prototype/curtains.obj", false, false);
	ribbon = ObjMesh::load("media/prototype/ribbon.obj", false, false);
	entrancePoles = ObjMesh::load("media/prototype/entrance_poles.obj", false, false);
	hatch = ObjMesh::load("media/prototype/hatch.obj", false, false);
	ceramics = ObjMesh::load("media/prototype/ceramics.obj", false, false);
	tentPegs = ObjMesh::load("media/prototype/tent_pegs.obj", false, false);
	carpet = ObjMesh::load("media/prototype/carpet.obj", false, false);
}

void Scene_Initial_Prototype::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	projection = mat4(1.0f);

	GLuint woodOneTexID =
		Texture::loadTexture("media/prototype/wood.jpg");

	GLuint woodTwoTexID =
		Texture::loadTexture("media/prototype/wood2.jpg");

	GLuint stoneOneTexID =
		Texture::loadTexture("media/prototype/stone2.jpg");

	GLuint potOneTexID =
		Texture::loadTexture("media/prototype/pot_tex_1.jpg");

	GLuint potTwoTexID =
		Texture::loadTexture("media/prototype/pot_tex_2.jpg");

	GLuint tentSheetsTexID =
		Texture::loadTexture("media/prototype/tent2.jpg");

	GLuint curtainsTexID =
		Texture::loadTexture("media/prototype/tent1.jpg");

	GLuint whiteChinaTexID =
		Texture::loadTexture("media/prototype/white_china.jpg");

	GLuint metalTexID =
		Texture::loadTexture("media/prototype/metal.jpg");

	GLuint carpetTexID =
		Texture::loadTexture("media/prototype/carpet.jpg");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodOneTexID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, woodTwoTexID);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, stoneOneTexID);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, potOneTexID);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, potTwoTexID);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, tentSheetsTexID);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, curtainsTexID);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, whiteChinaTexID);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, metalTexID);

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, carpetTexID);

	prog.setUniform("DirLight.Direction", vec4(0.8f, 0.5f, 1.0f, 1.0f));
	prog.setUniform("DirLight.La", vec3(0.4f));
	prog.setUniform("DirLight.L", vec3(0.8f));

	view = glm::lookAt(vec3(70, 40, 70),
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

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
	prog.setUniform("Material.Shininess", 180.0f);

	model = mat4(1.0f);
	setMatrices();

	prog.setUniform("RenderTex", 0);

	crateOne->render();
	crateTwo->render();

	prog.setUniform("RenderTex", 1);

	bigTable->render();
	stoolOne->render();
	stoolTwo->render();
	entrancePoles->render();

	prog.setUniform("RenderTex", 2);

	stones->render();

	prog.setUniform("RenderTex", 3);
	potOne->render();

	prog.setUniform("RenderTex", 4);
	potTwo->render();

	prog.setUniform("RenderTex", 5);
	tentSheets->render();
	hatch->render();

	prog.setUniform("RenderTex", 6);
	curtains->render();

	ribbon->render();

	prog.setUniform("RenderTex", 7);
	ceramics->render();

	prog.setUniform("RenderTex", 8);
	tentPegs->render();

	prog.setUniform("RenderTex", 9);
	carpet->render();
}

void Scene_Initial_Prototype::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(60.0f), (float)w / h,
		0.3f, 200.0f);

}

void Scene_Initial_Prototype::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
		vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}