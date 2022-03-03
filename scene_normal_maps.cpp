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

	view = glm::lookAt(vec3(-1.0f, 0.25f, 2.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	float x, z;
	for (int i = 0; i < 3; i++)
	{
		std::stringstream name;
		name << "lights[" << i << "].Position";
		x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
		z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
		prog.setUniform(name.str().c_str(), view * glm::vec4(x, 2.2f, z +
			1.0f, 1.0f));
	}

	prog.setUniform("lights[0].L", vec3(0.8f));
	prog.setUniform("lights[1].L", vec3(0.4f));
	prog.setUniform("lights[2].L", vec3(0.6f));

	prog.setUniform("lights[0].La", vec3(0.2f));
	prog.setUniform("lights[1].La", vec3(0.2f));
	prog.setUniform("lights[2].La", vec3(0.2f));

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
}

void Scene_Normal_Maps::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.setUniform("Material.Kd", 1.0f, 0.76f, 0.15f);
	prog.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
	prog.setUniform("Material.Ka", 0.50f, 0.38f, 0.075f);
	prog.setUniform("Material.Shininess", 180.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(3.0f, 0.75f, 3.0f));
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
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