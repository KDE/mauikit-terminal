# SPDX-FileCopyrightText: 2020 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/code)

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS -DQT_NO_CAST_FROM_BYTEARRAY)

if (QT_MAJOR_VERSION STREQUAL "6")
    add_library(MauiKitTerminal${MAUI_MAJOR_VERSION} SHARED)
endif()

ecm_add_qml_module(MauiKitTerminal${MAUI_MAJOR_VERSION}
    URI "org.mauikit.terminal"
    CLASS_NAME TerminalPlugin
    NO_PLUGIN_OPTIONAL)

set(plugin_SRCS
    code/colorschemesmodel.cpp
    code/moduleinfo.cpp)

set(plugin_HDRS
    code/moduleinfo.h)

if (QT_MAJOR_VERSION STREQUAL "5")
    if(QUICK_COMPILER)
        qtquick_compiler_add_resources(lib_QML_QRC resources.qrc)
    else()
        qt5_add_resources(lib_QML_QRC resources.qrc)
    endif()
endif()

ecm_target_qml_sources(MauiKitTerminal${MAUI_MAJOR_VERSION} SOURCES
    controls.${QT_MAJOR_VERSION}/Terminal.qml
    controls.${QT_MAJOR_VERSION}/ColorSchemesPage.qml
    controls.${QT_MAJOR_VERSION}/QMLTermScrollbar.qml)

ecm_target_qml_sources(MauiKitTerminal${MAUI_MAJOR_VERSION} PATH private SOURCES
    controls.${QT_MAJOR_VERSION}/private/TerminalInputArea.qml
    controls.${QT_MAJOR_VERSION}/private/TerminalScrollBar.qml)

# add_subdirectory(lib)

target_sources(MauiKitTerminal${MAUI_MAJOR_VERSION}
    PRIVATE    
    ${plugin_SRCS})
    
    target_sources(MauiKitTerminal${MAUI_MAJOR_VERSION}plugin
    PRIVATE    
    code/terminal_plugin.cpp
    ${plugin_SRCS})

add_library(MauiKit${MAUI_MAJOR_VERSION}::Terminal ALIAS MauiKitTerminal${MAUI_MAJOR_VERSION})

if (QT_MAJOR_VERSION STREQUAL "5")
    if(QUICK_COMPILER)
        target_sources(MauiKitTerminal${MAUI_MAJOR_VERSION} PRIVATE ${lib_QML_QRC})

        add_definitions(-DQUICK_COMPILER)
        target_compile_definitions(MauiKitTerminal${MAUI_MAJOR_VERSION} PUBLIC QUICK_COMPILER)
    endif()
endif()

generate_export_header(MauiKitTerminal${MAUI_MAJOR_VERSION} BASE_NAME Terminal)
set_target_properties(MauiKitTerminal${MAUI_MAJOR_VERSION} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME "Terminal")

target_include_directories(MauiKitTerminal${MAUI_MAJOR_VERSION}
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Terminal>")

target_include_directories(MauiKitTerminal${MAUI_MAJOR_VERSION} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

target_link_libraries(MauiKitTerminal${MAUI_MAJOR_VERSION}plugin
    PRIVATE
    KTerminalLib)
    
target_link_libraries(MauiKitTerminal${MAUI_MAJOR_VERSION}
    PRIVATE
    KTerminalLib
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::Widgets
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::ConfigCore
    KF${KF_MAJOR_VERSION}::ConfigGui
    MauiKit${MAUI_MAJOR_VERSION})


ecm_finalize_qml_module(MauiKitTerminal${MAUI_MAJOR_VERSION} DESTINATION ${KDE_INSTALL_QMLDIR})
# ecm_generate_qmltypes(org.mauikit.terminal 3.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/terminal)

install(TARGETS MauiKitTerminal${MAUI_MAJOR_VERSION} EXPORT MauiKitTerminal${MAUI_MAJOR_VERSION}Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${plugin_HDRS}
    ${CMAKE_CURRENT_BINARY_DIR}/terminal_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Terminal
    COMPONENT Devel)
