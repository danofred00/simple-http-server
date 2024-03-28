cmake_minimum_required(VERSION 3.14)

project(Simple-Http-Server LANGUAGES CXX VERSION 0.1.0)

#######################################

set(CMAKE_PROJECT_DESCRIPTION "Your Web Server for sharing files with others.")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/danofred00/simple-http-server")
set(CMAKE_PROJECT_VERSION 0.1.0)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 1)
set(CMAKE_PROJECT_VERSION_PATCH 0)

######################################

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core HttpServer Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core HttpServer Widgets Network)

########################################

configure_file(
    ${CMAKE_SOURCE_DIR}/version.h.in
    ${CMAKE_SOURCE_DIR}/src/version.h
    USE_SOURCE_PERMISSIONS)

########################################

add_subdirectory(cmake)
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(assets)
