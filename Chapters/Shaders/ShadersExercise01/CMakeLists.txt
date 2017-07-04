project(ShadersExercise01)
cmake_minimum_required(VERSION 3.6)

set(BIN_DIR "${CHAPTER_BIN_DIR}/${PROJECT_NAME}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
include(ucm)

set(SRC_LIST)
ucm_add_dirs(src TO SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES 
	FOLDER "Chapters/${DIR_NAME}"
	RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

################################################################################
# Enabling C++11. Add these two lines after add_executable. Notice you must 
# Enable C++11 for every target in all your CMake scripts.
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

################################################################################

target_link_libraries(${PROJECT_NAME} ${externalLibs})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PROJECT_SOURCE_DIR}/shaders" "${BIN_DIR}/$<CONFIG>/shaders")	 

