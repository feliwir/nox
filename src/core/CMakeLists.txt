find_package(FFMPEG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include(GenerateExportHeader)
add_library(nox-core SHARED
	DXTextureInterop.cpp DXTextureInterop.hpp
	IStream.hpp
	ITextureInterop.hpp
	Media.cpp Media.hpp
	Texture.cpp Texture.hpp
	VideoStream.cpp VideoStream.hpp)
generate_export_header(nox-core)

target_include_directories(nox-core PUBLIC 
	${CMAKE_CURRENT_BINARY_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${FFMPEG_INCLUDE_DIRS})

target_link_libraries(nox-core
    GLEW::glew 
    OpenGL::GL
	${FFMPEG_LIBRARIES})

link_directories(${FFMPEG_LIBRARY_DIRS})