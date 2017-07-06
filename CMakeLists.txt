project(LearnOpenGL)
cmake_minimum_required(VERSION 3.6)

# Allow subfolders in solution file
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ucm)

ucm_add_dirs(lib TO SRC_LIST)

set(GLAD_LIB_NAME glad)
add_library(${GLAD_LIB_NAME} ${SRC_LIST})
if (UNIX AND NOT APPLE)
    find_library(LIB_DL dl)
    target_link_libraries(${GLAD_LIB_NAME} ${LIB_DL})
endif ()
set_target_properties(${GLAD_LIB_NAME} PROPERTIES FOLDER "lib")
include_directories(${PROJECT_SOURCE_DIR}/include)
set(externalLibs ${externalLibs} ${GLAD_LIB_NAME})

################################################################################

if (UNIX AND NOT APPLE)
    set(DEFAULT_DOWNLOAD OFF)
else ()
    set(DEFAULT_DOWNLOAD ON)
endif ()

OPTION(DOWNLOAD_GLFW "Download GLFW. Recommended for Windows and Apple. In Linux use your package manager to install it." ${DEFAULT_DOWNLOAD})
OPTION(DOWNLOAD_GLM "Download GLM. Recommended for Windows and Apple. In Linux use your package manager to install it." ${DEFAULT_DOWNLOAD})

################################################################################

# Enabling C++11. Add these two lines after add_executable. Notice you must 
# Enable C++11 for every target in all your CMake scripts.
set_property(TARGET ${GLAD_LIB_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${GLAD_LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

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
	 
add_subdirectory(Chapters)
