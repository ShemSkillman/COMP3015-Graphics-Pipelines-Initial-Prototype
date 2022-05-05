#ifndef SCENE_PARTICLE_FOUNTAIN_H
#define SCENE_PARTICLE_FOUNTAIN_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <plane.h>
#include <random.h>
#include "helper/grid.h"

class Scene_Particle_Fountain : public Scene
{
private:
    GLSLProgram prog;

    Random rand;

    GLuint initVel, startTime, particles, nParticles;

    Grid grid;

    // Position and direction of particle emitter
    glm::vec3 emitterPos, emitterDir;

    float angle, time, particleLifetime;

    void initBuffers();
    float randFloat();

    void setMatrices(GLSLProgram&);

    void compile();

public:
    Scene_Particle_Fountain();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENE_PARTICLE_FOUNTAIN_H
