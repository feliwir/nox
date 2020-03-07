find_package(ffmpeg REQUIRED)

add_library(nox-core STATIC Media.cpp Media.hpp)
target_include_directories(nox-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(nox-core ${FFMPEG_LIBRARIES})