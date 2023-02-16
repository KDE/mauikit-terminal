project(KTerminalLib)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_definitions(-DHAVE_POSIX_OPENPT -DHAVE_SYS_TIME_H)

set(lib_SRCS
    BlockArray.h
    CharacterColor.h
    Character.h
    ColorScheme.h
    ColorTables.h
    DefaultTranslatorText.h
    Emulation.h
    ExtendedDefaultTranslator.h
    Filter.h
    History.h
    HistorySearch.h
    KeyboardTranslator.h
    konsole_wcwidth.h
    kprocess.h
    kptydevice.h
    kpty.h
    kpty_p.h
    kptyprocess.h
    LineFont.h
    Pty.h
    ProcessInfo.h
    Screen.h
    ScreenWindow.h
    Session.h
    ShellCommand.h
    TerminalCharacterDecoder.h
    TerminalDisplay.h
    tools.h
    Vt102Emulation.h
    mac-vkcode.h
    BlockArray.cpp
    ColorScheme.cpp
    Emulation.cpp
    Filter.cpp
    History.cpp
    HistorySearch.cpp
    KeyboardTranslator.cpp
    konsole_wcwidth.cpp
    kprocess.cpp
    kpty.cpp
    kptydevice.cpp
    kptyprocess.cpp
    ProcessInfo.cpp
    Pty.cpp
    Screen.cpp
    ScreenWindow.cpp
    Session.cpp
    ShellCommand.cpp
    TerminalCharacterDecoder.cpp
    TerminalDisplay.cpp
    tools.cpp
    Vt102Emulation.cpp

)

add_library(${PROJECT_NAME} STATIC
    ${lib_SRCS}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Quick
    Qt5::Gui
    Qt5::Widgets
)


install(DIRECTORY color-schemes DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/terminal)
install(DIRECTORY kb-layouts DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/terminal)
