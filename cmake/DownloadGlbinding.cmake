# Try to download and compile glbinding library
# Once done this will define
#
# 1. GLBINDING_INCLUDE_DIR
# 2. GLBINDING_LIBRARIES

set(GLBINDING_INSTALL_LOCATION "${PROJECT_BINARY_DIR}/external/glbinding")

#There is a bug in 2.1.3 which prevent us to compile it in Windows. Here is a simple workaround:
if (WIN32)
	set(glbinding_url "https://github.com/cginternals/glbinding/archive/v2.1.2.zip")
else ()
	set(glbinding_url "https://github.com/cginternals/glbinding/archive/v2.1.3.zip")
endif ()

include(ExternalProject)
ExternalProject_Add(external_glbinding
        URL ${glbinding_url}
        CMAKE_ARGS "${CMAKE_ARGS};"
                "-DCMAKE_INSTALL_PREFIX=${GLBINDING_INSTALL_LOCATION};"
                "-DBUILD_SHARED_LIBS=OFF;"
                "-DOPTION_BUILD_TESTS=OFF;"
                "-DOPTION_BUILD_GPU_TESTS=OFF;"
                "-DOPTION_BUILD_DOCS=OFF;"
                "-DOPTION_BUILD_TOOLS=OFF;"
                "-DOPTION_BUILD_EXAMPLES=OFF;"
                "-DOPTION_BUILD_WITH_BOOST_THREAD=OFF;"
        ) 
		
add_library(glbinding STATIC IMPORTED)
add_dependencies(glbinding external_glbinding)
set(GLBINDING_INCLUDE_DIR ${GLBINDING_INSTALL_LOCATION}/include)

set(glbinding_debug_lib_name ${CMAKE_STATIC_LIBRARY_PREFIX}glbindingd${CMAKE_STATIC_LIBRARY_SUFFIX})
set(glbinding_lib_name ${CMAKE_STATIC_LIBRARY_PREFIX}glbinding${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET glbinding PROPERTY  IMPORTED_LOCATION_DEBUG "${GLBINDING_INSTALL_LOCATION}/lib/${glbinding_debug_lib_name}")
set_property(TARGET glbinding PROPERTY  IMPORTED_LOCATION "${GLBINDING_INSTALL_LOCATION}/lib/${glbinding_lib_name}")
set(GLBINDING_LIBRARIES glbinding)
if (WIN32)
	set_target_properties(glbinding PROPERTIES INTERFACE_LINK_LIBRARIES "glu32;opengl32")
else ()
    find_package(OpenGL REQUIRED)
    set_target_properties(glbinding PROPERTIES INTERFACE_LINK_LIBRARIES "${OPENGL_LIBRARIES}")
endif ()
