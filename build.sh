#!/bin/bash
dir="./project/src/main.cpp \
     ./project/include/Mat.h \
     ./project/include/SMatrix.h \
     ./project/include/utils.h \
     ./project/src/utils.cpp \
     ./project/include/correlation.h"

lib="-lwiringPi \
     -ljpeg"
     #lwiringPi - библиотека для пинов
     #ljpeg - для чтения джепек картинок

svn=14  # Версия cpp

fn="main"  # Имя после компиляции

g++ $dir -std=c++$svn -o $fn $lib