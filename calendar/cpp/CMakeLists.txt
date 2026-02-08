cmake_minimum_required(VERSION 3.10)
project(Calendar)

set(CMAKE_CXX_STANDARD 20)

# Find all .cpp files automatically
file(GLOB SOURCES "*.cpp")

add_executable(Calendar ${SOURCES})
