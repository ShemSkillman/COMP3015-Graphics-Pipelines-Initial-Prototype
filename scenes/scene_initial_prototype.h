#ifndef SCENE_INITIAL_PROTOTYPE
#define SCENE_INITIAL_PROTOTYPE

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

class Scene_Initial_Prototype : public Scene
{
private:
    GLSLProgram progNormals, progTexture;

    std::unique_ptr<ObjMesh> crates, bigTable, stoolOne, stoolTwo, stones, potOne, potTwo, tentSheets, rope,
        curtains, ribbon, scrollHandles, scrollParchment, ceramics, tentPegs, carpet, tentPole, stonesTwo;

    glm::vec3 lightColOne, lightColTwo, targetCol, currentCol;

    float colourProgress, changeColSpeed;
    float spotDelta, spotChangeSpeed;
    float angle, tPrev, rotSpeed;

    Plane plane;

    void compile();
    void importModels();

public:
    Scene_Initial_Prototype();

    void initScene();
    void loadTextures();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_INITIAL_PROTOTYPE
