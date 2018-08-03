cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

set(DXLIB_DIR C:/DxLib_VC)
set(DXLIB_FRAMEWORK_DIR ${CMAKE_CURRENT_BINARY_DIR}/DxLibGameFramework/src/DxLibGameFramework)

# GravityDelivery Project
set(PROJ_NAME GravityDelivery)
project(${PROJ_NAME} CXX)

include_directories(${DXLIB_DIR}/プロジェクトに追加すべきファイル_VC用)
include_directories(${DXLIB_FRAMEWORK_DIR})
link_directories(${DXLIB_DIR}/プロジェクトに追加すべきファイル_VC用)
link_directories(${DXLIB_FRAMEWORK_DIR})
file(GLOB_RECURSE SOURCES
	Game/*.h
	Game/*.cpp
	WinMain.cpp
)
add_executable(${PROJ_NAME} WIN32 ${SOURCES} Resources/Icon/Icon.rc)
set_target_properties(${PROJ_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(MSVC)
	add_definitions(/FI"${DXLIB_FRAMEWORK_DIR}/GameFramework.h")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /W3 /EHsc")
else()
	add_definitions(-include ${DXLIB_FRAMEWORK_DIR}/GameFramework.h)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/Resources)
file(COPY Resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resources)

include(ExternalProject)
ExternalProject_Add(
    DxLibGameFramework
    PREFIX "DxLibGameFramework"
    URL https://ydeagames.github.io/DxLibGameFramework/GameFramework.tar.gz
    URL_HASH SHA512=562ce85507fda7072d5ecadf76bd022d26178c9957ec9d945e0fb935bb4a85b822907189a2acd59d5e76929e4fc4ebec09dbbc6ec99d5436d5271c38d8c8b860
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
if(NOT EXISTS "${DXLIB_DIR}")
    ExternalProject_Add(
        DxLib
        PREFIX "DxLib"
        URL http://dxlib.o.oo7.jp/DxLib/DxLib_VC3_19b.zip
        URL_HASH SHA512=4d19f3aa752c53b92f8f077ebeaed1a82353a9687dcb482b57cdaafcc3c707f66e4d062ea0782055c11c2b984ba96625f9225cbb2913bbe679587481d2a5ab3e
        SOURCE_DIR ${DXLIB_DIR}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )
endif()