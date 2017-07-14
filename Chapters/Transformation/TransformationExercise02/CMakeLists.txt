project(TransformationExercise02)
cmake_minimum_required(VERSION ${CMAKE_MINIMUM_VERSION})

set(BIN_DIR "bin/$<CONFIG>/${DIR_NAME}/${PROJECT_NAME}")

set(SRC_LIST)
ucm_add_dirs(src TO SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES
        ${DEFAULT_TARGET_OPTIONS}
	FOLDER "Chapters/${DIR_NAME}")

target_link_libraries(${PROJECT_NAME} ${externalLibs})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PROJECT_SOURCE_DIR}/shaders" "${PROJECT_BINARY_DIR}/shaders")
	
install(TARGETS ${PROJECT_NAME} DESTINATION "${BIN_DIR}")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/shaders" DESTINATION ${BIN_DIR}) 

