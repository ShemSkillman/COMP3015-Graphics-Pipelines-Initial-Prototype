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

Scene_Initial_Prototype::Scene_Initial_Prototype(): plane(150.0f, 150.0f, 1, 1), angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f), changeColSpeed(0.1f)
{
	lightColOne = vec3(0.4, 0.7f, 0.85f);
	lightColTwo = vec3(1.0, 0.95f, 0.9f);

	currentCol = lightColTwo;
	targetCol = lightColOne;

	importModels();
}

void Scene_Initial_Prototype::importModels()
{
	crates = ObjMesh::load("media/prototype/crate.obj", false, false);
	bigTable = ObjMesh::load("media/prototype/big_table.obj", false, false);
	stoolOne = ObjMesh::load("media/prototype/stool_1.obj", false, false);
	stoolTwo = ObjMesh::load("media/prototype/stool_2.obj", false, false);
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
	tentPole = ObjMesh::load("media/prototype/tent_pole.obj", false, false);
	stonesTwo = ObjMesh::load("media/prototype/pebbles2.obj", false, false);
	rope = ObjMesh::load("media/prototype/rope.obj", false, false);
}

void Scene_Initial_Prototype::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	progTexture.use();

	progTexture.setUniform("DirLight.Direction", vec4(0.8f, 0.2f, 1.0f, 1.0f));

	progTexture.setUniform("Fog.MaxDist", 150.0f);
	progTexture.setUniform("Fog.MinDist", 1.0f);
	progTexture.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));

	progNormals.use();	

	progNormals.setUniform("DirLight.Direction", vec4(0.8f, 0.2f, 1.0f, 1.0f));

	progNormals.setUniform("Fog.MaxDist", 150.0f);
	progNormals.setUniform("Fog.MinDist", 50.0f);
	progNormals.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));

	projection = glm::perspective(glm::radians(90.0f), (float)width / height,
		0.3f, 200.0f);

	GLuint woodOneTexID =
		Texture::loadTexture("media/prototype/wood.jpg");

	GLuint woodTwoTexID =
		Texture::loadTexture("media/prototype/wood2.jpg");

	GLuint stoneOneTexID =
		Texture::loadTexture("media/prototype/stone.jpg");

	GLuint potOneTexID =
		Texture::loadTexture("media/prototype/pot_tex_1.jpg");

	GLuint potTwoTexID =
		Texture::loadTexture("media/prototype/pot_tex_2.jpg");

	GLuint tentSheetsTexID =
		Texture::loadTexture("media/prototype/tent2.jpg");

	GLuint curtainsTexID =
		Texture::loadTexture("media/prototype/orange_cloth.jpg");

	GLuint whiteChinaTexID =
		Texture::loadTexture("media/prototype/white_china.jpg");

	GLuint metalTexID =
		Texture::loadTexture("media/prototype/metal.jpg");

	GLuint carpetTexID =
		Texture::loadTexture("media/prototype/carpet.jpg");

	GLuint stoneTwoTexID =
		Texture::loadTexture("media/prototype/stone2.jpg");

	GLuint groundTexID =
		Texture::loadTexture("media/prototype/ground.jpg");

	GLuint groundNormalsTexID =
		Texture::loadTexture("media/prototype/ground_normals.jpg");

	GLuint ropeTexID =
		Texture::loadTexture("media/prototype/rope.jpg");

	GLuint mapTexID =
		Texture::loadTexture("media/prototype/parchment.jpg");

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

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, stoneTwoTexID);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, groundTexID);

	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, groundNormalsTexID);

	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, ropeTexID);

	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, mapTexID);
}

void Scene_Initial_Prototype::compile()
{
	try {		
		progTexture.compileShader("shader/prototype/texture_shader.vert");
		progTexture.compileShader("shader/prototype/texture_shader.frag");
		progTexture.link();		
		progTexture.use();

		progNormals.compileShader("shader/prototype/normals_shader.vert");
		progNormals.compileShader("shader/prototype/normals_shader.frag");
		progNormals.link();
		progNormals.use();
		
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Initial_Prototype::update( float t )
{
	float deltaT = (t - tPrev) * 0.5f;

	if (tPrev == 0.0f)
		deltaT = 0.0f;

	tPrev = t;

	angle += rotSpeed * deltaT;

	colourProgress += changeColSpeed * deltaT;

	if (angle > glm::two_pi<float>())
		angle -= glm::two_pi<float>();

	if (colourProgress > 1)
	{
		colourProgress -= 1;

		if (targetCol == lightColOne)
		{
			targetCol = lightColTwo;
			currentCol = lightColOne;
		}
		else
		{
			targetCol = lightColOne;
			currentCol = lightColTwo;
		}
	}		
}

void Scene_Initial_Prototype::render()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = glm::lookAt(vec3(90.0f * sin(angle) * cos(angle), 60.0f, 90.0f * cos(angle)),
		vec3(0.0f, 20.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	model = mat4(1.0f);
	setMatrices();	

	vec3 lightCol = glm::mix(currentCol, targetCol, colourProgress);

	progNormals.use();

	progNormals.setUniform("DirLight.La", lightCol * 0.2f);
	progNormals.setUniform("DirLight.L", lightCol * 1.5f);

	progTexture.use();

	progTexture.setUniform("DirLight.La", lightCol * 0.2f);
	progTexture.setUniform("DirLight.L", lightCol * 1.5f);

	progTexture.setUniform("Material.Ks", vec3(0.9f) * 0.3f);
	progTexture.setUniform("Material.Shininess", 180.0f);	

	progTexture.setUniform("RenderTex", 0);

	crates->render();

	progTexture.setUniform("RenderTex", 1);

	bigTable->render();
	stoolOne->render();
	stoolTwo->render();
	entrancePoles->render();
	tentPole->render();

	progTexture.setUniform("RenderTex", 2);

	stones->render();

	progTexture.setUniform("RenderTex", 3);
	potOne->render();

	progTexture.setUniform("RenderTex", 4);
	potTwo->render();

	progTexture.setUniform("RenderTex", 5);
	tentSheets->render();

	progTexture.setUniform("Material.Ks", vec3(0.9f) * 0.0f);

	progTexture.setUniform("RenderTex", 14);
	hatch->render();

	progTexture.setUniform("Material.Ks", vec3(0.9f) * 0.3f);

	progTexture.setUniform("RenderTex", 6);
	curtains->render();
	ribbon->render();

	progTexture.setUniform("Material.Ks", vec3(0.9f) * 1.8f);

	progTexture.setUniform("RenderTex", 7);
	ceramics->render();

	progTexture.setUniform("Material.Ks", vec3(0.9f) * 0.3f);

	progTexture.setUniform("RenderTex", 8);
	tentPegs->render();

	progTexture.setUniform("RenderTex", 9);
	carpet->render();

	progTexture.setUniform("RenderTex", 10);
	stonesTwo->render();

	progTexture.setUniform("RenderTex", 13);
	rope->render();

	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0, -2.0, 0.0));
	setMatrices();

	progNormals.use();	

	progNormals.setUniform("Material.Ks", vec3(0.9f) * 0.3f);
	progNormals.setUniform("Material.Shininess", 180.0f);

	progNormals.setUniform("ColorTex", 11);
	progNormals.setUniform("NormalMapTex", 12);
	plane.render();
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

	progNormals.use();
	progNormals.setUniform("ModelViewMatrix", mv);
	progNormals.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
		vec3(mv[1]), vec3(mv[2])));
	progNormals.setUniform("MVP", projection * mv);

	progTexture.use();
	progTexture.setUniform("ModelViewMatrix", mv);
	progTexture.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
		vec3(mv[1]), vec3(mv[2])));
	progTexture.setUniform("MVP", projection * mv);
}