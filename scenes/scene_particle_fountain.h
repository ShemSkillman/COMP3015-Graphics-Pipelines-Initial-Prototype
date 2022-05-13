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
    GLSLProgram prog, flatProg;

    Random rand;

    // Position of direction of particle emitter
    glm::vec3 emitterPos, emitterDir;

    // Particle buffers
    GLuint posBuf[2], velBuf[2], age[2];

    // Particle VAOs
    GLuint particleArray[2];

    // Transform feedbacks
    GLuint feedback[2];

    GLuint drawBuf;
    Grid grid;
    
    int nParticles;
    float particleLifeTime, angle, time, deltaT;

    void initBuffers();

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
