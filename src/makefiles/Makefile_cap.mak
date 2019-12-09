#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libCap
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/CapThreadBase.o \
	$(ODIR_PROJ)/CapThreadCamTemp.o \
	$(ODIR_PROJ)/CapThreadSyn.o
	
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

$(ODIR_PROJ)/CapThreadCamTemp.o: $(SDIR_PROJ)/CapThreadCamTemp.cpp $(SDIR_PROJ)/CapThreadCamTemp.h
	$(CXX) -o $(ODIR_PROJ)/CapThreadCamTemp.o $(CFLAGS) $(SDIR_PROJ)/CapThreadCamTemp.cpp

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

