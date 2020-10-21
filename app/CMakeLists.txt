cmake_minimum_required(VERSION 3.16.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(TARGET uamp)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES main.cpp src/mainwindow.cpp)
set(HEADERS src/mainwindow.h)
set(UI src/mainwindow.ui)

include_directories(../cmake-build-debug/app/uamp_autogen/include)

qt5_add_resources(SOURCES res/resources.qrc)

add_executable(${TARGET} ${SOURCES} ${HEADERS} ${UI})

add_subdirectory(src)
include_directories(./src)

target_link_libraries(${TARGET} Qt5::Gui Qt5::Core Qt5::Widgets)

set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../..)
#cmake_minimum_required(VERSION 3.5)
#
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#
#include_directories(./src/)
#include_directories(../cmake-build-debug/app/src/src_autogen/include)
#
#add_compile_options(
#    -std=c++17
##    -Werror
##    -Wall
##    -Wextra
##    -Wpedantic
##    -Wno-unused
#)
#
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#
#find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core Gui REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Gui REQUIRED)
#
#add_subdirectory(src)
#
#qt5_add_resources(src res/resources.qrc)
##qt5_add_resources(main.cpp res/resources.qrc)
#
#add_executable(${CMAKE_PROJECT_NAME} main.cpp)
#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core
#        Qt${QT_VERSION_MAJOR}::Widgets
#        Qt${QT_VERSION_MAJOR}::Gui
#        src
#)
#
#set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../..)
