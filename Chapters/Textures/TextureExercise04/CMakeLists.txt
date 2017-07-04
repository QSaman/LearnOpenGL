project(TextureExercise04)
cmake_minimum_required(VERSION 3.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
include(ucm)

set(SRC_LIST)
ucm_add_dirs(src TO SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Chapters/Textures")

################################################################################
# Enabling C++11. Add these two lines after add_executable. Notice you must 
# Enable C++11 for every target in all your CMake scripts.
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

################################################################################

target_link_libraries(${PROJECT_NAME} ${externalLibs})

file(COPY ${PROJECT_SOURCE_DIR}/shaders/ DESTINATION ${PROJECT_BINARY_DIR}/shaders
     FILES_MATCHING PATTERN "*")	 

