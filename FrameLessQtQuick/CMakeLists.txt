cmake_minimum_required(VERSION 3.23)

# Project Name
project("FrameLessQtQuick")
 
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)

# This project uses C++17
set(CMAKE_CXX_STANDARD 17)

# This path should be set to your directory path where Qt5Config.cmake (or Qt6Config.cmake) is located. 
set(QT_DIR "D:\\ProgramFiles\\Qt\\5.15.10\\msvc2022_64\\lib\\cmake\\Qt5")

# Qt modules
find_package(QT NAMES Qt6 Qt5 REQUIRED Core)

set(Qt${QT_VERSION_MAJOR}_DIR ${QT_DIR})

find_package(Qt${QT_VERSION_MAJOR} REQUIRED Qml Quick QuickControls2)

file(GLOB SRC_FILES CONFIGURE_DEPENDS ./*.cpp)

file(GLOB UI_FILES CONFIGURE_DEPENDS ./*.ui)

file(GLOB QRC_FILES CONFIGURE_DEPENDS ./*.qrc)

# This project is for Windows OS. (I didn't test it in other OS like mac, linux)
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC_FILES} ${UI_FILES} ${QRC_FILES})

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    dwmapi.lib
    gdi32.lib
)

# For deployment.
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
     COMMAND windeployqt.exe $<IF:$<CONFIG:Debug>,--debug,--release> --qmldir "${CMAKE_SOURCE_DIR}/Qmls" "${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
     WORKING_DIRECTORY "${QT_DIR}/../../../bin"
)
