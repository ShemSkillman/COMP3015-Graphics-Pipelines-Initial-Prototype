#include "scene.h"
#include "scenerunner.h"
#include "scenes/scenebasic_uniform.h"
#include "scenes/scene_texture.h"
#include "scenes/scene_toon.h"
#include "scenes/scene_normal_maps.h"
#include "scenes/scene_skybox.h"
#include "scenes/scene_edge_detection.h"
#include "scenes/scene_projection_texture.h"
#include "scenes/scene_render_to_texture.h"
#include "scenes/scene_initial_prototype.h"
#include "scenes/scene_gaussian.h"
#include "scenes/scene_hdr.h"

int main(int argc, char* argv[])
{
	SceneRunner runner("Shader_Basics");

	std::unique_ptr<Scene> scene;

	scene = std::unique_ptr<Scene>(new Scene_HDR());

	return runner.run(*scene);
}