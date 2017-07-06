 find_package(GLM QUIET)
 
 if (GLM_FOUND)
    message(STATUS "GLM found")
else ()
    include (DownloadGLM)
 endif ()
