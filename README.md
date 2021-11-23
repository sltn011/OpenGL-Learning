# OpenGL-Learning
My code from learning OpenGL with book from https://learnopengl.com/ + my own tests with computer graphics  
  
![Render](https://github.com/sltn011/OpenGL-Learning/blob/main/images/playground4.png)  
  
OpenGLTools is a library by me that encapsulates some of boilerplate OpenGL code  
Engines folder right now contains my first OpenGL engine made by me for learning and testing purposes.  
Engine can render shadows from all three currently implemented light sources  
  
Dependencies:    
GLAD: https://glad.dav1d.de/  
GLFW: https://www.glfw.org/  
GLM: https://glm.g-truc.net/0.9.9/index.html  
STB_Image: https://github.com/nothings/stb/blob/master/stb_image.h  
TinyOBJLoader: https://github.com/tinyobjloader/tinyobjloader  
JSON: https://github.com/nlohmann/json  

To compile examples set paths to required libraries in CMakeLists.txt and use CMake to generate compile files  
  
# TODO LIST:  
~~Rotation of objects with quaternions~~  
Instanced rendering of Objects  
GUI for editing and debugging scene(partially done)  
~~Saving and loading scenes to/from files~~  
~~Normal mapping, parallax mapping~~  
~~HDR, Bloom, SSAO~~  
~~Deferred Shading~~  
PBR  
