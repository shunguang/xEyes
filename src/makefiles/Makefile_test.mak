#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libTest
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/TestCaps.o 
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)
	
$(ODIR_PROJ)/TestCaps.o: $(SDIR_PROJ)/TestCaps.cpp $(SDIR_PROJ)/TestCaps.h
	$(CXX) -o $(ODIR_PROJ)/TestCaps.o $(CFLAGS) $(SDIR_PROJ)/TestCaps.cpp
    
#$(ODIR_PROJ)/RunCapCamA.o: $(SDIR_PROJ)/RunCapCamA.cpp $(SDIR_PROJ)/RunCapCamA.h
#	$(CXX) -o $(ODIR_PROJ)/RunCapCamA.o $(CFLAGS) $(SDIR_PROJ)/RunCapCamA.cpp

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

