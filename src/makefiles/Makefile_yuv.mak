#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libyuv
include Makefile_app_header.mak

OBJS = $(ODIR_PROJ)/compare.o \
	$(ODIR_PROJ)/convert.o \
	$(ODIR_PROJ)/convert_argb.o \
	$(ODIR_PROJ)/convert_from.o \
	$(ODIR_PROJ)/convert_from_argb.o \
	$(ODIR_PROJ)/cpu_id.o \
	$(ODIR_PROJ)/mjpeg_decoder.o \
	$(ODIR_PROJ)/planar_functions.o \
	$(ODIR_PROJ)/rotate.o \
	$(ODIR_PROJ)/rotate_argb.o \
	$(ODIR_PROJ)/scale.o \
	$(ODIR_PROJ)/scale_argb.o \
	$(ODIR_PROJ)/video_common.o \
	$(ODIR_PROJ)/row_common.o \
	$(ODIR_PROJ)/row_any.o \
	$(ODIR_PROJ)/row_neon64.o \
	$(ODIR_PROJ)/scale_common.o \
	$(ODIR_PROJ)/scale_any.o \
	$(ODIR_PROJ)/scale_neon64.o
	

default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)

$(ODIR_PROJ)/compare.o: $(SDIR_PROJ)/compare.cc $(SDIR_PROJ)/compare.h
	$(CXX) -o $(ODIR_PROJ)/compare.o $(CFLAGS) $(SDIR_PROJ)/compare.cc

$(ODIR_PROJ)/convert.o: $(SDIR_PROJ)/convert.cc $(SDIR_PROJ)/convert.h
	$(CXX) -o $(ODIR_PROJ)/convert.o $(CFLAGS) $(SDIR_PROJ)/convert.cc

$(ODIR_PROJ)/convert_argb.o: $(SDIR_PROJ)/convert_argb.cc $(SDIR_PROJ)/convert_argb.h
	$(CXX) -o $(ODIR_PROJ)/convert_argb.o $(CFLAGS) $(SDIR_PROJ)/convert_argb.cc

$(ODIR_PROJ)/convert_from.o: $(SDIR_PROJ)/convert_from.cc $(SDIR_PROJ)/convert_from.h
	$(CXX) -o $(ODIR_PROJ)/convert_from.o $(CFLAGS) $(SDIR_PROJ)/convert_from.cc

$(ODIR_PROJ)/convert_from_argb.o: $(SDIR_PROJ)/convert_from_argb.cc $(SDIR_PROJ)/convert_from_argb.h
	$(CXX) -o $(ODIR_PROJ)/convert_from_argb.o $(CFLAGS) $(SDIR_PROJ)/convert_from_argb.cc

$(ODIR_PROJ)/cpu_id.o: $(SDIR_PROJ)/cpu_id.cc $(SDIR_PROJ)/cpu_id.h
	$(CXX) -o $(ODIR_PROJ)/cpu_id.o $(CFLAGS) $(SDIR_PROJ)/cpu_id.cc

$(ODIR_PROJ)/mjpeg_decoder.o: $(SDIR_PROJ)/mjpeg_decoder.cc $(SDIR_PROJ)/mjpeg_decoder.h
	$(CXX) -o $(ODIR_PROJ)/mjpeg_decoder.o $(CFLAGS) $(SDIR_PROJ)/mjpeg_decoder.cc

$(ODIR_PROJ)/planar_functions.o: $(SDIR_PROJ)/planar_functions.cc $(SDIR_PROJ)/planar_functions.h
	$(CXX) -o $(ODIR_PROJ)/planar_functions.o $(CFLAGS) $(SDIR_PROJ)/planar_functions.cc
	
$(ODIR_PROJ)/rotate.o: $(SDIR_PROJ)/rotate.cc $(SDIR_PROJ)/rotate.h
	$(CXX) -o $(ODIR_PROJ)/rotate.o $(CFLAGS) $(SDIR_PROJ)/rotate.cc

$(ODIR_PROJ)/rotate_argb.o: $(SDIR_PROJ)/rotate_argb.cc $(SDIR_PROJ)/rotate_argb.h
	$(CXX) -o $(ODIR_PROJ)/rotate_argb.o $(CFLAGS) $(SDIR_PROJ)/rotate_argb.cc

$(ODIR_PROJ)/scale.o: $(SDIR_PROJ)/scale.cc $(SDIR_PROJ)/scale.h
	$(CXX) -o $(ODIR_PROJ)/scale.o $(CFLAGS) $(SDIR_PROJ)/scale.cc

$(ODIR_PROJ)/scale_argb.o: $(SDIR_PROJ)/scale_argb.cc $(SDIR_PROJ)/scale_argb.h
	$(CXX) -o $(ODIR_PROJ)/scale_argb.o $(CFLAGS) $(SDIR_PROJ)/scale_argb.cc

$(ODIR_PROJ)/video_common.o: $(SDIR_PROJ)/video_common.cc $(SDIR_PROJ)/video_common.h
	$(CXX) -o $(ODIR_PROJ)/video_common.o $(CFLAGS) $(SDIR_PROJ)/video_common.cc

$(ODIR_PROJ)/row_common.o: $(SDIR_PROJ)/row_common.cc $(SDIR_PROJ)/row.h
	$(CXX) -o $(ODIR_PROJ)/row_common.o $(CFLAGS) $(SDIR_PROJ)/row_common.cc
$(ODIR_PROJ)/row_any.o: $(SDIR_PROJ)/row_any.cc $(SDIR_PROJ)/row.h
	$(CXX) -o $(ODIR_PROJ)/row_any.o $(CFLAGS) $(SDIR_PROJ)/row_any.cc
$(ODIR_PROJ)/row_neon64.o: $(SDIR_PROJ)/row_neon64.cc $(SDIR_PROJ)/row.h
	$(CXX) -o $(ODIR_PROJ)/row_neon64.o $(CFLAGS) $(SDIR_PROJ)/row_neon64.cc

$(ODIR_PROJ)/scale_common.o: $(SDIR_PROJ)/scale_common.cc $(SDIR_PROJ)/scale.h
	$(CXX) -o $(ODIR_PROJ)/scale_common.o $(CFLAGS) $(SDIR_PROJ)/scale_common.cc
$(ODIR_PROJ)/scale_any.o: $(SDIR_PROJ)/scale_any.cc $(SDIR_PROJ)/scale.h
	$(CXX) -o $(ODIR_PROJ)/scale_any.o $(CFLAGS) $(SDIR_PROJ)/scale_any.cc
$(ODIR_PROJ)/scale_neon64.o: $(SDIR_PROJ)/scale_neon64.cc $(SDIR_PROJ)/scale.h
	$(CXX) -o $(ODIR_PROJ)/scale_neon64.o $(CFLAGS) $(SDIR_PROJ)/scale_neon64.cc

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)
