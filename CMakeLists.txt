cmake_minimum_required(VERSION 3.5)

project(OpenRCT2Launcher VERSION 0.0.7 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)
find_package(Qt5 OPTIONAL_COMPONENTS Zlib)
if (Qt5Zlib_FOUND)
        set(ZLIB_LINK Qt5::Zlib)
else()
        find_package(ZLIB REQUIRED)
        set(ZLIB_LINK ZLIB::ZLIB)
endif()

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/configuration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/configuration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/configuration.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/configuration_data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/updater.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/updater.h
)

if (UNIX AND NOT APPLE)
    list(APPEND PROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/archives/gzipreadfilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/archives/gzipreadfilter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/archives/tarextractor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/archives/tarextractor.h
    )
else()
    list(APPEND PROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/archives/zipextractor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/archives/zipextractor.h
    )
endif()

if (WIN32)
    list(APPEND PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/icon/openrct2launcher.rc)
elseif(APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_COPYRIGHT "© LRFLEW - Released Under the MIT License")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.lrflew.OpenRCT2")
    set(MACOSX_BUNDLE_ICON_FILE openrct2.icns)
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})

    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/icon/openrct2.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")
    list(APPEND PROJECT_SOURCES ${APP_ICON_MACOSX})
endif()

add_executable(OpenRCT2Launcher WIN32 MACOSX_BUNDLE ${PROJECT_SOURCES})
target_compile_definitions(OpenRCT2Launcher PUBLIC APP_VERSION="${PROJECT_VERSION}")

target_link_libraries(OpenRCT2Launcher PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network ${ZLIB_LINK})
