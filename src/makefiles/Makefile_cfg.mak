#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libCfg
include Makefile_app_header.mak

OBJS = 	$(ODIR_PROJ)/Cfg.o \
    $(ODIR_PROJ)/CfgBase.o \
	$(ODIR_PROJ)/CfgLog.o \
	$(ODIR_PROJ)/CfgDisp.o \
	$(ODIR_PROJ)/CfgCam.o
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_PROJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)

$(ODIR_PROJ)/CfgBase.o: $(SDIR_PROJ)/CfgBase.cpp $(SDIR_PROJ)/CfgBase.h
	$(CXX) -o $(ODIR_PROJ)/CfgBase.o $(CFLAGS) $(SDIR_PROJ)/CfgBase.cpp

$(ODIR_PROJ)/CfgDisp.o: $(SDIR_PROJ)/CfgDisp.cpp $(SDIR_PROJ)/CfgDisp.h
	$(CXX) -o $(ODIR_PROJ)/CfgDisp.o $(CFLAGS) $(SDIR_PROJ)/CfgDisp.cpp

$(ODIR_PROJ)/CfgCam.o: $(SDIR_PROJ)/CfgCam.cpp $(SDIR_PROJ)/CfgCam.h
	$(CXX) -o $(ODIR_PROJ)/CfgCam.o $(CFLAGS) $(SDIR_PROJ)/CfgCam.cpp

$(ODIR_PROJ)/CfgLog.o: $(SDIR_PROJ)/CfgLog.cpp $(SDIR_PROJ)/CfgLog.h
	$(CXX) -o $(ODIR_PROJ)/CfgLog.o $(CFLAGS) $(SDIR_PROJ)/CfgLog.cpp

$(ODIR_PROJ)/Cfg.o: $(SDIR_PROJ)/Cfg.cpp $(SDIR_PROJ)/Cfg.h
	$(CXX) -o $(ODIR_PROJ)/Cfg.o $(CFLAGS) $(SDIR_PROJ)/Cfg.cpp
clean:
	\rm -r $(ODIR_PROJ)/*.o $(TARGETFILE)

