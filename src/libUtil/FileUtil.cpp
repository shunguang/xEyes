/*
*------------------------------------------------------------------------
*FileUtil.cpp
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
 *------------------------------------------------------------------------------
 * Copyright � 2012 Shunguang Wu (SWU)
 *
 * This program is developed by SWU in his personal interesting in evenings and holidays. 
 * SWU IS NOT supported by any angency during this work. Use, redistribute, or modify
 * is possible based on request ( shunguang@yahoo.com, 732-434-5523 (cell) )
 *------------------------------------------------------------------------------
 */
#include "FileUtil.h"
using namespace std;

void xeyes::getExtName( const std::string &fileName, std::string &ext )
{
	size_t n1 = fileName.find_last_of('.');
	size_t n2 = fileName.length();

	ext = fileName.substr(n1+1, n2-n1+1);
	for(size_t i=0; i<ext.length(); i++){
		ext[i] = toupper( ext[i]);
	}
}

uint32_t xeyes::getFileNameList( const std::string &dirName, const std::string &ext, std::vector<std::string> &vFileNames )
{
	string fName, extName;
	string ext0 = ext;
	int L = ext0.length();

	vFileNames.clear();
#if 0 //_WINDOWS
    _finddata_t c_file;
    intptr_t hFile;
     
	_chdir ( dirName.c_str() );

    // Find any file in current directory
    if( (hFile = _findfirst( "*.*", &c_file )) == -1 ){ 
		printf("CaptureFrmImg :: getFileNameList(): Error!\n" ); // This doesn't mean current directory is empty.
		return 0;
    }

	//read file names into $vRetNames$
	do {
		// if the file is not a sub dir...
		if( ( c_file.attrib & _A_SUBDIR ) != _A_SUBDIR ){ // _A_SUBDIR = 0x10 (16 in decimal). Also note the single &.
			//printf( "%s\n", c_file.name );
			fName = string(c_file.name);
			
			//xeyes::getExtName( fName, extName );
			extName = fName.substr(fName.length() - L);
			if( 0==ext0.compare(extName) || 0==ext.compare("*") ){
				vFileNames.push_back( fName );
			}
		}
	} while( _findnext( hFile, &c_file ) == 0 ); // find next file
	_findclose( hFile );

	//sort file names
	sort ( vFileNames.begin(), vFileNames.end() );

#if 0
	for( size_t i=0; i<vFileNames.size(); ++i){
		printf("i=%d, %s\n", i, vFileNames[i].c_str() );
	}
#endif
#else
	int L0 = dirName.length();
	boost::filesystem::path targetDir(dirName);
	boost::filesystem::recursive_directory_iterator iter(targetDir), eod;
	BOOST_FOREACH(boost::filesystem::path const& i, make_pair(iter, eod)) {
		if (is_regular_file(i)) {
			fName = i.string().substr(L0+1);

			extName = fName.substr(fName.length() - L);
			if (0 == ext0.compare(extName) || 0 == ext.compare("*")) {
				vFileNames.push_back(fName);
			}
		}
	}
#endif
	return vFileNames.size();
}


bool xeyes :: folderExists( const string &strPath )
{  
	if ( access( strPath.c_str(), 0 ) == 0 ){
		struct stat status;
		stat( strPath.c_str(), &status );
		if ( status.st_mode & S_IFDIR ){
			return true;
		}
		else{
#if 0
			printf( "The path you entered is a file." );
			if( remove( strPath.c_str() ) != 0 ){
				printf( "Error deleting file" );
				assert(0);
			}
			else{
				printf("File successfully deleted");
			}
#endif
			return false;
		}
	}
	else{
		return false;
	}
}

bool xeyes ::  fileExists(const std::string& name) 
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

bool xeyes::findSeqName( const std::string &inSeqFileName, std::string &seqName )
{
	string t = inSeqFileName;
	size_t p1 = t.find_last_of('/\\' );
	if( p1 < 0 )
		return false;

	t.replace(p1,1,"a");
	size_t p2 = t.find_last_of('/\\' );
	if( p2 < 0 )
		return false;
	
	seqName = inSeqFileName.substr(p2+1, p1-p2-1 );
	return true;	
}

bool xeyes::mkDumDirs( const string &inSeqFileName, const string &dumpPathRoot, string &dumpPath4Debug, string &dumpPath4Customer )
{

	string seqName;
	if( !findSeqName( inSeqFileName, seqName ) ){
		return false;
	}
	printf ("seqName=%s\n", seqName.c_str() );
	printf ("dumpPathRoot=%s\n", dumpPathRoot.c_str() );
#if 0
	dumpPath4Debug = dumpPathRoot + string("\\Results-") + seqName + string("-debug");
	dumpPath4Customer = dumpPathRoot+"\\Results-" + seqName + "-out";
	printf ("dumpPath4Debug=%s\n", dumpPath4Debug.c_str() );
	printf ("dumpPath4Customer=%s\n", dumpPath4Customer.c_str() );

	if( !folderExists( dumpPath4Debug ) ){
		mkdir( dumpPath4Debug.c_str() );
	}

	if( !folderExists( dumpPath4Customer ) ){
		mkdir( dumpPath4Customer.c_str() );
	}
#endif

	return true;
}


std::string xeyes::getCurDir()
{
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
		return std::string("");
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	printf("The current working directory is %s", cCurrentPath);
	
	return std::string(cCurrentPath);

}

void  xeyes::deleteFilesInFolder(const std::string &folderPath)
{
	std::vector<std::string> vFileNames;
	uint32_t n = getFileNameList(folderPath, "*", vFileNames);
	string filepath;
	for (uint32_t i=0; i<n; ++i){
		filepath = folderPath + "/" + vFileNames[i];
		remove( filepath.c_str() );
	}
	printf("totally %d files are removed from %s\n", n, folderPath.c_str());
}

void xeyes::myCreateDir(const std::string &p)
{
	boost::filesystem::path p0(p);
	if (!boost::filesystem::exists(p0)) {
		if (!boost::filesystem::create_directories(p0)) {
			printf("xeyes::myCreateDir(): cannot create root folder:%s", p0.string().c_str());
		}
	}
}

void xeyes::splitExt(const std::string &fPath, std::string &head, std::string &ext)
{
	int id = fPath.find_last_of('.');
	if (id == std::string::npos) return;

	head = fPath.substr(0, id);
	ext = fPath.substr(id + 1);
}

void xeyes::splitFolder(const std::string &fPath, std::string &fDir, std::string &fName)
{
	boost::filesystem::path p(fPath);
	boost::filesystem::path dir = p.parent_path();
	boost::filesystem::path name = p.filename();

	fDir = dir.generic_string();
	fName = name.generic_string();
}



bool xeyes::isVideoFile(const std::string &filePath)
{
	std::string  head, ext;

	splitExt(filePath, head, ext);

	if (0 == ext.compare("AVI")) {
		return true;
	}
	if (0 == ext.compare("MP4")) {
		return true;
	}

	return false;
}

bool xeyes::isImgeFile(const std::string &filePath)
{
	std::string  head, ext;

	splitExt(filePath, head, ext);

	if (0 == ext.compare("BMP")) {
		return true;
	}
	if (0 == ext.compare("JPG")) {
		return true;
	}
	if (0 == ext.compare("PNG")) {
		return true;
	}
	if (0 == ext.compare("GIF")) {
		return true;
	}
	if (0 == ext.compare("JPEG")) {
		return true;
	}
	if (0 == ext.compare("PBM")) {
		return true;
	}
	if (0 == ext.compare("PGM")) {
		return true;
	}
	if (0 == ext.compare("PPM")) {
		return true;
	}
	if (0 == ext.compare("XBM")) {
		return true;
	}
	if (0 == ext.compare("XPM")) {
		return true;
	}

	return false;
}
