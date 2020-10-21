cmake_minimum_required(VERSION 3.17)
project(uamp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(3dparty)
add_subdirectory(app)
