cmake_minimum_required(VERSION 3.22.2)

project(GUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_ASSUME_STDERR_HAS_CONSOLE 1)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(
  PROJECT_SOURCES
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  dialog.h
  dialog.cpp
  dialog.ui
  annotation.h
  annotation.cpp
  annotation.ui
  containerbutton.h
  containerbutton.cpp
  loadcontainer.cpp
  loadcontainer.h
  loadcontainer.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(
    GUI
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
  )
  # Define target properties for Android with Qt 6 as:
  #    set_property(TARGET GUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
  # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(
      GUI SHARED
      ${PROJECT_SOURCES}
    )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(
      GUI
      ${PROJECT_SOURCES}
    )
  endif()
endif()

target_link_libraries(GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(GUI PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(GUI PRIVATE Solver)
set_target_properties(
  GUI PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(GUI)
endif()
