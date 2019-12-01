#---------------------------------------------------------
# This qmake x.pro files
# by wus1@jhuapl.edu
# 10/3/2017
#---------------------------------------------------------

PROJ_NAME=runApps

#---------------------------------------------------------
# BEG: don not change for maven proj
#---------------------------------------------------------
SDIR_ROOT=/media/sf_Maven/trunk/vmti/src
SDIR_PROJ=/media/sf_Maven/trunk/vmti/src/$$PROJ_NAME
ODIR_ROOT=/media/sf_Maven/build/redhat7_1_1_3/vmti
ODIR_PROJ=/media/sf_Maven/build/redhat7_1_1_3/vmti/$$PROJ_NAME
ODIR_LIB=/media/sf_Maven/build/redhat7_1_1_3/vmti/libs

#include and lib paths
PLTF_INC=/usr/include/c++/7
PLTF_LIB=/usr/lib64
PKG_ROOT=/home/wus1/pkg
BOOST_INC=$$PKG_ROOT/boost_1_65_1_install/include
BOOST_LIB=$$PKG_ROOT/boost_1_65_1_install/lib
CV_INC=$$PKG_ROOT/opencv_3_3_install/include
CV_LIB=$$PKG_ROOT/opencv_3_3_install/lib
QT_INC=$$PKG_ROOT/qt_5_9/include
QT_LIB=$$PKG_ROOT/qt_5_9/lib

TEMPLATE = lib   # build a static lib
CONFIG += debug 
#CONFIG += release
#CONFIG += dll
CONFIG += c++11

#-------- note ----------
#prevent to generate symbolic link files in <DESTDIR>
#threre is an error when creating symbolic link files in a VM folder which are shared w/ host magchine 
#so we must prevent to generate multi-symbolic linked files (with different version tag info on it) with libqmake_runApps.so
CONFIG += plugin   

INCLUDEPATH += $$PLTF_INC
INCLUDEPATH += $$BOOST_INC
INCLUDEPATH += $$CV_INC 
INCLUDEPATH += $$QT_INC 
INCLUDEPATH += $$QT_INC 
INCLUDEPATH += $$SDIR_ROOT 
INCLUDEPATH += $$SDIR_PROJ 

OBJECTS_DIR = $$ODIR_PROJ 			#Intermediate object files directory
MOC_DIR = $$ODIR_PROJ 				#Intermediate moc files directory
DESTDIR = $$ODIR_LIB

#---------------------------------------------------------
# END: don not change for maven proj
#---------------------------------------------------------
 
QMAKE_MAKEFILE = $$SDIR_ROOT/makefiles/Makefile_runApps.mak

HEADERS = \
	$$SDIR_PROJ/RunThread.h \
	$$SDIR_PROJ/RunCapFromStream.h \
	$$SDIR_PROJ/RunPyr.h \
	$$SDIR_PROJ/RunStab.h \
	$$SDIR_PROJ/RunCd.h \
	$$SDIR_PROJ/RunGrabCut.h \
	$$SDIR_PROJ/RunSttBasic2d.h \
	$$SDIR_PROJ/RunMttJpda2d.h \
	$$SDIR_PROJ/RunMttJpda3d.h 

SOURCES = \
	$$SDIR_PROJ/RunThread.cpp \
	$$SDIR_PROJ/RunCapFromStream.cpp \
	$$SDIR_PROJ/RunPyr.cpp \
	$$SDIR_PROJ/RunStab.cpp \
	$$SDIR_PROJ/RunCd.cpp \
	$$SDIR_PROJ/RunGrabCut.cpp \
	$$SDIR_PROJ/RunSttBasic2d.cpp \
	$$SDIR_PROJ/RunMttJpda2d.cpp \
	$$SDIR_PROJ/RunMttJpda3d.cpp
	
msg = $$QMAKE_MAKEFILE
msg += "will be created!"
message( $$msg )	