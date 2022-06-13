
# 修改为自己 opengl 所在路径，其他不变
set(prefix "D:/libs/glew-2.1.0")
set(glfw_prefix "D:/libs/glfw-3.3.7.bin.WIN64")
set(OpenGL_PREFIX "${prefix}")
set(OpenGL_LIBDIR "${OpenGL_PREFIX}/lib/Release/x64")
set(glfw_libdir "${glfw_prefix}/lib-mingw-w64")
set(OpenGL_INCLUDE_DIRS "${OpenGL_PREFIX}/include" "${glfw_prefix}/include")
set(OpenGL_LIBRARIES "-L${OpenGL_LIBDIR} -lglew32  -lglew32s -lopengl32 -lglu32 -L${glfw_libdir} -lglfw3")