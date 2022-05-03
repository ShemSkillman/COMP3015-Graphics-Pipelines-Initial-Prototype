#ifndef SCENE_POINT_SPRITES_H
#define SCENE_POINT_SPRITES_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

class Scene_Point_Sprites : public Scene
{
private:
    GLSLProgram prog;

    GLuint sprites;
    int numSprites;
    float* locations;

    void setMatrices();
    void compile();

public:
    Scene_Point_Sprites();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENE_POINT_SPRITES_H
