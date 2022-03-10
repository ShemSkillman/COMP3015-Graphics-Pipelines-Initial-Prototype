#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "scenebasic_uniform.h"
#include "scene_texture.h"
#include "scene_toon.h"
#include "scene_normal_maps.h"
#include "scene_skybox.h"
#include "scene_edge_detection.h"
#include "scene_projection_texture.h"

int main(int argc, char* argv[])
{
	SceneRunner runner("Shader_Basics");

	std::unique_ptr<Scene> scene;

	scene = std::unique_ptr<Scene>(new Scene_Projection_Texture());

	return runner.run(*scene);
}