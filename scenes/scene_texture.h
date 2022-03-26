#ifndef SCENE_TEXTURE_H
#define SCENE_TEXTURE_H

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

class Scene_Texture : public Scene
{
private:
    GLSLProgram prog;
    glm::mat4 rotationMatrix;
    Plane plane;
	Teapot teapot;
	Torus torus;
	Cube cube;

	float tPrev;
	float angle;

    void compile();

public:
	Scene_Texture();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_TEXTURE_H
