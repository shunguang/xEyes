#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libCap
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/CapThreadBase.o \
	$(ODIR_PROJ)/CapThreadSyn.o \
	$(ODIR_PROJ)/CapSaveRtspH264.o \
	$(ODIR_PROJ)/CapAndSaveThread.o
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)
	
$(ODIR_PROJ)/CapThreadBase.o: $(SDIR_PROJ)/CapThreadBase.cpp $(SDIR_PROJ)/CapThreadBase.h
	$(CXX) -o $(ODIR_PROJ)/CapThreadBase.o $(CFLAGS) $(SDIR_PROJ)/CapThreadBase.cpp
    
$(ODIR_PROJ)/CapThreadSyn.o: $(SDIR_PROJ)/CapThreadSyn.cpp $(SDIR_PROJ)/CapThreadSyn.h
	$(CXX) -o $(ODIR_PROJ)/CapThreadSyn.o $(CFLAGS) $(SDIR_PROJ)/CapThreadSyn.cpp

$(ODIR_PROJ)/CapSaveRtspH264.o: $(SDIR_PROJ)/CapSaveRtspH264.cpp $(SDIR_PROJ)/CapSaveRtspH264.h
	$(CXX) -o $(ODIR_PROJ)/CapSaveRtspH264.o $(CFLAGS) $(SDIR_PROJ)/CapSaveRtspH264.cpp

$(ODIR_PROJ)/CapAndSaveThread.o: $(SDIR_PROJ)/CapAndSaveThread.cpp $(SDIR_PROJ)/CapAndSaveThread.h
	$(CXX) -o $(ODIR_PROJ)/CapAndSaveThread.o $(CFLAGS) $(SDIR_PROJ)/CapAndSaveThread.cpp

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

