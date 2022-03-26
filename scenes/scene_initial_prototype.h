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
