#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libCap
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/RunCapBase.o \
	$(ODIR_PROJ)/RunCapCamA.o
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)
	
$(ODIR_PROJ)/RunCapBase.o: $(SDIR_PROJ)/RunCapBase.cpp $(SDIR_PROJ)/RunCapBase.h
	$(CXX) -o $(ODIR_PROJ)/RunCapBase.o $(CFLAGS) $(SDIR_PROJ)/RunCapBase.cpp
    
$(ODIR_PROJ)/RunCapCamA.o: $(SDIR_PROJ)/RunCapCamA.cpp $(SDIR_PROJ)/RunCapCamA.h
	$(CXX) -o $(ODIR_PROJ)/RunCapCamA.o $(CFLAGS) $(SDIR_PROJ)/RunCapCamA.cpp

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

