cmake_minimum_required(VERSION 3.5)

project(Airport_Inspector_ver1.0 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Concurrent PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Concurrent PrintSupport)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        graph.h
        graph.cpp
        graph.ui
        qcustgraph.h
        qcustgraph.cpp
	qcustbarchart.h
	qcustbarchart.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Airport_Inspector_ver1.0
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        database.h database.cpp
        graph.h graph.cpp graph.ui
        qcustgraph.h qcustgraph.cpp
	qcustbarchart.h qcustbarchart.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET start_airport_pr APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
	add_library(Airport_Inspector_ver1.0 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
	add_executable(Airport_Inspector_ver1.0
            ${PROJECT_SOURCES}
        )
    endif()
endif()

#Необходимо при использовании динамической библиотеки QCustomPlot
add_compile_definitions(QCUSTOMPLOT_USE_LIBRARY)
#Добавляем путь где искать библиотеки
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Airport_Inspector_ver1.0 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                                      Qt6::Sql
                                                                      Qt6::Concurrent
                                                                      Qt6::PrintSupport
                                                                      qcustomplot2.dll)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Airport_Inspector_ver1.0)
endif()
set_target_properties(Airport_Inspector_ver1.0 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Airport_Inspector_ver1.0
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Airport_Inspector_ver1.0)
endif()
