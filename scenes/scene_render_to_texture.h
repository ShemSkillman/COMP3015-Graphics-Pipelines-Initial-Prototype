#ifndef SCENE_RENDER_TO_TEXTURE
#define SCENE_RENDER_TO_TEXTURE

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

class Scene_Render_To_Texture : public Scene
{
private:
    GLSLProgram prog;
    glm::mat4 rotationMatrix;

	std::unique_ptr<ObjMesh> spot;
	Cube cube;

	GLuint fboHandle;

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
