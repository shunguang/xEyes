-include Makefile.inc

#build intermediat output paths
SDIR_ROOT?=/home/swu/projects/xEyes/src
SDIR_PROJ=$(SDIR_ROOT)/$(PROJ_NAME)

ODIR_ROOT?=/home/swu/projects/build_xeyes
ODIR_PROJ=$(ODIR_ROOT)/$(PROJ_NAME)
ODIR_LIB=$(ODIR_ROOT)/libs

#include and lib paths of the platform
PLTF_INC=/usr/include
PLTF_LIB=/usr/lib

#the same as PLTF_INC and PLTF_LIB, so we ignored
#CV_INC=/usr/include
#CV_LIB=/usr/lib
#BOOST_INC=/usr/include
#BOOST_LIB=/usr/local/lib

QT_INC=/usr/local/Qt-5.9.2/include
QT_LIB=/usr/local/Qt-5.9.2/lib
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
	-I$(SDIR_PROJ) -I$(SDIR_ROOT) -I$(CUDA_INC) -I$(QT_INC) $(GST_INC) -I$(PLTF_INC) -I$(JETSON_INFER_INC) -I$(JETSON_UTIL_INC)

TARGETFILE=$(ODIR_LIB)/$(PROJ_NAME).a

