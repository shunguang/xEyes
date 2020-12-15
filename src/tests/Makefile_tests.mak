PROJ_NAME = tests
include Makefile_header.mak

#redefine CFLAGS and LIBS
CFLAGS	= -Wall -c $(DEBUG) -DqDNGDebug=1 -D__xlC__=1 -DNO_FCGI_DEFINES=1 -DqDNGUseStdInt=0 -DUNIX_ENV=1 -D__LITTLE_ENDIAN__=1 -DqMacOS=0 -DqWinOS=0 -std=gnu++11 \
	-I$(SDIR_PROJ) -I$(XEYES_SRC) -I$(QT_INC) -I$(CUDA_INC) $(GST_INC) -I$(JETSON_INFER_INC) -I$(JETSON_UTIL_INC) -I$(PLTF_INC) -I$(CV_INC) -I$(CV_LIB)
	
#link flags
#define link flags and lib searching paths
LFLAGS	= -Wall $(DEBUG) $(L_GST_LIB) -L$(QT_LIB) -L$(ODIR_LIB) -L$(JETSON_LIB)  -L$(CUDA_LIB) -L$(PLTF_LIB) -L$(GST_LIB)

OBJS = CircularQ.o \
	LLNodeQ.o \
	VehicleQ.o \
	cqDriver.out \
	LLNodeQ.out \
	VehicleQ.out

#default: directories $(TARGETFILE)

#directories:
	#mkdir -p $(ODIR_PROJ)
	#mkdir -p $(ODIR_LIB)

$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)

CircularQ.o: $(SDIR_PROJ)/cqDriver.cpp
	$(CXX) $(CFLAGS) -o CircularQ.o $(SDIR_PROJ)/cqDriver.cpp

YuvCircularQ.o: $(SDIR_PROJ)/YuvDriver.cpp
	$(CXX) $(CFLAGS) -o YuvCircularQ.o $(SDIR_PROJ)/YuvDriver.cpp

LLNodeQ.o: $(SDIR_PROJ)/LLDriver.cpp
	$(CXX) $(CFLAGS) -o LLNodeQ.o $(SDIR_PROJ)/LLDriver.cpp

VehicleQ.o: $(SDIR_PROJ)/VehicleDriver.cpp
	$(CXX) $(CFLAGS) -o VehicleQ.o $(SDIR_PROJ)/VehicleDriver.cpp

cqDriver.out : CircularQ.o
	$(CXX) $(LFLAGS) CircularQ.o -o cqDriver.out

YuvDriver.out : YuvCircularQ.o
	$(CXX) $(LFLAGS) YuvCircularQ.o -o YuvDriver.out

LLNodeQ.out: LLNodeQ.o
	$(CXX) $(LFLAGS) LLNodeQ.o -o LLNodeQ.out

VehicleQ.out: VehicleQ.o
	$(CXX) $(LFLAGS) VehicleQ.o -o VehicleQ.out

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)
