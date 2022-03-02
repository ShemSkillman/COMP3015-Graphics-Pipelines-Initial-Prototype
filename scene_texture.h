#ifndef SCENE_TEXTURE_H
#define SCENE_TEXTURE_H

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

class Scene_Texture : public Scene
{
private:
    GLSLProgram prog;
    glm::mat4 rotationMatrix;
    Plane plane;
	Teapot teapot;
	Torus torus;
    std::unique_ptr<ObjMesh> mesh;
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
