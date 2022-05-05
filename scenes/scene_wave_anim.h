#ifndef SCENE_WAVE_ANIM_H
#define SCENE_WAVE_ANIM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <plane.h>

class Scene_Wave_Anim : public Scene
{
private:
    GLSLProgram prog;

    Plane plane;

    float angle;
    float time;

    GLuint sprites;
    int numSprites;
    float* locations;

    void setMatrices();
    void compile();

public:
    Scene_Wave_Anim();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENE_WAVE_ANIM_H
