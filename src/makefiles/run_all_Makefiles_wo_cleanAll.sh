#!/bin/bash
#---------------------------------------
make -f Makefile_util.mak -j 2
make -f Makefile_yuv.mak -j 2
make -f Makefile_cfg.mak -j 2
make -f Makefile_dc.mak -j 2
make -f Makefile_cap.mak -j 2
make -f Makefile_det.mak -j 2
make -f Makefile_gui.mak -j 2
make -f Makefile_dsp.mak -j 2
make -f Makefile_run.mak -j 2

make -f Makefile_xEyes.mak  rm_app
make -f Makefile_xEyes.mak
