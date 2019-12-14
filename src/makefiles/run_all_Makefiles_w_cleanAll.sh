#!/bin/bash

#clean all
make -f Makefile_util.mak clean
make -f Makefile_yuv.mak clean
make -f Makefile_cfg.mak clean
make -f Makefile_ds.mak clean
make -f Makefile_cap.mak clean
make -f Makefile_det.mak clean
make -f Makefile_gui.mak clean
make -f Makefile_dsp.mak clean
make -f Makefile_run.mak clean
make -f Makefile_xEyes.mak clean

#---------------------------------------
make -f Makefile_util.mak -j 2
make -f Makefile_yuv.mak -j 2
make -f Makefile_cfg.mak -j 2
make -f Makefile_ds.mak -j 2
make -f Makefile_cap.mak -j 2
make -f Makefile_det.mak -j 2
make -f Makefile_gui.mak -j 2
make -f Makefile_dsp.mak -j 2
make -f Makefile_run.mak -j 2
make -f Makefile_xEyes.mak -j 2
#----------------------------------------

