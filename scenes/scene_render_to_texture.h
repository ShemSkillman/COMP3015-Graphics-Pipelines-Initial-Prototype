#ifndef SCENE_RENDER_TO_TEXTURE
#define SCENE_RENDER_TO_TEXTURE

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

class Scene_Render_To_Texture : public Scene
{
private:
    GLSLProgram prog;
    glm::mat4 rotationMatrix;

	std::unique_ptr<ObjMesh> spot;
	Cube cube;

	unsigned int fboHandle;

	float tPrev;
	float angle;

    void compile();
	void setupFBO();
	void renderScene();
	void renderToTexture();

public:
	Scene_Render_To_Texture();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void setMatrices();
};

#endif // SCENE_RENDER_TO_TEXTURE