project(KTerminalLib)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR})

find_package(KF${KF_MAJOR_VERSION} REQUIRED COMPONENTS Pty)

if (${QT_MAJOR_VERSION} EQUAL 6)
     find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
endif()

# this lib uses them
add_definitions(-DHAVE_POSIX_OPENPT -DHAVE_SYS_TIME_H)
remove_definitions(-DQT_NO_SIGNALS_SLOTS_KEYWORDS)
remove_definitions(-DQT_NO_KEYWORDS)
remove_definitions(-DQT_NO_CAST_TO_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

set(lib_SRCS

    BlockArray.cpp
    ColorScheme.cpp
    customcolorscheme.cpp
    Emulation.cpp
    Filter.cpp
    History.cpp
    HistorySearch.cpp
    KeyboardTranslator.cpp
    ProcessInfo.cpp
    Pty.cpp
    Screen.cpp
    ScreenWindow.cpp
    Session.cpp
    ksession.cpp
    ShellCommand.cpp
    TerminalCharacterDecoder.cpp
    TerminalDisplay.cpp
    tools.cpp
    Vt102Emulation.cpp)

add_library(${PROJECT_NAME}  STATIC)
target_sources(${PROJECT_NAME}  PRIVATE ${lib_SRCS})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::Widgets
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::ConfigCore
    KF${QT_MAJOR_VERSION}::Pty)

if (${QT_MAJOR_VERSION} EQUAL 6)
    target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Core5Compat)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(DIRECTORY color-schemes DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/terminal)
install(DIRECTORY kb-layouts DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/terminal)
