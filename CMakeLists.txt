cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)

project(OpenGLLearning)


# USER DEFINED VARIABLES!!!
# Path to directory with GLFW Source
set(PATH_TO_GLFW_SOURCE "/home/sltn011/Documents/cpp/OpenGL Libs/GLFW" CACHE PATH "Path to directory with GLFW source")
set(PATH_TO_GLFW_BUILT "${PATH_TO_GLFW_SOURCE}/build" CACHE PATH "Path to directory with GLFW built files")

# Path to directory with GLAD Source
set(PATH_TO_GLAD_SOURCE "/home/sltn011/Documents/cpp/OpenGL Libs/GLAD" CACHE PATH "Path to directory with GLAD source")
set(PATH_TO_GLAD_BUILT "${PATH_TO_GLAD_SOURCE}/build" CACHE PATH "Path to directory with GLAD built files")

# Path to directory with GLM library headers
set(PATH_TO_GLM_INCLUDES "/home/sltn011/Documents/cpp/OpenGL Libs/GLM" CACHE PATH "Path to directory with GLM headers")

# Path to directory with STB Image library headers
set(PATH_TO_STB_INCLUDE "/home/sltn011/Documents/cpp/OpenGL Libs/STBImage" CACHE PATH "Path to directory with STB header")



# Set directory where executables are created
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "./example")

# Generate compile_commands.json for vscode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Turn off building unnecessary files
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Include GLFW Source to be built
add_subdirectory(${PATH_TO_GLFW_SOURCE} ${PATH_TO_GLFW_BUILT})

# Include GLAD Source to be built
add_subdirectory(${PATH_TO_GLAD_SOURCE} ${PATH_TO_GLAD_BUILT})

add_library(OpenGLTools STATIC src/GLFWInitRAII src/Shader.cpp src/Exception.cpp src/CameraFree.cpp src/Mesh.cpp)

target_include_directories(OpenGLTools PUBLIC include)

# Compiler commands
#add_definitions("-lglfw3 -lGL -lX11 -lXrandr -lXildl -lpthread")

# Folder with header files
include_directories(include)
include_directories("${PATH_TO_GLFW_SOURCE}/include")
include_directories("${PATH_TO_GLAD_SOURCE}/include")
include_directories(${PATH_TO_GLM_INCLUDES})
include_directories(${PATH_TO_STB_INCLUDE})

# Executable location
add_executable(1-HelloWindow example/1-HelloWindow.cpp)
add_executable(2-HelloTriangle example/2-HelloTriangle.cpp)
add_executable(3-HelloRectangle example/3-HelloRectangle.cpp)
add_executable(4-TriangleExercise example/4-TriangleExercise.cpp)
add_executable(5-ShadersBasic example/5-ShadersBasic.cpp)
add_executable(6-TextureBasics example/6-TextureBasics.cpp)
add_executable(7-TextureExercise example/7-TextureExercise.cpp)
add_executable(8-Transformations example/8-Transformations.cpp)
add_executable(9-3DCube example/9-3DCube.cpp)
add_executable(10-Camera example/10-Camera.cpp)
add_executable(11-ModelRenderer example/11-ModelRenderer.cpp)

# Linkning libraries for executable
target_link_libraries(1-HelloWindow glad glfw)
target_link_libraries(2-HelloTriangle glad glfw)
target_link_libraries(3-HelloRectangle glad glfw)
target_link_libraries(4-TriangleExercise glad glfw)
target_link_libraries(5-ShadersBasic glad glfw OpenGLTools)
target_link_libraries(6-TextureBasics glad glfw OpenGLTools)
target_link_libraries(7-TextureExercise glad glfw OpenGLTools)
target_link_libraries(8-Transformations glad glfw OpenGLTools)
target_link_libraries(9-3DCube glad glfw OpenGLTools)
target_link_libraries(10-Camera glad glfw OpenGLTools)
target_link_libraries(11-ModelRenderer glad glfw OpenGLTools)