#ifndef SCENE_SKYBOX_H
#define SCENE_SKYBOX_H

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
#include "helper/skybox.h"

class Scene_Skybox : public Scene
{
private:
    GLSLProgram prog;
	SkyBox sky;

	float angle, tPrev, rotSpeed;

    void compile();

public:
	Scene_Skybox();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_SKYBOX_H