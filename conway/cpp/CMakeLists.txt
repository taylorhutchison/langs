cmake_minimum_required(VERSION 3.10)
project(Conway)

set(CMAKE_CXX_STANDARD 17)

# Find all .cpp files automatically
file(GLOB SOURCES "*.cpp")

add_executable(Conway ${SOURCES})