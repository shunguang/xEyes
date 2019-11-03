#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libDet
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/RunDetBase.o \
	$(ODIR_PROJ)/RunDetBkgChg.o
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)
	
$(ODIR_PROJ)/RunDetBase.o: $(SDIR_PROJ)/RunDetBase.cpp $(SDIR_PROJ)/RunDetBase.h
	$(CXX) -o $(ODIR_PROJ)/RunDetBase.o $(CFLAGS) $(SDIR_PROJ)/RunDetBase.cpp
    
$(ODIR_PROJ)/RunDetBkgChg.o: $(SDIR_PROJ)/RunDetBkgChg.cpp $(SDIR_PROJ)/RunDetBkgChg.h
	$(CXX) -o $(ODIR_PROJ)/RunDetBkgChg.o $(CFLAGS) $(SDIR_PROJ)/RunDetBkgChg.cpp

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

