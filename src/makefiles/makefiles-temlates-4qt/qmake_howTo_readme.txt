how to write x.pro for qmake

1. edit x.pro file in host machine folder: 
   C:\Projects\Maven\trunk\vmti\src\makefiles\

   (ref: http://doc.qt.io/qt-5/qmake-manual.html)

2. login into VM (guest machine), go to the shared (w/ host machine) folder   
 $pwd
 $/media/sf_Maven/trunk/vmti/src/makefiles
 
3. run qmake
 $/home/wus1/pkg/qt_5_9_install/bin/qmake -makefile qmake_runApps.pro 
  it will generate a makefile: 
  /media/sf_Maven/trunk/vmti/src/makefiles/Makefile_x.mak 
  
4. run make 
 $pwd
 $/media/sf_Maven/trunk/vmti/src/makefiles
 $make -f Makefile_x.mak 
 