cmake_minimum_required(VERSION ${CMAKE_MINIMUM_VERSION})
project(Glad)

include_directories(${PROJECT_SOURCE_DIR}/include)

include(ucm)
ucm_add_dirs(lib TO SRC_LIST)
add_library(${PROJECT_NAME} ${SRC_LIST})

if (UNIX AND NOT APPLE)
	find_library(LIB_DL dl)	
	set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_LINK_LIBRARIES "${LIB_DL}")
endif ()
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "lib/glad")
if (DEFAULT_TARGET_OPTIONS)
	set_target_properties(TARGET ${PROJECT_NAME}} PROPERTIES ${DEFAULT_TARGET_OPTIONS})
endif ()
set(GLAD_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE)
set(GLAD_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)