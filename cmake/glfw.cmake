# Try to download and compile GLFW library
# Once done this will define
#
# 1. GLFW_INCLUDE_DIR
# 2. GLFW_LIBRARIES

find_package(GLFW 3.2 QUIET)

if (GLFW_FOUND)
    message(STATUS "Found GLFW")
else ()
    message(STATUS "GLFW not found! Trying to download and build from the source.")
    
    set(GLFW_INSTALL_LOCATION "${PROJECT_BINARY_DIR}/external/glfw")

    include(ExternalProject)
    ExternalProject_Add(external_glfw
            URL https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
            CMAKE_ARGS "${CMAKE_ARGS};"
                    "-DCMAKE_INSTALL_PREFIX=${GLFW_INSTALL_LOCATION};"
                    "-DGLFW_BUILD_DOCS=OFF;"
                    "-DGLFW_BUILD_TESTS=OFF;"
                    "-DGLFW_BUILD_EXAMPLES=OFF;"
            )
            
    add_library(glfw STATIC IMPORTED)
    add_dependencies(glfw external_glfw)
    set(GLFW_INCLUDE_DIR ${GLFW_INSTALL_LOCATION}/include)
    set(glfw_lib_name ${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})
    set_property(TARGET glfw PROPERTY  IMPORTED_LOCATION "${GLFW_INSTALL_LOCATION}/lib/${glfw_lib_name}")
    set(GLFW_LIBRARIES glfw)    
    if (UNIX)
        list(APPEND GLFW_LIBRARIES ${GLFW_x11_LIBRARY})
    endif ()
endif ()
