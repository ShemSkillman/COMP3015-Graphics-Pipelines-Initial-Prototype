#ifndef SCENE_INITIAL_PROTOTYPE
#define SCENE_INITIAL_PROTOTYPE

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/plane.h"
#include "helper/teapot.h"
#include "helper/torus.h"
#include "helper/objmesh.h"
#include "glm/glm.hpp"
#include <sstream>
#include "helper/cube.h"

class Scene_Initial_Prototype : public Scene
{
private:
    GLSLProgram prog;

    GLuint fsQuad, fboHandle, renderTex;

    float angle, tPrev, rotSpeed;

    Plane plane;
	Teapot teapot;
	Torus torus;
	Cube cube;

    void compile();

    void setupFBO();
    void pass1();
    void pass2();

public:
    Scene_Initial_Prototype();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_INITIAL_PROTOTYPE
