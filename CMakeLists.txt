project(LearnOpenGL)
cmake_minimum_required(VERSION 3.6)

# Allow subfolders in solution file
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ucm)

ucm_add_dirs(lib TO SRC_LIST)

set(GLAD_LIB_NAME glad)
add_library(${GLAD_LIB_NAME} ${SRC_LIST})
set_target_properties(${GLAD_LIB_NAME} PROPERTIES FOLDER "lib")
include_directories(${PROJECT_SOURCE_DIR}/include)
set(externalLibs ${externalLibs} ${GLAD_LIB_NAME})


if(MSVC)
	# The following line needs at least version 3.6
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

################################################################################
if (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    OPTION(USE_System_GLFW "Use GLFW externally (recommended for platforms which support pkgconfig like Linux" ON)
    OPTION(USE_System_GLM "Use GLM externally (recommended for platforms like Linux" ON)
	find_library(dl_lib dl)
	set(externalLibs ${externalLibs} ${dl_lib})
else (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    OPTION(USE_System_GLFW "Use GLFW externally (recommended for platforms which support pkgconfig like Linux" OFF)
    OPTION(USE_System_GLM "Use GLM externally (recommended for platforms like Linux" OFF)
endif (${CMAKE_SYSTEM_NAME} STREQUAL Linux)    
################################################################################

# Adding header files recursively. aux_source_directory doesn't do that very well
#file(GLOB_RECURSE headerFilesList "*.h" "*.hpp")
#add_custom_target(headerFiles SOURCES ${headerFilesList})

################################################################################
# Enabling C++11. Add these two lines after add_executable. Notice you must 
# Enable C++11 for every target in all your CMake scripts.
set_property(TARGET ${GLAD_LIB_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${GLAD_LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

################################################################################

if (${USE_System_GLFW})
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    set(externalLibs ${externalLibs} ${GLFW_LIBRARIES})
else (${USE_System_GLFW})
    include(glfw)
    include_directories(${GLFW_INCLUDE_DIR})
    set(externalLibs ${externalLibs} ${GLFW_LIBRARIES})
endif (${USE_System_GLFW})

if (NOT ${USE_System_GLM})
#    include_directories(${PROJECT_SOURCE_DIR}/external/glm)
endif (NOT ${USE_System_GLM})
	 
add_subdirectory(Chapters)
