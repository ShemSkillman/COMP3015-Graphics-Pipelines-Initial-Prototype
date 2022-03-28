# Initial Custom Shader Model Prototype by Shem Skillman
This is a campsite scene rendered in OpenGL using 2 shaders and various textured 3D models.

## Environments used to test and write the code

IDE: Microsoft Visual Studio Community 2019 Version 16.11.3

OS : Microsoft Windows 10

## Features

 * All objects rendererd in the scene have appropriate textures applied to them.
 * Animated camera that smoothly rotates around the campsite to give all angles of the scene.
 * Forest ground is rendered with texture and normal map.
 * Direction light which gradually fades between two colours (light orange to a darker blue) to imitate the different times in the day.
 * A scroll lit up by a gold coloured spotlight that fades in and out.
 * White fog that becomes more noticable as the scene becomes darker.

## How does it work?
Simply open the .exe file in the build folder and then sit back and relax!
There is no input required from the user.

## Code walkthrough

[Video Link]()

## **Scene_Initial_Prototype**

The scene_initial_prototype class is responsible for defining and managing all the objects and textures used to render the scene.

### loadTextures()
 * Loads and binds textures for later use.
 * The texture unit index is later used in the render() method telling the shaders what textures to apply to each model.

 ### update() 
  * Changes the camera angle, spotlight intensity, and direction light colour over time to give a dynamic scene.

### render()
 * Updates the camera position.
 * Configures the spotlight position relative to the camera.
 * Sets the direction light to a blended colour between 2 pre-defined colours used a weighted value that changes in the update()
 * Majority of the method is setting the render texture and then rendering objects that use that texture.
 * Notice the spot light intensity is only changed when the scroll is rendered because none of the surrounding objects around the scroll should be influenced by the spotlight.
 * progNormals.use() is then called after progTexture is finished with to render the ground that is able to use a normal map.

### compile()
 * Compiles both shaders for use.
 * Only has to be completed once on startup.

 ## **texture_shader**
The texture_shader is made up of texture_shader.vert and texture_shader.frag. These can be found in shaders/prototype.

Majority of the processing happens in the fragment shader so we'll be looking at the code in there.

### dirBlinnPhong()
 * First off, the colour of the pixel is extracted from the texture.
 * Ambient light is then calculated to prevent completely black areas in the scene.
 * Diffuse is then caluclated using dot product between normal and light direction at fragment position. If the normal and inverse light direction line up perfectly then the light will be strongest in this area to mimic real lighting.
 * Finally the specular is calculated. The normalized negative vertex position gives us the direction from the vertex position to the eye or camera coordinates (since the vertex position is calculated using the ModelView matrix). Since we are using blinn phong, this is an optimized calculation using a half vector which is close to the reflection vector. This still gives believeable lighting results whilst saving on computation.
 * The ambient, diffuse, and specular values are then summed together using the appropriate material values so that the lighting can behave differently on separate objects.

### blinnPhongSpot()
 * Very similar to dirBlinnPhong except only areas within spotlight cone are lit up.
 * An angle is calculated relative to the current vertex position. This angle is used to see whether this area in the scene should be lit up - is in contained within the cone of the spotlight?
 * Areas outside the cone of influence are left to only use the spotlight ambience.
 * The intensity of light in the spotlight is such that the center of the spotlight circle is the strongest. The spotlight intensity decrease towards the edges of the circle is determined by the exponent value assigned to the spotlight. A higher exponent value results in increasingly diminishing light intensity values (using power function).


### main()
 * Fog is calculated by judging the distance between the eye coords and the vertex position on the z axis. If the distance falls between the fog min and max range, the fragment colour is blended between the light processed texel and fog colour.
 * The shade colour is simply the summation of the blinphong calculations for the direction light and spotlight.

 ## **normals_shader**
 The normals shader is very similar to the texture shader with the added ability of being able to accept a normal map.
 
 The normal map is used to give the illusion that the object surface has the bumps in the given texture so that it appears to be more 3 dimensional.

 Most of the new code is in the vertex shader so we'll start off looking at the code in there.

 ### normals_shader.vert
 * A new in parameter VertexTangent is passed in from the model file data. This basically helps gives us context on the rotation of each face so that the normal mapping can be applied properly.
 * Using the tangent direction (x axis) and normal direction (z axis) we can calculate the binormal direction (y axis) using the cross product. Since we now have the directions of each axis we can now convert the light and view directions from world space to local space to use the localized normal values provided by the normal map.


  ### normals_shader.frag
 * Within main() the texel is extracted from the normal map and its colour values are used to determine the direction of the normal. 
 * This normal is then passed to blinnPhong so that the lighting for this fragment is influenced by the provided normal map.