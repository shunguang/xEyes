#---------------------------------------------------------
# This qmake x.pro files
# by shunguang.wu@gmail.com
# 12/8/2019
#---------------------------------------------------------

PROJ_NAME=xEyes

#---------------------------------------------------------
# BEG: don not change for my nano evn
#---------------------------------------------------------
SDIR_ROOT=/home/swu/projects/xEyes/src
SDIR_PROJ=/home/swu/projects/xEyes/src/$$PROJ_NAME

ODIR_ROOT?=/home/swu/projects/xEyes/build
ODIR_PROJ=/home/swu/projects/xEyes/build/$$PROJ_NAME
ODIR_LIB=/home/swu/projects/xEyes/build/libs

#include and lib paths
PLTF_INC=/usr/include/c++/7
PLTF_LIB=/usr/lib

#CV_INC=/usr/include
#CV_LIB=/usr/lib
#BOOST_INC=/usr/include
#BOOST_LIB=/usr/lib

QT_INC=/usr/local/Qt-5.9.2/include
QT_LIB=/usr/local/Qt-5.9.2/lib
CUDA_INC=/usr/local/cuda/include

TEMPLATE = app   # build an executable file
CONFIG += debug 
#CONFIG += release
CONFIG += c++11

#-------- note ----------
#prevent to generate symbolic link files in <DESTDIR>
#threre is an error when creating symbolic link files in a VM folder which are shared w/ host magchine 
#so we must prevent to generate multi-symbolic linked files (with different version tag info on it) with libqmake_runApps.so
CONFIG += plugin   

INCLUDEPATH += $$PLTF_INC
#INCLUDEPATH += $$BOOST_INC
#INCLUDEPATH += $$CV_INC 
INCLUDEPATH += $$QT_INC 
INCLUDEPATH += $$CUDA_INC 
INCLUDEPATH += $$SDIR_ROOT 
INCLUDEPATH += $$SDIR_PROJ 

OBJECTS_DIR = $$ODIR_PROJ 			#Intermediate object files directory
MOC_DIR = $$ODIR_PROJ 				#Intermediate moc files directory
DESTDIR = $$ODIR_LIB
#---------------------------------------------------------
# END: don not change  my nano evn
#---------------------------------------------------------
QMAKE_MAKEFILE=$$SDIR_ROOT/makefiles/Makefile_qmake_xEyes.mak
TARGET = $$ODIR_ROOT
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