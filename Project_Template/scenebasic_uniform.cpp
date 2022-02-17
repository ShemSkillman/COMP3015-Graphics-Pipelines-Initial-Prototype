#include "scenebasic_uniform.h"

#include "scenebasic_uniform.h"
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 100, 100) {}

void SceneBasic_Uniform::initScene()
{
    compile();

	glEnable(GL_DEPTH_TEST);

	model = mat4(1.0f);
	
	prog.setUniform("ViewPosition", vec3(0.0f, 0.0f, 2.0f));
	prog.setUniform("SpecularPowerCoefficient", 10);
	prog.setUniform("Ks", glm::vec3(0.1f));
	prog.setUniform("Ls", glm::vec3(0.02f));

	view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	prog.setUniform("Ka", glm::vec3(1.0f));
	prog.setUniform("La", glm::vec3(0.3f));

	prog.setUniform("Kd", glm::vec3(1.0f));
	prog.setUniform("Ld", glm::vec3(1.0f));

	prog.setUniform("LightPosition", view *
		glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	model = glm::rotate(model, glm::radians(0.001f * t), vec3(0.0f, 1.0f, 0.0f));
}

void SceneBasic_Uniform::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setMatrices();
	torus.render();
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h,
		0.3f, 100.0f);

}

void SceneBasic_Uniform::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
		vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}