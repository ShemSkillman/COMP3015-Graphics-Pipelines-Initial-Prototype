#ifndef SCENE_EDGE_DETECTION_H
#define SCENE_EDGE_DETECTION_H

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

class Scene_Edge_Detection : public Scene
{
private:
    GLSLProgram prog;

	GLuint fsQuad, fboHandle, renderTex;

	Plane plane;
	Torus torus;
	Teapot teapot;

	float angle, tPrev, rotSpeed;

    void compile();

	void setupFBO();
	void pass1();
	void pass2();

public:
    Scene_Edge_Detection();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_EDGE_DETECTION_H
