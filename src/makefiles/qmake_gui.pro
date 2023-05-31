#---------------------------------------------------------
# This is a qmake pro file
# /usr/local/Qt-5.9.2/bin/qmake -makefile qmake_gui.pro 
# by shunguang.wu@gmail.com
# 12/8/2019
#---------------------------------------------------------

# Note: you need to define the following env in ~/.bashrc
# export XEYES_SRC=/home/swu/projects/xEyes/src
# export XEYES_BUILD=/home/swu/projects/build_xeyes
# since qmake cannot access env (bad), we need to redefine them at here

XEYES_SRC=/home/swu/projects/xEyes/src
XEYES_BUILD=/home/swu/projects/build_xeyes

PROJ_NAME=libGui

#---------------------------------------------------------
# BEG: don not change for my nano evn
#---------------------------------------------------------
SDIR_PROJ=$$XEYES_SRC/$$PROJ_NAME

ODIR_PROJ=$$XEYES_BUILD/lib/$$PROJ_NAME
ODIR_LIB=$$XEYES_BUILD/lib/libs

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

TEMPLATE = lib        #build a static or shared lib
CONFIG += debug 
#CONFIG += release
CONFIG += staticlib   #comment this line will build shared lib
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
INCLUDEPATH += $$XEYES_SRC 
INCLUDEPATH += $$SDIR_PROJ 

OBJECTS_DIR = $$ODIR_PROJ 			#Intermediate object files directory
MOC_DIR = $$ODIR_PROJ 				#Intermediate moc files directory
RCC_DIR = $$ODIR_PROJ 				#Intermediate resource files directory
DESTDIR = $$ODIR_LIB
#---------------------------------------------------------
# END: don not change  my nano evn
#---------------------------------------------------------

#destination makeFile name
QMAKE_MAKEFILE=$$XEYES_SRC/makefiles/Makefile_gui.mak

HEADERS = \
	$$SDIR_PROJ/ImgLabel.h \
	$$SDIR_PROJ/MsgBox.h \
	$$SDIR_PROJ/ProgDialog.h \
	$$SDIR_PROJ/AppGui.h

SOURCES = \
	$$SDIR_PROJ/ImgLabel.cpp \
	$$SDIR_PROJ/MsgBox.cpp \
	$$SDIR_PROJ/AppGui.cpp

RESOURCES = \
	$$SDIR_PROJ/appGuiRc.qrc

msg = $$QMAKE_MAKEFILE
msg += "will be created!"
message( $$msg )	