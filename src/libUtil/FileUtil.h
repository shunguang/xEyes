/*
 * \file        AppFileIo.h
 * \brief       an implementtaion of a FIFO circular queue
  *
 * \date        10/11/11
 *                                                  
 * \author      swu
 * \version     $Id:  $
 * \Copyright     All Rights Reserved
 *------------------------------------------------------------------------------
 * Copyright � 2012 Shunguang Wu (SWU)
 *
 * This program is developed by SWU in his personal interesting in evenings and holidays. 
 * SWU IS NOT supported by any angency during this work. Use, redistribute, or modify
 * is possible based on request ( shunguang@yahoo.com, 732-434-5523 (cell) )
 *------------------------------------------------------------------------------
 */
#ifndef APP_FILE_IO_H_
#define APP_FILE_IO_H_

#include <stdio.h>
#include <sys/types.h>		// For stat().
#include <sys/stat.h>		// For stat().

#include <cctype>
#include <string>
#include <vector>
#include <algorithm>

#include "DataTypes.h"

#ifdef _WINDOWS
#include <io.h>				// For access().
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include "UtilDefs.h"
namespace xeyes {
	uint32_t UTIL_EXPORT getFileNameList( const std::string &dirName, const std::string &ext, std::vector<std::string> &vFileNames );
	void UTIL_EXPORT getExtName( const std::string &fileName, std::string &ext );
	bool UTIL_EXPORT folderExists( const std::string &strPath );
	bool UTIL_EXPORT fileExists(const std::string& name);

    bool UTIL_EXPORT mkDumDirs( const std::string &inSeqFileName, const std::string &dumpPathRoot, std::string &dumpPath4Debug, std::string &dumpPath4Customer );
    bool UTIL_EXPORT findSeqName( const std::string &inSeqFileName, std::string &seqName );
	std::string UTIL_EXPORT getCurDir();
	void UTIL_EXPORT deleteFilesInFolder( const std::string &folderPath );
	void UTIL_EXPORT myCreateDir(const std::string &p);
	//fPath="c:/temp/f1.txt" ---> head = "c:/temp/f1", ext="txt" 
	void UTIL_EXPORT splitExt(const std::string &fPath, std::string &head, std::string &ext);
	//fPath="c:/temp/f1.txt" ---> fDir = "c:/temp", fName="f1.txt" 
	void UTIL_EXPORT splitFolder(const std::string &fPath, std::string &fDir, std::string &fName);

	bool UTIL_EXPORT isVideoFile(const std::string &filePath);
	bool UTIL_EXPORT isImgeFile(const std::string &filePath);
}

#endif
