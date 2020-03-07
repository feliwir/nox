# CMakeLists.txt
cmake_minimum_required(VERSION 3.7)
project(nox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

find_package(GTest CONFIG REQUIRED)