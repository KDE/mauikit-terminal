# SPDX-FileCopyrightText: 2020 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

set(QT_MAJOR_VERSION 6)
set(REQUIRED_QT_VERSION 6.4)

set(REQUIRED_KF_VERSION 5.240.0)
set(KF_MAJOR_VERSION 6)

set(MAUI_MAJOR_VERSION 4)
set(MAUIKIT_VERSION 4.0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(AUTOMOC_MOC_OPTIONS -Muri=org.mauikit.terminal)

project(Terminal LANGUAGES CXX VERSION ${MAUIKIT_VERSION})

find_package(ECM ${REQUIRED_KF_VERSION} NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(GNUInstallDirs)
include(FeatureSummary)
include(CMakePackageConfigHelpers)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(KDEClangFormat)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMFindQmlModule)
include(ECMQmlModule)
include(ECMSourceVersionControl)

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS
    Core
    Quick
    Widgets)

find_package(KF${KF_MAJOR_VERSION} ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS
    I18n
    CoreAddons
    Config)

find_package(MauiKit${MAUI_MAJOR_VERSION} ${MAUIKIT_VERSION} REQUIRED)

if(${ECM_SOURCE_UNDER_VERSION_CONTROL})
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Get the latest abbreviated commit hash of the working branch
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
    add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

else()
    add_definitions(-DGIT_COMMIT_HASH="${PROJECT_VERSION}")
    add_definitions(-DGIT_BRANCH="Stable")
endif()

qt_policy(SET QTP0004 NEW)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/MauiKitTerminal${MAUI_MAJOR_VERSION}")

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX Terminal
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/terminal_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/MauiKitTerminal${MAUI_MAJOR_VERSION}ConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR})

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/MauiKitTerminalConfig.cmake.in.${QT_MAJOR_VERSION}"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitTerminal${MAUI_MAJOR_VERSION}Config.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitTerminal${MAUI_MAJOR_VERSION}ConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitTerminal${MAUI_MAJOR_VERSION}Config.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel)

install(EXPORT MauiKitTerminal${MAUI_MAJOR_VERSION}Targets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE MauiKitTerminal${MAUI_MAJOR_VERSION}Targets.cmake
    NAMESPACE MauiKit${MAUI_MAJOR_VERSION}::)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/terminal_version.h"
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Terminal
    COMPONENT Devel)

add_definitions(-DTRANSLATION_DOMAIN="mauikitterminal")
ki18n_install(po)

add_subdirectory(src/lib)
add_subdirectory(src)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
