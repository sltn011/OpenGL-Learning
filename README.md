# OpenGL-Learning
My code from learning OpenGL with book from https://learnopengl.com/  
  
![Render](https://github.com/sltn011/OpenGL-Learning/blob/main/images/renderWithMultLights.png)  
  
OpenGLTools is a library by me that encapsulates some of boilerplate OpenGL code  
Mesh classes can be used to load 3D models from files  
For now Mesh classes only work with triangulated models  
ColoredMesh supports .ply files
  
Code requires GLAD OpenGL Loader, GLFW, GLM and STB_Image libraries  
GLAD: https://glad.dav1d.de/  
GLFW: https://www.glfw.org/  
GLM: https://glm.g-truc.net/0.9.9/index.html  
STB_Image: https://github.com/nothings/stb/blob/master/stb_image.h  
Assimp: https://github.com/assimp/assimp  

To compile examples set paths to required libraries in CMakeLists.txt and use CMake to generate compile files  
