-include Makefile.inc

PROJ_NAME=xEyes

include Makefile_app_header.mak

#the target binary name
TARGETFILE=$(ODIR_ROOT)/app.out

#redefine CFLAGS and LIBS
CFLAGS	:= -Wall -c $(DEBUG) -DqDNGDebug=1 -D__xlC__=1 -DNO_FCGI_DEFINES=1 -DqDNGUseStdInt=0 -DUNIX_ENV=1 -D__LITTLE_ENDIAN__=1 -DqMacOS=0 -DqWinOS=0 -std=gnu++11 \
	-I$(SDIR_PROJ) -I$(SDIR_ROOT) -I$(QT_INC) -I$(CUDA_INC) $(GST_INC) -I$(PLTF_INC) 
	
#redefine link flags and lib searching paths
LFLAGS	:= -Wall $(DEBUG) $(L_GST_LIB) -L$(QT_LIB) -L$(ODIR_LIB) -L$(CUDA_LIB) -L$(PLTF_LIB) -L$(GST_LIB)

#link libs
#L_GST_LIB=-lgstrtspserver-1.0
LIBS	:= -lqmake_run -lqmake_gui -lqmake_dsp -lDet -lCap -lDc -lCfg -lUtil -lyuv \
	-lboost_timer -lboost_filesystem -lboost_system -lboost_date_time -lboost_regex \
	-lboost_chrono -lboost_thread -pthread \
	-lnppc_static -lnppif_static -lnppig_static -lnppial_static -lnppicc_static -lnppicom_static -lnppisu_static -lnppidei -lculibos -lcublas_static -lcudart_static \
	-lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_dnn \
	-lopencv_ml -lopencv_shape -lopencv_video -lopencv_calib3d -lopencv_features2d \
	-lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc -lopencv_core \
	-lQt5Core -lQt5Gui -lQt5Widgets -lQt5OpenGL \
	-lgthread-2.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lgstapp-1.0 -lz -lv4l2 \
	-ldl -lm -lpthread -lrt 

OBJS = $(ODIR_PROJ)/main.o \
	$(ODIR_PROJ)/test_gst_rtsp_rcvH264_dec.o \
	$(ODIR_PROJ)/test_gst_rtsp_rcvH264_dec_and_save.o \
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

$(ODIR_PROJ)/test_gst_rtsp_rcvH264_dec.o	:	$(SDIR_PROJ)/test_gst_rtsp_rcvH264_dec.cpp
	$(CXX) -o $(ODIR_PROJ)/test_gst_rtsp_rcvH264_dec.o $(CFLAGS) $(SDIR_PROJ)/test_gst_rtsp_rcvH264_dec.cpp

$(ODIR_PROJ)/test_gst_rtsp_rcvH264_dec_and_save.o	:	$(SDIR_PROJ)/test_gst_rtsp_rcvH264_dec_and_save.cpp
	$(CXX) -o $(ODIR_PROJ)/test_gst_rtsp_rcvH264_dec_and_save.o $(CFLAGS) $(SDIR_PROJ)/test_gst_rtsp_rcvH264_dec_and_save.cpp

$(ODIR_PROJ)/test_gpu_availability.o	:	$(SDIR_PROJ)/test_gpu_availability.cpp
	$(CXX) -o $(ODIR_PROJ)/test_gpu_availability.o $(CFLAGS) $(SDIR_PROJ)/test_gpu_availability.cpp

$(ODIR_PROJ)/test_xEyes.o	:	$(SDIR_PROJ)/test_xEyes.cpp
	$(CXX) -o $(ODIR_PROJ)/test_xEyes.o $(CFLAGS) $(SDIR_PROJ)/test_xEyes.cpp

clean:
	\rm $(ODIR_PROJ)/*.o $(TARGETFILE)

rm_app:
	\rm $(TARGETFILE)
            
