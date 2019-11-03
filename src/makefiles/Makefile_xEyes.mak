-include Makefile.inc
#define the macros 
PROJ_NAME=xEyes

#----------------------------- start keep consistance w/ Makefile_app_header.mak ---------------
#build intermediat output paths
SDIR_ROOT?=/home/swu/Projects/xEyes/src
SDIR_PROJ=$(SDIR_ROOT)/$(PROJ_NAME)

ODIR_ROOT?=/home/swu/Projects/xEyes/build
ODIR_PROJ=$(ODIR_ROOT)/$(PROJ_NAME)
ODIR_LIB=$(ODIR_ROOT)/libs

#include and lib paths of the platform
PLTF_INC=/usr/include
PLTF_LIB=/usr/lib

CV_INC=/usr/include
CV_LIB=/usr/lib
BOOST_INC=/usr/include
#BOOST_LIB=/usr/lib
#QT_INC=$(PKG_ROOT)/qt_5_9/include
#QT_LIB=$(PKG_ROOT)/qt_5_9/lib
CUDA_INC=/usr/local/cuda/include
CUDA_LIB=/usr/local/cuda/lib64


#find by cmd: pkg-config gstreamer-1.0 --cflags" and "pkg-config gstreamer-1.0 --libs
#find by cmd: pkg-config --cflags --libs gstreamer-rtsp-server-1.0
GST_INC=-I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_LIB=-lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lgstapp-1.0

CC = /usr/bin/gcc
CXX = /usr/bin/g++
DEBUG = -g
#DEBUG = -DNDEBUG -g
#DEBUG = -DDEBUG -g

#----------------------------- end keep consistance w/ Makefile_app_header.mak ---------------

#the target binary name
TARGETFILE=$(ODIR_ROOT)/app

#include flags
CFLAGS	:= -Wall -c $(DEBUG) -DqDNGDebug=1 -D__xlC__=1 -DNO_FCGI_DEFINES=1 -DqDNGUseStdInt=0 -DUNIX_ENV=1 -D__LITTLE_ENDIAN__=1 -DqMacOS=0 -DqWinOS=0 -std=gnu++11 \
	-I$(SDIR_PROJ) -I$(SDIR_ROOT) -I$(CV_INC) -I$(CUDA_INC) $(GST_INC) -I$(PLTF_INC) -I/opt/euresys/coaxlink/include
	
#link flags and lib searching paths
LFLAGS	:= -Wall $(DEBUG) -L$(CV_LIB) -L$(ODIR_LIB) -L$(CUDA_LIB) -L$(PLTF_LIB)

#link libs
LIBS	:= -lTest -lGui -lDet -lCap -lDs -lCfg -lUtil -lyuv \
	-lboost_timer -lboost_filesystem -lboost_system -lboost_date_time -lboost_regex \
	-lboost_chrono -lboost_thread -pthread \
	-lnppc_static -lnppif_static -lnppig_static -lnppial_static -lnppicc_static -lnppicom_static -lnppisu_static -lnppidei -lculibos -lcublas_static -lcudart_static \
	-lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_dnn \
	-lopencv_ml -lopencv_shape -lopencv_video -lopencv_calib3d -lopencv_features2d \
	-lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc \
	-lopencv_core \
	-lgthread-2.0 $(GST_LIB) -lglib-2.0 -L/usr/lib/aarch64-linux-gnu -lz -lv4l2 \
	-ldl -lm -lpthread -lrt 


OBJS = $(ODIR_PROJ)/main.o \
	$(ODIR_PROJ)/test_time_now.o \
	$(ODIR_PROJ)/test_gpu_availability.o \
	$(ODIR_PROJ)/test_xEyes.o

default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)
	
#the output binary file name is <$(TARGETFILE)>
$(TARGETFILE)	:	$(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) $(LIBS) $(LIBS) -o $(TARGETFILE)

$(ODIR_PROJ)/main.o	:	$(SDIR_PROJ)/main.cpp
	$(CXX) -o $(ODIR_PROJ)/main.o $(CFLAGS) $(SDIR_PROJ)/main.cpp

$(ODIR_PROJ)/test_time_now.o	:	$(SDIR_PROJ)/test_time_now.cpp
	$(CXX) -o $(ODIR_PROJ)/test_time_now.o $(CFLAGS) $(SDIR_PROJ)/test_time_now.cpp

$(ODIR_PROJ)/test_gpu_availability.o	:	$(SDIR_PROJ)/test_gpu_availability.cpp
	$(CXX) -o $(ODIR_PROJ)/test_gpu_availability.o $(CFLAGS) $(SDIR_PROJ)/test_gpu_availability.cpp

$(ODIR_PROJ)/test_xEyes.o	:	$(SDIR_PROJ)/test_xEyes.cpp
	$(CXX) -o $(ODIR_PROJ)/test_xEyes.o $(CFLAGS) $(SDIR_PROJ)/test_xEyes.cpp

clean:
	\rm $(ODIR_PROJ)/*.o $(TARGETFILE)

rm_app:
	\rm $(TARGETFILE)
            
