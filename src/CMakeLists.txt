include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/code
)

set(plugin_SRCS
    code/ksession.cpp
    code/colorschemesmodel.cpp
    code/customcolorscheme.cpp
    code/terminal_plugin.cpp
)

if(QUICK_COMPILER)
    qtquick_compiler_add_resources(lib_QML_QRC resources.qrc)
else()
    qt5_add_resources(lib_QML_QRC resources.qrc)
endif()

ecm_generate_qmltypes(org.mauikit.terminal ${PROJECT_VERSION} DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/terminal)

add_subdirectory(lib)

add_library(MauiKitTerminal
    SHARED
    ${plugin_SRCS}
    ${lib_QML_QRC}
)

add_library(MauiKit::Terminal ALIAS MauiKitTerminal)

generate_export_header(MauiKitTerminal BASE_NAME Terminal)
set_target_properties(MauiKitTerminal PROPERTIES
    VERSION ${Terminal_VERSION_STRING}
    SOVERSION ${Terminal_SOVERSION}
    EXPORT_NAME Terminal
    )

target_include_directories(MauiKitTerminal
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiKit/Terminal>")

target_include_directories(MauiKitTerminal PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

target_link_libraries(MauiKitTerminal
    PRIVATE
    KTerminalLib
    Qt5::Core
    Qt5::Quick
    Qt5::Gui
    Qt5::Widgets
    KF5::ConfigCore
    KF5::ConfigGui
    MauiKit
)

install(TARGETS MauiKitTerminal EXPORT MauiKitTerminalTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
#    ${headers}
    ${CMAKE_CURRENT_BINARY_DIR}/terminal_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit/Terminal
    COMPONENT Devel)

if (BUILD_SHARED_LIBS)
    add_custom_target(copy_to_bin ALL
        COMMAND ${CMAKE_COMMAND} -E
                make_directory ${CMAKE_BINARY_DIR}/bin/org/mauikit/terminal/
        COMMAND ${CMAKE_COMMAND} -E
                copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/controls ${CMAKE_BINARY_DIR}/bin/org/mauikit/terminal/
        COMMAND ${CMAKE_COMMAND} -E
                copy $<TARGET_FILE:MauiKitTerminal> ${CMAKE_BINARY_DIR}/bin/org/mauikit/terminal/
    )

    install(DIRECTORY controls/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/terminal)

    #include(ECMGeneratePriFile)
    #ecm_generate_pri_file(BASE_NAME KQuickImageEditor LIB_NAME KQuickImageEditor DEPS "core qml quick svg" FILENAME_VAR PRI_FILENAME )
    #install(FILES ${PRI_FILENAME}
            #DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

endif()

install(TARGETS MauiKitTerminal DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/terminal )


