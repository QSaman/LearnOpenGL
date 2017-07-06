set(GLM_INSTALL_LOCATION "${PROJECT_BINARY_DIR}/external/glm")

include(ExternalProject)
ExternalProject_Add(external_glm
        URL https://github.com/g-truc/glm/releases/download/0.9.8.4/glm-0.9.8.4.zip
        CMAKE_ARGS "${CMAKE_ARGS};"
                "-DCMAKE_INSTALL_PREFIX=${GLM_INSTALL_LOCATION}")

                
set(GLM_INCLUDE_DIR ${GLM_INSTALL_LOCATION}/include)
