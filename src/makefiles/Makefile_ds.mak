#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libDs
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/Yuv420Frm_h.o \
	$(ODIR_PROJ)/Yuv420Frm_d.o \
	$(ODIR_PROJ)/JpegFrm_h.o \
	$(ODIR_PROJ)/RgbFrm_d.o \
	$(ODIR_PROJ)/RgbFrm_h.o \
	$(ODIR_PROJ)/YuvCircularQ_h.o \
	$(ODIR_PROJ)/RgbCircularQ_h.o \
	$(ODIR_PROJ)/Dc.o \
	$(ODIR_PROJ)/DcUI.o \
	$(ODIR_PROJ)/ThreadX.o 
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)
	
$(ODIR_PROJ)/Yuv420Frm_h.o: $(SDIR_PROJ)/Yuv420Frm_h.cpp $(SDIR_PROJ)/Yuv420Frm_h.h
	$(CXX) -o $(ODIR_PROJ)/Yuv420Frm_h.o $(CFLAGS) $(SDIR_PROJ)/Yuv420Frm_h.cpp
    
$(ODIR_PROJ)/Yuv420Frm_d.o: $(SDIR_PROJ)/Yuv420Frm_d.cpp $(SDIR_PROJ)/Yuv420Frm_d.h
	$(CXX) -o $(ODIR_PROJ)/Yuv420Frm_d.o $(CFLAGS) $(SDIR_PROJ)/Yuv420Frm_d.cpp

$(ODIR_PROJ)/JpegFrm_h.o: $(SDIR_PROJ)/JpegFrm_h.cpp $(SDIR_PROJ)/JpegFrm_h.h
	$(CXX) -o $(ODIR_PROJ)/JpegFrm_h.o $(CFLAGS) $(SDIR_PROJ)/JpegFrm_h.cpp

$(ODIR_PROJ)/RgbFrm_d.o: $(SDIR_PROJ)/RgbFrm_d.cpp $(SDIR_PROJ)/RgbFrm_d.h
	$(CXX) -o $(ODIR_PROJ)/RgbFrm_d.o $(CFLAGS) $(SDIR_PROJ)/RgbFrm_d.cpp
$(ODIR_PROJ)/RgbFrm_h.o: $(SDIR_PROJ)/RgbFrm_h.cpp $(SDIR_PROJ)/RgbFrm_h.h
	$(CXX) -o $(ODIR_PROJ)/RgbFrm_h.o $(CFLAGS) $(SDIR_PROJ)/RgbFrm_h.cpp

$(ODIR_PROJ)/YuvCircularQ_h.o: $(SDIR_PROJ)/YuvCircularQ_h.cpp $(SDIR_PROJ)/YuvCircularQ_h.h
	$(CXX) -o $(ODIR_PROJ)/YuvCircularQ_h.o $(CFLAGS) $(SDIR_PROJ)/YuvCircularQ_h.cpp
$(ODIR_PROJ)/RgbCircularQ_h.o: $(SDIR_PROJ)/RgbCircularQ_h.cpp $(SDIR_PROJ)/RgbCircularQ_h.h
	$(CXX) -o $(ODIR_PROJ)/RgbCircularQ_h.o $(CFLAGS) $(SDIR_PROJ)/RgbCircularQ_h.cpp

$(ODIR_PROJ)/Dc.o: $(SDIR_PROJ)/Dc.cpp $(SDIR_PROJ)/Dc.h
	$(CXX) -o $(ODIR_PROJ)/Dc.o $(CFLAGS) $(SDIR_PROJ)/Dc.cpp

$(ODIR_PROJ)/DcUI.o: $(SDIR_PROJ)/DcUI.cpp $(SDIR_PROJ)/DcUI.h
	$(CXX) -o $(ODIR_PROJ)/DcUI.o $(CFLAGS) $(SDIR_PROJ)/DcUI.cpp

#$(ODIR_PROJ)/CudaUtil.o: $(SDIR_PROJ)/CudaUtil.cpp $(SDIR_PROJ)/CudaUtil.h
#	$(CXX) -o $(ODIR_PROJ)/CudaUtil.o $(CFLAGS) $(SDIR_PROJ)/CudaUtil.cpp

$(ODIR_PROJ)/ThreadX.o: $(SDIR_PROJ)/ThreadX.cpp $(SDIR_PROJ)/ThreadX.h
	$(CXX) -o $(ODIR_PROJ)/ThreadX.o $(CFLAGS) $(SDIR_PROJ)/ThreadX.cpp

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

