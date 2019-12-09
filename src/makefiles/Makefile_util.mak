#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libUtil
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/CvUtilFuncs.o \
	$(ODIR_PROJ)/AppLog.o \
	$(ODIR_PROJ)/FileUtil.o \
	$(ODIR_PROJ)/Roi.o \
	$(ODIR_PROJ)/CvQtUtil.o \
	$(ODIR_PROJ)/UtilFuncs.o
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)
	
$(ODIR_PROJ)/CvUtilFuncs.o: $(SDIR_PROJ)/CvUtilFuncs.cpp $(SDIR_PROJ)/CvUtilFuncs.h
	$(CXX) -o $(ODIR_PROJ)/CvUtilFuncs.o $(CFLAGS) $(SDIR_PROJ)/CvUtilFuncs.cpp

$(ODIR_PROJ)/FileUtil.o: $(SDIR_PROJ)/FileUtil.cpp $(SDIR_PROJ)/FileUtil.h
	$(CXX) -o $(ODIR_PROJ)/FileUtil.o $(CFLAGS) $(SDIR_PROJ)/FileUtil.cpp

$(ODIR_PROJ)/AppLog.o: $(SDIR_PROJ)/AppLog.cpp $(SDIR_PROJ)/AppLog.h
	$(CXX) -o $(ODIR_PROJ)/AppLog.o $(CFLAGS) $(SDIR_PROJ)/AppLog.cpp

$(ODIR_PROJ)/Roi.o: $(SDIR_PROJ)/Roi.cpp $(SDIR_PROJ)/Roi.h
	$(CXX) -o $(ODIR_PROJ)/Roi.o $(CFLAGS) $(SDIR_PROJ)/Roi.cpp

$(ODIR_PROJ)/CvQtUtil.o: $(SDIR_PROJ)/CvQtUtil.cpp $(SDIR_PROJ)/CvQtUtil.h
	$(CXX) -o $(ODIR_PROJ)/CvQtUtil.o $(CFLAGS) $(SDIR_PROJ)/CvQtUtil.cpp

$(ODIR_PROJ)/UtilFuncs.o: $(SDIR_PROJ)/UtilFuncs.cpp $(SDIR_PROJ)/UtilFuncs.h
	$(CXX) -o $(ODIR_PROJ)/UtilFuncs.o $(CFLAGS) $(SDIR_PROJ)/UtilFuncs.cpp

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

