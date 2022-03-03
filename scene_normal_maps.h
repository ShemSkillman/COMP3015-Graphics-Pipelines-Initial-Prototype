#ifndef SCENE_NORMAL_MAPS_H
#define SCENE_NORMAL_MAPS_H

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
