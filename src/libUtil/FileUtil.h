/*
*------------------------------------------------------------------------
*FileUtil.h
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
*
* THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
*
* IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTOR BE LIABLE FOR
* ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
* WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
* LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
* OF THIS SOFTWARE.
*
* Permission to use, copy, modify, distribute, and sell this software and
* its documentation for any purpose is prohibited unless it is granted under
* the author's written notice.
*
* Copyright(c) 2020 by Shunguang Wu, All Right Reserved
*-------------------------------------------------------------------------
*/
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
