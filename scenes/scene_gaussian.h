#ifndef SCENE_GAUSSIAN_H
#define SCENE_GAUSSIAN_H

#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "plane.h"
#include "teapot.h"
#include "torus.h"
#include "objmesh.h"
#include "glm/glm.hpp"
#include <sstream>
#include "cube.h"
#include "skybox.h"

class Scene_Gaussian : public Scene
{
private:
    GLSLProgram prog;

	GLuint fsQuad;
	GLuint renderFBO, intermediateFBO;
	GLuint renderTex, intermediateTex;

	Plane plane;
	Torus torus;
	Teapot teapot;

	float angle, tPrev, rotSpeed;

	void setMatrices();

    void compile();

	void setupFBO();

	void pass1();
	void pass2();
	void pass3();
	float gauss(float, float);

public:
	Scene_Gaussian();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENE_GAUSSIAN_H
