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

    GLuint renderTex;

    std::unique_ptr<ObjMesh> crateOne, bigTable, stoolOne, stoolTwo, crateTwo, stones, potOne, potTwo, tentSheets, curtains, ribbon, entrancePoles, hatch, ceramics, tentPegs, carpet;

    Plane plane;

    void compile();

public:
    Scene_Initial_Prototype();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_INITIAL_PROTOTYPE
