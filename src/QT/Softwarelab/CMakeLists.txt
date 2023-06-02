cmake_minimum_required(VERSION 3.5)

project(Softwarelab VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        seg_menu.cpp
        seg_menu.h
        seg_menu.ui
        batch_menu.h
        batch_menu.cpp
        batch_menu.ui
        save_menu.h
        save_menu.cpp
        save_menu.ui
        compare_menu.h
        compare_menu.cpp
        compare_menu.ui
        load_scan_menu.h
        load_scan_menu.cpp
        load_scan_menu.ui
		seg_option_menu.h
		seg_option_menu.cpp
		seg_option_menu.ui
		scan_option_menu.h 
		scan_option_menu.cpp 
		scan_option_menu.ui
		../../Core/batch_mode.cpp
		../../Core/batch_mode_qt.h
		../../Core/Disjoint_Set.cpp
		../../Core/Disjoint_Set.h
		../../Core/Nifti.cpp
		../../Core/Nifti.h
		../../Core/Scan.cpp
		../../Core/Scan.h
		../../Core/Segmentation.cpp
		../../Core/Segmentation.h
		../../Core/gaussian_blur.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Softwarelab
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Softwarelab APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Softwarelab SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Softwarelab
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Softwarelab PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Softwarelab PUBLIC OpenMP::OpenMP_CXX)
endif()

set_target_properties(Softwarelab PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Softwarelab
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Softwarelab)
endif()
