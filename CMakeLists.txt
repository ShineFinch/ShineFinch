cmake_minimum_required(VERSION 3.16)

project(ShineFinch VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)


include(FetchContent)
FetchContent_Declare(
    QHotkey
    GIT_REPOSITORY https://github.com/Skycoder42/QHotkey.git
    GIT_TAG 1.5.0
    GIT_SHALLOW TRUE
)
set(QT_DEFAULT_MAJOR_VERSION 6 CACHE STRING "Qt version for QHotkey" FORCE)
FetchContent_MakeAvailable(QHotkey)



find_package(Qt6 REQUIRED COMPONENTS Quick Widgets QuickWidgets Concurrent )
find_package(Tesseract CONFIG REQUIRED)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(ShineFinch
    main.cpp
)


qt_add_qml_module(ShineFinch
    URI ShineFinch
    QML_FILES
        Main.qml
        SelectorWindowOverlay.qml
    RESOURCES
        res.qrc
    SOURCES
        src/ShortcutManager.h
        src/ShortcutManager.cpp
        src/Screenshot.h
        src/Screenshot.cpp
        src/SelectorWindow.h
        src/SelectorWindow.cpp
        RESOURCES vcpkg.json
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(ShineFinch PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ShineFinch
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(ShineFinch
    PRIVATE
        Qt6::Quick
        Qt6::Widgets
        Qt6::QuickWidgets
        Qt6::Concurrent
        QHotkey::QHotkey
        Tesseract::libtesseract
)


install(TARGETS ShineFinch
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
