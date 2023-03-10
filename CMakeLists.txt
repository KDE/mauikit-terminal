# SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
#
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.16)

set(KF5_VERSION "5.70.0")
set(REQUIRED_QT_VERSION 5.15.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(AUTOMOC_MOC_OPTIONS -Muri=org.mauikit.terminal)

project(Terminal LANGUAGES CXX VERSION 1.1.0)

find_package(ECM ${KF5_VERSION} NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

option(QUICK_COMPILER "Use QtQuick compiler to improve performance" TRUE)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
include(ECMSetupVersion)
include(ECMGenerateQmlTypes)
include(CMakePackageConfigHelpers)
include(KDEClangFormat)
include(KDEPackageAppTemplates)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/MauiKitTerminal")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/MauiKitTerminalConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitTerminalConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitTerminalConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitTerminalConfig.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

    install(EXPORT MauiKitTerminalTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE MauiKitTerminalTargets.cmake
        NAMESPACE MauiKit::)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX Terminal
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/terminal_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/MauiKitTerminalConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/terminal_version.h"
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit/Terminal
    COMPONENT Devel)


find_package(Qt5 ${REQUIRED_QT_VERSION} COMPONENTS Core Quick Widgets REQUIRED)
find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED COMPONENTS I18n CoreAddons Config)
find_package(MauiKit REQUIRED)

if(QUICK_COMPILER)
        find_package(Qt5QuickCompiler)
        set_package_properties(Qt5QuickCompiler PROPERTIES
                DESCRIPTION "Compile QML at build time"
                TYPE OPTIONAL
        )
endif()

    add_definitions(-DTRANSLATION_DOMAIN="mauikitterminal")
    ki18n_install(po)

add_subdirectory(src)

    file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
