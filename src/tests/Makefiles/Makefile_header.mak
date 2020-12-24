-include Makefile.inc

#Note: you need to define the following env in ~/.bashrc
#export XEYES_SRC=/home/jack/xEyes/src
#export XEYES_BUILD=/home/jack/build_xeyes
#export XEYES_TEST =/home/jack/xEyes/src/tests

SDIR_PROJ=$(XEYES_SRC)/$(PROJ_NAME)
SDIR_LIB=$(XEYES_SRC)/libJack

ODIR_PROJ=$(XEYES_TEST)/lib/$(PROJ_NAME)
ODIR_LIB=$(XEYES_TEST)/lib/libs
ODIR_BIN=$(XEYES_TEST)/bin

#include and lib paths of the platform
PLTF_INC=/usr/local/include
PLTF_LIB=/usr/local/lib

#the same as PLTF_INC and PLTF_LIB, so we ignored
CV_INC=/usr/local/include/opencv4
CV_LIB=/usr/local/lib
#BOOST_INC=/usr/include
#BOOST_LIB=/usr/local/lib

QT_INC=/usr/include/aarch64-linux-gnu/qt5/QtCore
QT_LIB=/usr/include/aarch64-linux-gnu/qt5/QtWidgets
CUDA_INC=/usr/local/cuda/include
CUDA_LIB=/usr/local/cuda/lib64
JETSON_INFER_INC=/usr/local/include/jetson-inference
JETSON_UTIL_INC=/usr/local/include/jetson-utils
JETSON_LIB=/usr/local/lib


GST_INC=-I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_LIB=/usr/lib/aarch64-linux-gnu 

CC = /usr/bin/gcc
CXX = /usr/bin/g++

# --- debug ---
#DEBUG = -O0 -g

# ---release---- 
# -O2 will turn on all optimizations that don't require a space\speed trade off and tends to be the one 
# I see used most often. -O3 does some space for speed trade offs(like function inline.) -Os does O2 plus 
# does other things to reduce code size. This can make things faster than O3 by improving cache use. 
# (test to find out if it works for you.) Note there are a large number of options that none of the O switches touch. 
# The reason they are left out is because it often depends on what kind of code you are writing or are very 
# architecture dependent.
DEBUG = -Os -DNDEBUG

#include flags
CFLAGS = -Wall -static -c $(DEBUG) -DqDNGDebug=1 -D__xlC__=1 -DNO_FCGI_DEFINES=1 -DqDNGUseStdInt=0 -DUNIX_ENV=1 -D__LITTLE_ENDIAN__=1 -DqMacOS=0 -DqWinOS=0 -std=gnu++11 \
-I$(SDIR_PROJ) -I$(XEYES_SRC) -I$(CUDA_INC) -I$(QT_INC) $(GST_INC) -I$(PLTF_INC) -I$(JETSON_INFER_INC) -I$(JETSON_UTIL_INC) -I$(QT_LIB) -I$(CV_INC) -I$(CV_LIB)

TARGETFILE=$(PROJ_NAME).a
