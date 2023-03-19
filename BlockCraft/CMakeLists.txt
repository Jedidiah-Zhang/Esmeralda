cmake_minimum_required(VERSION 3.5)

project(BlockCraft VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Bluetooth Charts Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Bluetooth Charts Widgets)

set(PROJECT_SOURCES
    main.cpp
    pushbutton.h
    pushbutton.cpp
    mainwindow.cpp
    mainwindow.h
    menu.h
    menu.cpp
    levelselect.h
    levelselect.cpp
    build.h
    build.cpp
    design.h
    design.cpp
    games.h
    games.cpp
    progress.h
    progress.cpp
    bluetooth.h
    bluetooth.cpp
    open3dmodel.h
    open3dmodel.cpp
    structs.h
    mainwindow.ui
    res.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BlockCraft
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET BlockCraft APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BlockCraft SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(BlockCraft
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(BlockCraft PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Bluetooth
    Qt${QT_VERSION_MAJOR}::Charts
)

set_target_properties(BlockCraft PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS BlockCraft
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BlockCraft)
endif()
