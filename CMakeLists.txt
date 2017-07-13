project(LearnOpenGL)
set(CMAKE_MINIMUM_VERSION 3.1)
cmake_minimum_required(VERSION ${CMAKE_MINIMUM_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ucm)
include(DefaultOptions)

include_directories(${PROJECT_SOURCE_DIR}/include)

################################################################################

if (UNIX AND NOT APPLE)
    set(DEFAULT_DOWNLOAD OFF)
    set(DEFAULT_OPENGL_LOADER "glbinding")
else ()
    set(DEFAULT_DOWNLOAD ON)
    set(DEFAULT_OPENGL_LOADER "Glad")
endif ()

OPTION(DOWNLOAD_GLFW "Download GLFW. Recommended for Windows and Apple. In Linux use your package manager to install it." ${DEFAULT_DOWNLOAD})
OPTION(DOWNLOAD_GLM "Download GLM. Recommended for Windows and Apple. In Linux use your package manager to install it." ${DEFAULT_DOWNLOAD})
set(OPENGL_LOADER "${DEFAULT_OPENGL_LOADER}" CACHE STRING "OpenGL loading library. It is recommended to use glbinding for Linux (it will be downloaded).")
set_property(CACHE OPENGL_LOADER PROPERTY STRINGS glbinding Glad)

################################################################################

if (${DOWNLOAD_GLFW})
    include(DownloadGLFW)
else ()
    include(glfw)
endif ()
include_directories(${GLFW_INCLUDE_DIR})
set(externalLibs ${externalLibs} ${GLFW_LIBRARIES})

if (${DOWNLOAD_GLM})
    include(DownloadGLM)
else ()
    include(glm)    
endif ()
include_directories(${GLM_INCLUDE_DIR})

if (${OPENGL_LOADER} STREQUAL "glbinding")
    add_definitions(-DUSE_GLBINDING)
    include(DownloadGlbinding)
    include_directories(${GLBINDING_INCLUDE_DIR})
    if (UNIX AND NOT APPLE)
        find_library(GL_LIB GL)
        set(externalLibs ${externalLibs} ${GL_LIB})
    endif ()
    set(externalLibs ${externalLibs} ${GLBINDING_LIBRARIES})
elseif (${OPENGL_LOADER} STREQUAL "Glad")
    add_definitions(-DUSE_GLAD)
	add_subdirectory(glad)
    include_directories(${GLAD_INCLUDE_DIR})
    set(externalLibs ${externalLibs} ${GLAD_LIBRARIES})
else ()
    message(FATAL_ERROR "Unknown OpenGL loading library.") 
endif ()
	 
add_subdirectory(Chapters)
