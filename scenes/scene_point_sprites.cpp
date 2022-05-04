#include "scene_point_sprites.h"

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

Scene_Point_Sprites::Scene_Point_Sprites()
{
}

void Scene_Point_Sprites::initScene()
{
	compile();

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	numSprites = 50;
	locations = new float[numSprites * 3];
	srand((unsigned int)time(0));

	for (int i = 0; i < numSprites; i++)
	{
		vec3 p(((float)rand() / RAND_MAX * 2.0f) - 1.0f,
			((float)rand() / RAND_MAX * 2.0f) - 1.0f,
			((float)rand() / RAND_MAX * 2.0f) - 1.0f);
		
		locations[i * 3] = p.x;
		locations[i * 3 + 1] = p.y;
		locations[i * 3 + 2] = p.z;
	}

	GLuint handle;
	glGenBuffers(1, &handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, numSprites * 3 * sizeof(float), locations, GL_STATIC_DRAW);

	delete[] locations;

	glGenVertexArrays(1, &sprites);
	glBindVertexArray(sprites);

	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	const char* texName = "media/texture/flower.png";
	Texture::loadTexture(texName);

	prog.setUniform("SpriteTex", 0);
	prog.setUniform("Size2", 0.15f);
}

void Scene_Point_Sprites::compile()
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

void Scene_Point_Sprites::update( float t )
{
}

void Scene_Point_Sprites::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec3 cameraPos(0.0f, 0.0f, 3.0f);
	view = glm::lookAt(cameraPos,
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));

	model = mat4(1.0f);
	setMatrices();

	glBindVertexArray(sprites);
	glDrawArrays(GL_POINTS, 0, numSprites);

	glFinish();
}

void Scene_Point_Sprites::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h,
		0.3f, 100.0f);

}

void Scene_Point_Sprites::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("ProjectionMatrix", projection);
}