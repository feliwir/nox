find_package(FFMPEG REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include(GenerateExportHeader)
add_library(nox-core SHARED
	IStream.hpp
	ITextureInterop.hpp
	Media.cpp Media.hpp
	Texture.cpp Texture.hpp
	VideoStream.cpp VideoStream.hpp)

if(WIN32)
target_sources(nox-core	PRIVATE
	DXTextureInterop.cpp DXTextureInterop.hpp)
endif()

generate_export_header(nox-core)

target_include_directories(nox-core PUBLIC 
	${CMAKE_CURRENT_BINARY_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${FFMPEG_INCLUDE_DIRS})

target_link_libraries(nox-core
    ${GLEW_LIBRARIES}
    OpenGL::GL
	${FFMPEG_LIBRARIES})

link_directories(${FFMPEG_LIBRARY_DIRS})