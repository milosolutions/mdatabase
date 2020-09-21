#!/bin/bash

get_qtbase_sources_darwin() {
    curl -OL https://download.qt.io/archive/qt/${QT_VER_MINOR}/${QT_VER}/submodules/qtbase-everywhere-src-${QT_VER}.tar.xz
    tar -xf qtbase-everywhere-src-${QT_VER}.tar.xz
}

get_qtbase_sources_other() {
    wget https://download.qt.io/archive/qt/${QT_VER_MINOR}/${QT_VER}/submodules/qtbase-everywhere-src-${QT_VER}.tar.xz
    tar -xaf qtbase-everywhere-src-${QT_VER}.tar.xz
}

get_qtbase_sources() {
    local system=$(uname -s)

    if [ $system = "Darwin" ]; then
        get_qtbase_sources_darwin
    else
        get_qtbase_sources_other
    fi
}