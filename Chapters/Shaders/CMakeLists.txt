cmake_minimum_required(VERSION 3.6)

set(DIR_NAME Shaders)
set(CHAPTER_BIN_DIR "${PROJECTS_BIN_DIR}/${DIR_NAME}")

add_subdirectory(SetColorThroughVertexShader)
add_subdirectory(ShadersUniform)
add_subdirectory(ShaderInterpolation)
add_subdirectory(ShadersClass)
add_subdirectory(ShadersExercise01)
add_subdirectory(ShadersExercise02)
add_subdirectory(ShadersExercise03)

