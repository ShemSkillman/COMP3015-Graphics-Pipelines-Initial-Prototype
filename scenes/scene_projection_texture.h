#ifndef SCENE_PROJECTION_TEXTURE_H
#define SCENE_PROJECTION_TEXTURE_H

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

class Scene_Projection_Texture : public Scene
{
private:
    GLSLProgram prog;
	Teapot teapot;
	Plane plane;


	float angle, tPrev, rotSpeed;

    void compile();

public:
    Scene_Projection_Texture();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_PROJECTION_TEXTURE_H
