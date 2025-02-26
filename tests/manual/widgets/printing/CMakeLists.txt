# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(printing LANGUAGES CXX)
    find_package(Qt6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_manual_test(printing
    GUI
    SOURCES
        main.cpp
        resources.qrc
    LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::PrintSupport
        Qt::WebEngineWidgets
    ENABLE_AUTOGEN_TOOLS
        moc
        rcc
)
