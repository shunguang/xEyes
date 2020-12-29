#Note: you need to define the following env in ~/.bashrc
#export XEYES_SRC=/home/jack/xEyes/src
#export XEYES_BUILD=/home/jack/build_xeyes
#export XEYES_TEST =/home/jack/xEyes/src/tests

-include Makefile.inc

PROJ_NAME=tests

include Makefile_header.mak

#the target binary name
TARGETFILE=test.out

#redefine CFLAGS and LIBS
CFLAGS	= -Wall -c $(DEBUG) -DqDNGDebug=1 -D__xlC__=1 -DNO_FCGI_DEFINES=1 -DqDNGUseStdInt=0 -DUNIX_ENV=1 -D__LITTLE_ENDIAN__=1 -DqMacOS=0 -DqWinOS=0 -std=gnu++11 \
	-I$(SDIR_PROJ) -I$(XEYES_SRC) -I$(QT_INC) -I$(CUDA_INC) $(GST_INC) -I$(JETSON_INFER_INC) -I$(JETSON_UTIL_INC) -I$(PLTF_INC) 
	
#link flags
#define link flags and lib searching paths
LFLAGS	= -Wall $(DEBUG) $(L_GST_LIB) -L$(QT_LIB) -L$(ODIR_LIB) -L$(JETSON_LIB)  -L$(CUDA_LIB) -L$(PLTF_LIB) -L$(GST_LIB)

#link libs
#L_GST_LIB=-lgstrtspserver-1.0
LIBS	:= -lboost_timer -lboost_filesystem -lboost_system -lboost_date_time -lboost_regex \
	-lboost_chrono -lboost_thread -pthread \
	-lnppc_static -lnppif_static -lnppig_static -lnppial_static -lnppicc_static -lnppicom_static -lnppisu_static -lnppidei -lculibos -lcublas_static -lcudart_static \
	-lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_dnn \
	-lopencv_ml -lopencv_shape -lopencv_video -lopencv_calib3d -lopencv_features2d \
	-lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc -lopencv_core \
	-lQt5Core -lQt5Gui -lQt5Widgets -lQt5OpenGL \
	-ljetson-inference -ljetson-utils \
	-lgthread-2.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lgstapp-1.0 -lz -lv4l2 \
	-ldl -lm -lpthread -lrt

OBJS = YuvCircularQ.o 

#default:  directories $(TARGETFILE)

#directories:    
	#mkdir -p $(ODIR_PROJ)
	#mkdir -p $(ODIR_LIB)
	#mkdir -p $(ODIR_BIN)
	
#the output binary file name is <$(TARGETFILE)>
$(TARGETFILE)	:	$(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) $(LIBS) -o $(TARGETFILE)

cleanAll:
	\rm $(ODIR_PROJ)/*.o $(TARGETFILE)

rm_app:
	\rm $(TARGETFILE)
            
