#---------------------------------------------------------
# This is a qmake pro file, 
# to create make file use the following cmd:
# /usr/local/Qt-5.9.2/bin/qmake -makefile qmake_run.pro 
#
# by shunguang.wu@gmail.com
# 12/8/2019
#---------------------------------------------------------

PROJ_NAME=libRun

#---------------------------------------------------------
# BEG: don not change for my nano evn
#---------------------------------------------------------
SDIR_ROOT=/home/swu/projects/xEyes/src
SDIR_PROJ=/home/swu/projects/xEyes/src/$$PROJ_NAME

ODIR_ROOT?=/home/swu/projects/build_xeyes
ODIR_PROJ=/home/swu/projects/build_xeyes/$$PROJ_NAME
ODIR_LIB=/home/swu/projects/build_xeyes/libs

#include and lib paths
PLTF_INC=/usr/include/c++/7
PLTF_LIB=/usr/lib

#CV_INC=/usr/include
#CV_LIB=/usr/lib
#BOOST_INC=/usr/include
#BOOST_LIB=/usr/lib
GST_INC1=/usr/include/gstreamer-1.0 
GST_INC2=/usr/include/glib-2.0
GST_INC3=/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_LIB=/usr/lib/aarch64-linux-gnu1

QT_INC=/usr/local/Qt-5.9.2/include
QT_LIB=/usr/local/Qt-5.9.2/lib

CUDA_INC=/usr/local/cuda/include

JETSON_INFER_INC=/usr/local/include/jetson-inference
JETSON_UTIL_INC=/usr/local/include/jetson-utils
JETSON_LIB=/usr/local/lib


TEMPLATE = lib   # build a static or shared lib
CONFIG += debug 
#CONFIG += release
CONFIG += staticlib  #comment this line will build shared lib
CONFIG += c++11

#-------- note ----------
#prevent to generate symbolic link files in <DESTDIR>
#threre is an error when creating symbolic link files in a VM folder which are shared w/ host magchine 
#so we must prevent to generate multi-symbolic linked files (with different version tag info on it) with libqmake_runApps.so
CONFIG += plugin   

INCLUDEPATH += $$PLTF_INC
INCLUDEPATH += $$GST_INC1
INCLUDEPATH += $$GST_INC2
INCLUDEPATH += $$GST_INC3
#INCLUDEPATH += $$BOOST_INC
#INCLUDEPATH += $$CV_INC 
INCLUDEPATH += $$QT_INC 
INCLUDEPATH += $$CUDA_INC 
INCLUDEPATH += $$JETSON_INFER_INC 
INCLUDEPATH += $$JETSON_UTIL_INC 

INCLUDEPATH += $$SDIR_ROOT 
INCLUDEPATH += $$SDIR_PROJ 

OBJECTS_DIR = $$ODIR_PROJ 			#Intermediate object files directory
MOC_DIR = $$ODIR_PROJ 				#Intermediate moc files directory
DESTDIR = $$ODIR_LIB
#---------------------------------------------------------
# END: don not change  my nano evn
#---------------------------------------------------------
#
#destination makeFile name
QMAKE_MAKEFILE=$$SDIR_ROOT/makefiles/Makefile_run.mak
HEADERS = \
	$$SDIR_PROJ/RunCap.h \
	$$SDIR_PROJ/RunDet.h \
	$$SDIR_PROJ/RunGui.h \
	$$SDIR_PROJ/RunXEyes.h

SOURCES = \
	$$SDIR_PROJ/RunCap.cpp \
	$$SDIR_PROJ/RunDet.cpp \
	$$SDIR_PROJ/RunGui.cpp \
	$$SDIR_PROJ/RunXEyes.cpp

#RESOURCES=$$SDIR_PROJ/appGui.qrc

msg = $$QMAKE_MAKEFILE
msg += "will be created!"
message( $$msg )	