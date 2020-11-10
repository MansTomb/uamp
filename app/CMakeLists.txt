cmake_minimum_required(VERSION 3.16.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(TAGLIB_PATH "../3dparty/taglib/taglib")

set(TARGET uamp)

find_library(TAGLIB taglib)
find_package(Qt5 COMPONENTS Sql Widgets Network NetworkAuth Multimedia MultimediaWidgets Svg REQUIRED )

set(SOURCES main.cpp
    src/mainwindow.cpp
    src/LoginWindow.cpp
    src/PlayerView.cpp
    src/PlaylistView.cpp
    src/ImageFile.cpp
    src/FileTag.cpp
    src/GoogleAuthWrapper.cpp
    src/playerlistitem.cpp
    src/PlayerController.cpp
    src/ToolBar.cpp
    src/SideMenu.cpp
    src/PlaylistListView.cpp
    src/MenuPlaylistItemView.cpp
    src/EditFileTagsDialog.cpp
    src/ImportPlaylistView.cpp
    src/SongTimeline.cpp
    src/SqlDatabase.cpp
        src/Equalizer.cpp
    )

set(HEADERS src/mainwindow.h
    src/LoginWindow.h
    src/PlayerView.h
    src/PlaylistView.h
    src/ImageFile.h
    src/FileTag.h
    src/GoogleAuthWrapper.h
    src/playerlistitem.h
    src/PlayerController.h
    src/ToolBar.h
    src/SideMenu.h
    src/PlaylistListView.h
    src/MenuPlaylistItemView.h
    src/EditFileTagsDialog.h
    src/ImportPlaylistView.h
    src/SongTimeline.h
    src/SqlDatabase.h
        src/Equalizer.h
    )

set(UI src/mainwindow.ui
    src/LoginWindow.ui
    src/playerlistitem.ui
    src/PlayerView.ui
    src/ToolBar.ui
    src/SideMenu.ui
    src/PlaylistListView.ui
    src/MenuPlaylistItemView.ui
    src/EditFileTagsDialog.ui
    src/ImportPlaylistView.ui
        src/Equalizer.ui
    )

include_directories(../cmake-build-debug/app/uamp_autogen/include)
include_directories(
    ./src
    ../build/3dparty/taglib
    ../cmake-build-debug
    ../cmake-build-debug/3dparty/taglib/
    ${TAGLIB_PATH}/ape
    ${TAGLIB_PATH}/asf
    ${TAGLIB_PATH}/flac
    ${TAGLIB_PATH}/it
    ${TAGLIB_PATH}/mod
    ${TAGLIB_PATH}/mp4
    ${TAGLIB_PATH}/mpc
    ${TAGLIB_PATH}/mpeg
    ${TAGLIB_PATH}/mpeg/id3v2
    ${TAGLIB_PATH}/mpeg/id3v2/frames
    ${TAGLIB_PATH}/ogg
    ${TAGLIB_PATH}/riff
    ${TAGLIB_PATH}/s3m
    ${TAGLIB_PATH}/toolkit
    ${TAGLIB_PATH}/trueaudio
    ${TAGLIB_PATH}/wavpack
    ${TAGLIB_PATH}/xm
    ${TAGLIB_PATH}
)

link_directories(${CMAKE_SOURCE_DIR}/3dparty/BASS)
include_directories(../3dparty/BASS)

qt5_add_resources(SOURCES res/resources.qrc)

add_executable(${TARGET} ${SOURCES} ${HEADERS} ${UI})

add_subdirectory(src)
include_directories(./src)

add_custom_command(TARGET ${TARGET}
        PRE_LINK
        COMMAND "${CMAKE_SOURCE_DIR}/build.sh"
        )

target_link_libraries(${TARGET} tag Qt5::Sql Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::NetworkAuth
        Qt5::Multimedia Qt5::MultimediaWidgets Qt5::Svg bass)

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
