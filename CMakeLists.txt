cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)

project(OpenGLLearning)



######################################################################################################
# USER DEFINED VARIABLES!!!
# Path to directory with GLFW Source
set(GLFW_SOURCE "D:/OpenGL/Libs/GLFW_3.3.2" CACHE PATH "Path to directory with GLFW source")
set(GLFW_BUILT "${GLFW_SOURCE}/build" CACHE PATH "Path to directory with GLFW built files")

# Path to directory with GLAD Source
set(GLAD_SOURCE "D:/OpenGL/Libs/GLAD" CACHE PATH "Path to directory with GLAD source")
set(GLAD_BUILT "${GLAD_SOURCE}/build" CACHE PATH "Path to directory with GLAD built files")

# Path to directory with GLM library headers
set(GLM_INCLUDES "D:/OpenGL/Libs/GLM_0.9.9.8" CACHE PATH "Path to directory with GLM headers")

# Path to directory with STB Image library headers
set(STB_INCLUDE "D:/OpenGL/Libs/STB_Image" CACHE PATH "Path to directory with STB header")

# Path to directory with Assimp Source
set(ASSIMP_SOURCE "D:/OpenGL/Libs/Assimp" CACHE PATH "Path to directory with Assimp source")
set(ASSIMP_BUILT "${ASSIMP_SOURCE}/build" CACHE PATH "Path to directory with Assimp built files")

# Path to directory with Dear ImGui Source
set(DEARIMGUI_SOURCE "D:/OpenGL/Libs/Dear_ImGui" CACHE PATH "Path to directory with Dear ImGui source")

# Path to directory with JSON library headers
set(JSON_INCLUDE "D:/OpenGL/Libs/JSON" CACHE PATH "Path to directory with JSON header")
######################################################################################################



######################################################################################################
set (ASSIMP_BUILD_TESTS CACHE BOOL OFF)
set (ASSIMP_BUILD_NO_EXPORT CACHE BOOL ON)
set (ASSIMP_BUILD_ASSIMP_TOOLS CACHE BOOL OFF)

set (ASSIMP_BUILD_AMF_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_ASE_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_3D_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_3DS_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_AC_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_ACE_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_ASSBIN_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_B3D_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_BVH_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_DXF_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_CSM_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_HMP_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_IRRMESH_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_IRR_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_LWO_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_LWS_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_MD2_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_MD3_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_MD5_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_MDC_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_MDL_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_NFF_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_NDO_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_OFF_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_OGRE_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_OPENGEX_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_MS3D_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_COB_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_BLEND_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_IFC_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_XGL_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_FBX_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_Q3D_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_Q3BSP_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_RAW_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_SIB_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_SMD_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_STL_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_TERRAGEN_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_X_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_X3D_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_GLTF_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_3MF_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_MMD_IMPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_STEP_IMPORTER CACHE BOOL OFF)

set (ASSIMP_BUILD_ASSBIN_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_ASSXML_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_COLLADA_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_OPENGEX_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_PLY_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_FBX_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_STL_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_X_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_X3D_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_GLTF_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_3MF_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_ASSJSON_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_STEP_EXPORTER CACHE BOOL OFF)
set (ASSIMP_BUILD_3DS_EXPORTER CACHE BOOL OFF)
######################################################################################################



######################################################################################################
set(DEARIMGUI_FILES
    ${DEARIMGUI_SOURCE}/imgui.cpp
    ${DEARIMGUI_SOURCE}/imgui_demo.cpp
    ${DEARIMGUI_SOURCE}/imgui_draw.cpp
    ${DEARIMGUI_SOURCE}/imgui_tables.cpp
    ${DEARIMGUI_SOURCE}/imgui_widgets.cpp
    ${DEARIMGUI_SOURCE}/backends/imgui_impl_glfw.cpp 
    ${DEARIMGUI_SOURCE}/backends/imgui_impl_opengl3.cpp 
)
######################################################################################################


######################################################################################################
# Set directory where executables are created
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "./example/exec")

# Generate compile_commands.json for vscode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Turn off building unnecessary files
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Include GLFW Source to be built
add_subdirectory(${GLFW_SOURCE} ${GLFW_BUILT})

# Include GLAD Source to be built
add_subdirectory(${GLAD_SOURCE} ${GLAD_BUILT})

# Include Assimp Source to be built
add_subdirectory(${ASSIMP_SOURCE} ${ASSIMP_BUILT})
######################################################################################################



######################################################################################################
# Compiler commands
#add_definitions("-lglfw3 -lGL -lX11 -lXrandr -lXildl -lpthread")

# Folder with header files
include_directories(include)
include_directories("${GLFW_SOURCE}/include")
include_directories("${GLAD_SOURCE}/include")
include_directories(${GLM_INCLUDES})
include_directories(${STB_INCLUDE})
include_directories("${ASSIMP_SOURCE}/include")
include_directories("${ASSIMP_BUILT}/include")
include_directories(${DEARIMGUI_SOURCE})
include_directories("${DEARIMGUI_SOURCE}/backends")
include_directories(${JSON_INCLUDE})
######################################################################################################



######################################################################################################
add_library(OpenGLTools STATIC
    src/GLFWInitRAII.cpp
    src/Shader.cpp
    src/Exception.cpp
    src/BasicCamera.cpp
    src/CameraStatic.cpp
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
    src/ShadowMap.cpp
    src/ShadowCubemap.cpp
    src/CameraShadowMap.cpp
    src/CameraShadowCubemap.cpp
	src/GBuffer.cpp
)

target_include_directories(OpenGLTools PUBLIC include)

target_link_libraries(OpenGLTools)
######################################################################################################



######################################################################################################
# ENGINE FILES
# Engines folder
include_directories("engines/First OGL Engine/include")

# Engine library
add_library(OGL_E1 STATIC
	    "engines/First OGL Engine/src/OGL_E1.cpp"
        "engines/First OGL Engine/src/Scene.cpp"
        "engines/First OGL Engine/src/NormalRenderer.cpp"
        "engines/First OGL Engine/src/TransparentRenderer.cpp"
        "engines/First OGL Engine/src/CubemapRenderer.cpp"
        "engines/First OGL Engine/src/MirrorRenderer.cpp"
        "engines/First OGL Engine/src/SkyboxRenderer.cpp"
        "engines/First OGL Engine/src/InstancesRenderer.cpp"
        "engines/First OGL Engine/src/ShadowMapRenderer.cpp"
        "engines/First OGL Engine/src/ShadowCubemapRenderer.cpp"
        "engines/First OGL Engine/src/LightSourcesDebugRenderer.cpp"
        "engines/First OGL Engine/src/ColoredShapesRenderer.cpp"
        "engines/First OGL Engine/src/LevelSaver.cpp"
        "engines/First OGL Engine/src/LevelLoader.cpp"
		"engines/First OGL Engine/src/Bloom.cpp"
)

add_library(OGL_E1_GUI STATIC
        "engines/First OGL Engine/src/GUI/GUIRenderer.cpp"
        "engines/First OGL Engine/src/GUI/BasicWindow.cpp"
        "engines/First OGL Engine/src/GUI/MainWindow.cpp"
        "engines/First OGL Engine/src/GUI/ObjectTransformWindow.cpp"
        "engines/First OGL Engine/src/GUI/CursorPicker.cpp"
		"engines/First OGL Engine/src/GUI/EnvironmentControlWindow.cpp"
		"engines/First OGL Engine/src/GUI/PostprocessingWindow.cpp"
)

target_include_directories(OGL_E1 PUBLIC "engines/First OGL Engine/include")
target_include_directories(OGL_E1_GUI PUBLIC "engines/First OGL Engine/include")

target_link_libraries(OGL_E1 OpenGLTools)
target_link_libraries(OGL_E1_GUI OpenGLTools)
######################################################################################################



######################################################################################################
# Executable location
add_executable(01-Engine1Tests example/01-Engine1Tests.cpp ${DEARIMGUI_FILES})
add_executable(02-ImGuiTests example/02-ImGuiTests.cpp ${DEARIMGUI_FILES})

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
add_executable(22-GeometryShader example/22-GeometryShader.cpp)
add_executable(23-Instancing example/23-Instancing.cpp)
add_executable(24-BlinnPhong example/24-BlinnPhong.cpp ${DEARIMGUI_FILES})
add_executable(25-GammaCorrection example/25-GammaCorrection.cpp ${DEARIMGUI_FILES})
add_executable(26-ShadowMapping example/26-ShadowMapping.cpp ${DEARIMGUI_FILES})
add_executable(27-PointShadow example/27-PointShadow.cpp ${DEARIMGUI_FILES})
add_executable(28-NormalMapping example/28-NormalMapping.cpp)
add_executable(29-ParallaxMapping example/29-ParallaxMapping.cpp ${DEARIMGUI_FILES})
add_executable(30-HDR example/30-HDR.cpp ${DEARIMGUI_FILES})
add_executable(31-Bloom example/31-Bloom.cpp ${DEARIMGUI_FILES})
add_executable(32-DeferredShading example/32-DeferredShading.cpp ${DEARIMGUI_FILES})

# Linking libraries for executable
target_link_libraries(01-Engine1Tests glad glfw OGL_E1 OGL_E1_GUI assimp)
target_link_libraries(02-ImGuiTests glad glfw OGL_E1 OGL_E1_GUI assimp)

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
target_link_libraries(22-GeometryShader glad glfw OpenGLTools)
target_link_libraries(23-Instancing glad glfw OpenGLTools assimp)
target_link_libraries(24-BlinnPhong glad glfw assimp OpenGLTools OGL_E1 OGL_E1_GUI)
target_link_libraries(25-GammaCorrection glad glfw assimp OpenGLTools OGL_E1 OGL_E1_GUI)
target_link_libraries(26-ShadowMapping glad glfw assimp OpenGLTools OGL_E1 OGL_E1_GUI)
target_link_libraries(27-PointShadow glad glfw assimp OpenGLTools OGL_E1 OGL_E1_GUI)
target_link_libraries(28-NormalMapping glad glfw assimp OpenGLTools)
target_link_libraries(29-ParallaxMapping glad glfw assimp OpenGLTools)
target_link_libraries(30-HDR glad glfw OGL_E1 OGL_E1_GUI assimp)
target_link_libraries(31-Bloom glad glfw OGL_E1 OGL_E1_GUI assimp)
target_link_libraries(32-DeferredShading glad glfw OGL_E1 OGL_E1_GUI assimp)
######################################################################################################

