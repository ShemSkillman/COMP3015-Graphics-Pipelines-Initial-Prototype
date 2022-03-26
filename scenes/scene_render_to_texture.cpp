#include "scene_render_to_texture.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"
#include "texture.h"

using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::mat3;

Scene_Render_To_Texture::Scene_Render_To_Texture() : cube(1.0f)
{
	fboHandle = -1;
	tPrev = 0.0f;
	angle = 0.0f;

	spot = ObjMesh::load("media/spot/spot_triangulated.obj", false, true);
}

void Scene_Render_To_Texture::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);

	prog.setUniform("Light.L", vec3(1.0f));
	prog.setUniform("Light.La", vec3(0.15f));

	GLuint spotTexID =
		Texture::loadTexture("media/spot/spot_texture.png");	

	setupFBO();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, spotTexID);
}

void Scene_Render_To_Texture::setupFBO()
{
	// Generate and bind the framebuffer
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	// Create the texture object
	GLuint renderTex;
	glGenTextures(1, &renderTex);
	glActiveTexture(GL_TEXTURE0); // Use texture unit 0, this is what renderTex is bound to
	glBindTexture(GL_TEXTURE_2D, renderTex);

	// Specifies the storage format of texture
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);

	// Config texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind the texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		renderTex, 0);

	// Create the depth buffer
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);

	// Bind the depth buffer to the FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuf);

	// Set the targets for the fragment output variables
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (result == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer is complete" << endl;
	}
	else {
		std::cout << "Framebuffer error: " << result << endl;
	}

	// Unbind the framebuffer, and revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene_Render_To_Texture::compile()
{
	try {
		prog.compileShader("shader/render_to_texture_shader.vert");
		prog.compileShader("shader/render_to_texture_shader.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Render_To_Texture::update( float t )
{
	float deltaT = t - tPrev;
	if (tPrev == 0.0f)
		deltaT = 0.0f;

	tPrev = t;
	angle += 2.0f * deltaT;
	if (angle > glm::two_pi<float>())
	{
		angle -= glm::two_pi<float>();
	}
	
	std::string blah;
	std::cin >> blah;
}

void Scene_Render_To_Texture::render()
{
	//bind the buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);	

	//render to texture
	renderToTexture();

	//flush the buffer
	glFlush();

	//unbind the write buffer and bind the default buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//render the scene using the newly written texture
	renderScene();
}

void Scene_Render_To_Texture::renderScene() {
	prog.setUniform("RenderTex", 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec3 cameraPos = vec3(2.0f * cos(angle), 1.5f, 2.0f * sin(angle));
	view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.3f, 100.0f);

	prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
	prog.setUniform("Material.Shininess", 1.0f);

	model = mat4(1.0f);

	setMatrices();
	
	cube.render();
}

void Scene_Render_To_Texture::renderToTexture() {
	 prog.setUniform("RenderTex", 1);
	 glViewport(0, 0, 512, 512);
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 view = glm::lookAt(vec3(0.0f, 0.0f, 2.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	 projection = glm::perspective(glm::radians(50.0f), 1.0f, 0.3f, 100.0f);

	 prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	 prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
	 prog.setUniform("Material.Shininess", 100.0f);

	 model = mat4(1.0f);
	 model = glm::rotate(model, angle, vec3(0.0f, 1.0f, 0.0f));

	 setMatrices();

	 spot->render();
}

void Scene_Render_To_Texture::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h,
		0.3f, 100.0f);

}

void Scene_Render_To_Texture::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
		vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}