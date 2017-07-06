# Try to download and compile GLFW library
# Once done this will define
#
# 1. GLFW_INCLUDE_DIR
# 2. GLFW_LIBRARIES

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

if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)

    find_package(X11 REQUIRED)

    if(NOT X11_Xrandr_FOUND)
        message(FATAL_ERROR "Xrandr library not found - required for GLFW")
    endif()

    if(NOT X11_xf86vmode_FOUND)
        message(FATAL_ERROR "xf86vmode library not found - required for GLFW")
    endif()

    if(NOT X11_Xcursor_FOUND)
        message(FATAL_ERROR "Xcursor library not found - required for GLFW")
    endif()

    if(NOT X11_Xinerama_FOUND)
        message(FATAL_ERROR "Xinerama library not found - required for GLFW")
    endif()

    if(NOT X11_Xi_FOUND)
        message(FATAL_ERROR "Xi library not found - required for GLFW")
    endif()

    list(APPEND GLFW_x11_LIBRARY "${X11_Xrandr_LIB}" "${X11_Xxf86vm_LIB}" "${X11_Xcursor_LIB}" "${X11_Xinerama_LIB}" "${X11_Xi_LIB}" "${X11_LIBRARIES}" "${CMAKE_THREAD_LIBS_INIT}" -lrt -ldl)
    
    list(APPEND GLFW_LIBRARIES ${GLFW_x11_LIBRARY})
endif ()

