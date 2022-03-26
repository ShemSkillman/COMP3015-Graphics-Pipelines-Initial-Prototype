#ifndef SCENE_NORMAL_MAPS_H
#define SCENE_NORMAL_MAPS_H

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

class Scene_Normal_Maps : public Scene
{
private:
    GLSLProgram prog;
    glm::mat4 rotationMatrix;

	std::unique_ptr<ObjMesh> ogre;

    void compile();

public:
	Scene_Normal_Maps();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_NORMAL_MAPS_H
