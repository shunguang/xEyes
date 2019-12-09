#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libDet
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/DetThreadBase.o \
	$(ODIR_PROJ)/DetThreadBkgChg.o
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)
	
$(ODIR_PROJ)/DetThreadBase.o: $(SDIR_PROJ)/DetThreadBase.cpp $(SDIR_PROJ)/DetThreadBase.h
	$(CXX) -o $(ODIR_PROJ)/DetThreadBase.o $(CFLAGS) $(SDIR_PROJ)/DetThreadBase.cpp
    
$(ODIR_PROJ)/DetThreadBkgChg.o: $(SDIR_PROJ)/DetThreadBkgChg.cpp $(SDIR_PROJ)/DetThreadBkgChg.h
	$(CXX) -o $(ODIR_PROJ)/DetThreadBkgChg.o $(CFLAGS) $(SDIR_PROJ)/DetThreadBkgChg.cpp

clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

