# SPDX-License-Identifier: GPL-3.0-only
# MuseScore-CLA-applies
#
# MuseScore
# Music Composition & Notation
#
# Copyright (C) 2023 MuseScore BVBA and others
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

declare_module(muse_ai)
set(MODULE_ALIAS muse::ai)

set(MODULE_SRC
    ${CMAKE_CURRENT_LIST_DIR}/aimodule.cpp
    ${CMAKE_CURRENT_LIST_DIR}/aimodule.h
    ${CMAKE_CURRENT_LIST_DIR}/IAiMusicGenerator.h
    ${CMAKE_CURRENT_LIST_DIR}/AiMusicDataConverter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AiMusicDataConverter.h
    ${CMAKE_CURRENT_LIST_DIR}/AiController.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AiController.h
    ${CMAKE_CURRENT_LIST_DIR}/AiMusicTypes.h
)

set(MODULE_ROOT ${CMAKE_CURRENT_LIST_DIR})
# If you have QML files, uncomment the following line and specify the directory
# set(MODULE_QML_IMPORT ${CMAKE_CURRENT_LIST_DIR}/qml)

setup_module()

# If you have tests, uncomment the following lines
# if (MUSE_MODULE_AI_TESTS)
#     add_subdirectory(tests)
# endif()
