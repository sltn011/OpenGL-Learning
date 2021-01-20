cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)

project(OpenGLLearning)



######################################################################################################
# USER DEFINED VARIABLES!!!
# Path to directory with GLFW Source
set(PATH_TO_GLFW_SOURCE "D:/OpenGL/Libs/GLFW_3.3.2" CACHE PATH "Path to directory with GLFW source")
set(PATH_TO_GLFW_BUILT "${PATH_TO_GLFW_SOURCE}/build" CACHE PATH "Path to directory with GLFW built files")

# Path to directory with GLAD Source
set(PATH_TO_GLAD_SOURCE "D:/OpenGL/Libs/GLAD" CACHE PATH "Path to directory with GLAD source")
set(PATH_TO_GLAD_BUILT "${PATH_TO_GLAD_SOURCE}/build" CACHE PATH "Path to directory with GLAD built files")

# Path to directory with GLM library headers
set(PATH_TO_GLM_INCLUDES "D:/OpenGL/Libs/GLM_0.9.9.8" CACHE PATH "Path to directory with GLM headers")

# Path to directory with STB Image library headers
set(PATH_TO_STB_INCLUDE "D:/OpenGL/Libs/STB_Image" CACHE PATH "Path to directory with STB header")

# Path to directory with Assimp Source
set(PATH_TO_ASSIMP_SOURCE "D:/OpenGL/Libs/assimp-5.0.1" CACHE PATH "Path to directory with Assimp source")
set(PATH_TO_ASSIMP_BUILT "${PATH_TO_ASSIMP_SOURCE}/build" CACHE PATH "Path to directory with Assimp built files")

SET (ASSIMP_BUILD_TESTS CACHE BOOL OFF)
SET (ASSIMP_BUILD_NO_EXPORT CACHE BOOL ON)
SET (ASSIMP_BUILD_ASSIMP_TOOLS CACHE BOOL OFF)

SET (ASSIMP_BUILD_AMF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASE_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_3DS_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_AC_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ACE_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASSBIN_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_B3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_BVH_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_DXF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_CSM_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_HMP_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_IRRMESH_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_IRR_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_LWO_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_LWS_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MD2_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MD3_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MD5_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MDC_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MDL_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_NFF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_NDO_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OFF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OGRE_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OPENGEX_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MS3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_COB_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_BLEND_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_IFC_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_XGL_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_FBX_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_Q3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_Q3BSP_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_RAW_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_SIB_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_SMD_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_STL_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_TERRAGEN_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_X_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_X3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_GLTF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_3MF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MMD_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_STEP_IMPORTER CACHE BOOL OFF)

SET (ASSIMP_BUILD_ASSBIN_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASSXML_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_COLLADA_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OPENGEX_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_PLY_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_FBX_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_STL_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_X_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_X3D_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_GLTF_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_3MF_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASSJSON_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_STEP_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_3DS_EXPORTER CACHE BOOL OFF)
######################################################################################################



######################################################################################################
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

# Include Assimp Source to be built
add_subdirectory(${PATH_TO_ASSIMP_SOURCE} ${PATH_TO_ASSIMP_BUILT})
######################################################################################################



######################################################################################################
# Compiler commands
#add_definitions("-lglfw3 -lGL -lX11 -lXrandr -lXildl -lpthread")

# Folder with header files
include_directories(include)
include_directories("${PATH_TO_GLFW_SOURCE}/include")
include_directories("${PATH_TO_GLAD_SOURCE}/include")
include_directories(${PATH_TO_GLM_INCLUDES})
include_directories(${PATH_TO_STB_INCLUDE})
include_directories("${PATH_TO_ASSIMP_SOURCE}/include")
include_directories("${PATH_TO_ASSIMP_BUILT}/include")
######################################################################################################



######################################################################################################
add_library(OpenGLTools STATIC
        src/Utils.cpp
        src/GLFWInitRAII.cpp
        src/Shader.cpp
        src/Exception.cpp
        src/BasicCamera.cpp
        src/CameraFree.cpp
        src/CameraFirstPerson.cpp
        src/CameraCubemap.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/Object.cpp
        src/BasicLight.cpp
        src/DirectionalLight.cpp
        src/PointLight.cpp
        src/SpotLight.cpp
        src/Descriptor.cpp
        src/VertexArrayObject.cpp
        src/VertexBufferObject.cpp
        src/ElementBufferObject.cpp
        src/ColorBufferObject.cpp
        src/RenderBufferObject.cpp
        src/FrameBufferObject.cpp
        src/UniformBufferObject.cpp
        src/Texture.cpp
        src/Cubemap.cpp
        src/Skybox.cpp
)

target_include_directories(OpenGLTools PUBLIC include)

target_link_libraries(OpenGLTools)
######################################################################################################


######################################################################################################
# ENGINE FILES
# Engines folder
include_directories(engines)

# Engine library
add_library(OGL_E1_LIB STATIC
        "engines/First OGL Engine/src/Scene.cpp"
        "engines/First OGL Engine/src/NormalRenderer.cpp"
        "engines/First OGL Engine/src/TransparentRenderer.cpp"
        "engines/First OGL Engine/src/CubemapRenderer.cpp"
        "engines/First OGL Engine/src/MirrorRenderer.cpp"
        "engines/First OGL Engine/src/SkyboxRenderer.cpp"
)

target_include_directories(OGL_E1_LIB PUBLIC "engines/First OGL Engine/include")

target_link_libraries(OGL_E1_LIB OpenGLTools)
######################################################################################################



######################################################################################################
# Executable location
add_executable(01-Engine1Tests example/01-Engine1Tests.cpp)

add_executable(0-ModelRenderer example/0-ModelRenderer.cpp)
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
add_executable(11-LightBasics example/11-LightBasics.cpp)
add_executable(12-SimpleLightTypes example/12-SimpleLightTypes.cpp)
add_executable(13-LightCasters example/13-LightCasters.cpp)
add_executable(14-MultipleLights example/14-MultipleLights.cpp)
add_executable(15-DepthTesting example/15-DepthTesting.cpp)
add_executable(16-StencilTesting example/16-StencilTesting.cpp)
add_executable(17-Blending example/17-Blending.cpp)
add_executable(18-FaceCulling example/18-FaceCulling.cpp)
add_executable(19-Framebuffers example/19-Framebuffers.cpp)
add_executable(20-Cubemaps example/20-Cubemaps.cpp)
add_executable(21-AdvancedGLSL example/21-AdvancedGLSL.cpp)

# Linking libraries for executable
target_link_libraries(01-Engine1Tests glad glfw OGL_E1_LIB assimp)

target_link_libraries(0-ModelRenderer glad glfw OpenGLTools assimp)
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
target_link_libraries(11-LightBasics glad glfw OpenGLTools)
target_link_libraries(12-SimpleLightTypes glad glfw OpenGLTools)
target_link_libraries(13-LightCasters glad glfw OpenGLTools)
target_link_libraries(14-MultipleLights glad glfw OpenGLTools)
target_link_libraries(15-DepthTesting glad glfw OpenGLTools assimp)
target_link_libraries(16-StencilTesting glad glfw OpenGLTools assimp)
target_link_libraries(17-Blending glad glfw OpenGLTools assimp)
target_link_libraries(18-FaceCulling glad glfw OpenGLTools assimp)
target_link_libraries(19-Framebuffers glad glfw OpenGLTools assimp)
target_link_libraries(20-Cubemaps glad glfw OpenGLTools assimp)
target_link_libraries(21-AdvancedGLSL glad glfw OpenGLTools)
######################################################################################################

