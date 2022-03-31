#ifndef SCENE_HDR_H
#define SCENE_HDR_H

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
#include <sphere.h>

class Scene_HDR : public Scene
{
private:
	GLSLProgram prog;

	GLuint hdrFBO;
	GLuint quad;
	GLuint hdrTex, avgTex;

	Plane plane;
	Sphere sphere;
	Teapot teapot;

	void setMatrices();
	void compile();

	void setupFBO();
	void pass1();
	void pass2();
	void computeLogAveLuminance();
	void drawScene();

public:
	Scene_HDR();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENE_HDR_H
