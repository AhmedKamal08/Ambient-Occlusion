cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0015 NEW)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(appName BaseCode)
project(${appName})

if(UNIX)
    #If running on Unix
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

    execute_process(COMMAND ln -s ../shaders)
    execute_process(COMMAND ln -s ../fonts)
    
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(SOIL REQUIRED)
    find_package(Freetype REQUIRED)

elseif(WIN32)
    # If running on Windows
    find_package(OpenGL REQUIRED)

    # Setup GLUT
    set(GLEW_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/lib/Release/x64)
    set(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/include)
    set(GLEW_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/lib/Release/x64/glew32.lib)
    
    # Setup GLUT
    set(GLUT_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/freeglut/lib/x64)
    set(GLUT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/freeglut/include)
    set(GLUT_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/freeglut/lib/x64/freeglut.lib)
    
    # Setup freetype
    set(FREETYPE_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/freetype291/win64)
    set(FREETYPE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/freetype291/include)
    set(FREETYPE_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/freetype291/win64/freetype.lib)
    
    # Setup SOIL
    set(SOIL_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/soil/libs)
    set(SOIL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/soil/include)
    set(SOIL_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/soil/libs/SOIL.lib)
    
endif()

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/glm")
include_directories(${SOIL_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})

link_directories(${OPENGL_LIBRARY_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
link_directories(${SOIL_LIBRARY_DIRS})
link_directories(${FREETYPE_LIBRARY_DIRS})

add_executable(${appName} Text.h Text.cpp TexturedQuad.h TexturedQuad.cpp Texture.h Texture.cpp PLYReader.h PLYReader.cpp TriangleMesh.h TriangleMesh.cpp Camera.h Camera.cpp Scene.h Scene.cpp Shader.h Shader.cpp ShaderProgram.h ShaderProgram.cpp Application.h Application.cpp Mesh.h Mesh.cpp main.cpp )

target_link_libraries(${appName} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${SOIL_LIBRARIES} ${FREETYPE_LIBRARIES})

if (MSVC)
    # If building with Visual Studio 
    # This is needed, because when loading very big models, memory usage goes above 1GB
    # and Visual Studio compiler throws exceptions without this option
    set_property(TARGET ${appName} APPEND PROPERTY LINK_FLAGS /LARGEADDRESSAWARE)
endif()
    

