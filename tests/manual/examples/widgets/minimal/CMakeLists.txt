# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
     cmake_minimum_required(VERSION 3.16)
     project(minimal LANGUAGES CXX)
     find_package(Qt6BuildInternals COMPONENTS STANDALONE_TEST)
 endif()

qt_internal_add_manual_test(minimal-widgets
    SOURCES
        main.cpp
)

set_target_properties(minimal-widgets PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(minimal-widgets PUBLIC
    Qt::Core
    Qt::Gui
    Qt::WebEngineWidgets
)
