#---------------------------------------------------------
# This qmake x.pro files
# Ref: http://doc.qt.io/qt-5/qmake-manual.html
# by wus1@jhuapl.edu
# 10/3/2017
#---------------------------------------------------------

PROJ_NAME=apps

#---------------------------------------------------------
# BEG: don not change for maven proj
#---------------------------------------------------------
SDIR_ROOT=/media/sf_Maven/trunk/vmti/src
SDIR_PROJ=/media/sf_Maven/trunk/vmti/src/$$PROJ_NAME
ODIR_ROOT=/media/sf_Maven/build/redhat7_1_1_3/vmti
ODIR_PROJ=/media/sf_Maven/build/redhat7_1_1_3/vmti/$$PROJ_NAME
ODIR_LIB=/media/sf_Maven/build/redhat7_1_1_3/vmti/libs
ODIR_BIN=/media/sf_Maven/build/redhat7_1_1_3/vmti/bin

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

TEMPLATE = app   # build an application
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
#DESTDIR = $$ODIR_LIB
#---------------------------------------------------------
# END: don not change for maven proj
#---------------------------------------------------------
#target.path += $$[QT_INSTALL_PLUGINS]/imageformats
#INSTALLS += target

#does not work
#QMAKE_MAKEFILE=$$SDIR_ROOT/makefiles/Makefile_$$PROJ_NAME.mak

#manully change
QMAKE_MAKEFILE=$$SDIR_ROOT/makefiles/Makefile_apps.mak

TARGET = $$ODIR_BIN/vmti

SOURCES = \
	$$SDIR_PROJ/main.cpp \
	$$SDIR_PROJ/mainRunVmtiGui.cpp \
	$$SDIR_PROJ/test_extract_text.cpp \
	$$SDIR_PROJ/testCapture.cpp \
	$$SDIR_PROJ/testStab.cpp \	
	$$SDIR_PROJ/testFuncs.cpp \	
	$$SDIR_PROJ/testGrabCut.cpp \	
	$$SDIR_PROJ/testTrk2d.cpp \	
	$$SDIR_PROJ/testMeanShiftDemo.cpp \	
	$$SDIR_PROJ/testVideoReader_n_Writer.cpp \	
	$$SDIR_PROJ/testKlvTrifectaParser.cpp \	
	$$SDIR_PROJ/testRunCapFromStream.cpp	

LIBS += -L$$ODIR_LIB
LIBS += -L$$PLTF_LIB
LIBS += -L$$BOOST_LIB
LIBS += -L$$CV_LIB
LIBS += -L$$QT_LIB
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_features2d -lopencv_xfeatures2d -lopencv_videoio -lopencv_video -lopencv_videostab
LIBS += -lQt5Core -lQt5Gui -lQt5Widgets
LIBS += -lboost_timer -lboost_filesystem -lboost_system -lboost_date_time -lboost_regex -lboost_chrono -lboost_thread -lrt
LIBS += -lqmake_runApps -lqmake_gui -ltrk -lJpda3d -lJpda2d -ldisp -lchgDetection -lstab -lcapture -lkfFusion -lafm -ldc -lkBest -lcfg -lklv -lsurfAna -lswMat -lutil

msg = $$QMAKE_MAKEFILE
msg += "will be created!"
message( $$msg )	