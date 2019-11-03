/*
adopted from John Cole's Common/c++/cpplib
*/

#ifndef __UTIL_FUNCS_H__
#define __UTIL_FUNCS_H__

#include "DataTypes.h"
#include <random>
#include <chrono>
#include "UtilDefs.h"

namespace xeyes {
	std::string UTIL_EXPORT num_to_string( const bool x ,const std::string &msg="");
	std::string UTIL_EXPORT num_to_string(const uint8_t x,const std::string &msg = "");

	std::string UTIL_EXPORT num_to_string( const int32_t x, const int len, const std::string &msg="" );
	std::string UTIL_EXPORT num_to_string( const uint16_t x, const int len, const std::string &msg="" );
	std::string UTIL_EXPORT num_to_string( const uint32_t x, const int len, const std::string &msg="" );
	std::string UTIL_EXPORT num_to_string( const uint64_t x, const int len, const std::string &msg="" );

	std::string UTIL_EXPORT num_to_string(const int32_t x,  const std::string &msg = "");
	std::string UTIL_EXPORT num_to_string(const uint16_t x, const std::string &msg = "");
	std::string UTIL_EXPORT num_to_string(const uint32_t x, const std::string &msg = "");
	std::string UTIL_EXPORT num_to_string(const uint64_t x, const std::string &msg = "");

	std::string UTIL_EXPORT num_to_string( const float x,	const std::string &msg="" );
	std::string UTIL_EXPORT num_to_string(const double x, const std::string &msg = "");

	std::string UTIL_EXPORT vec_to_string( const std::vector<std::string> &v, const std::string &msg="" );

	bool	UTIL_EXPORT string_in_vector( const std::string &s, const std::vector<std::string> &v );
	int		UTIL_EXPORT string_in_vector_relax( const std::string &s, const std::vector<std::string> &v );

	bool	UTIL_EXPORT string_to_bool( const std::string &s );
	uint64_t	UTIL_EXPORT string_to_uint64( const std::string &s );
	uint32_t	UTIL_EXPORT string_to_vector( std::vector<std::string> &v, const std::string &s, const std::string &key, const bool keepKey=false );
	uint32_t	UTIL_EXPORT string_to_vector2( std::vector<std::string> &v, const std::string &s, const std::string &key, const bool keepKey=false );

	int8_t	UTIL_EXPORT int32_t_to_int8_t( const int32_t x );

	std::string	UTIL_EXPORT vector_to_string( const std::vector<std::string> &v, const std::string &seperateKey );

	//remove all chars $ch$ at the beging and end of the string.
	void	UTIL_EXPORT string_trim( std::string &x, const char ch=' ' );
	void	UTIL_EXPORT string_trim( std::string &x, const std::vector<char> &keys );
	void	UTIL_EXPORT string_find_n_replace( std::string &x, const char chFind, const char chRaplace );

	std::string  UTIL_EXPORT string_parse( const std::string &x, const std::string &key );
	std::string  UTIL_EXPORT string_find_mac( const std::string &x, const char &seperationKey=':' );

	//process vector of string
	void UTIL_EXPORT vstr_read_txt_file( std::vector<std::string> &vLines, const std::string &fileName, const char commentChar );
	void UTIL_EXPORT vstr_select_util( std::vector<std::string> &retLines, const std::vector<std::string> &vAllLines, const std::string &startKey, const std::string &stopKey );
	std::string UTIL_EXPORT vstr_find_value(  const std::vector<std::string> &vLines, const std::string &key );

	//time related funcs
	std::string UTIL_EXPORT getPrettyTimeStamp( const bool dayTimeOnly=false );
	std::string UTIL_EXPORT getPrettyTimeStamp(const boost::posix_time::ptime &t, const bool dayTimeOnly = false);


	uint32_t UTIL_EXPORT checkTimeOut( const boost::posix_time::ptime &start, const uint32_t thdInMillisec );
	uint32_t UTIL_EXPORT timeIntervalMillisec( const boost::posix_time::ptime &start );
	uint32_t UTIL_EXPORT timeIntervalMillisec( const boost::posix_time::ptime &start, const boost::posix_time::ptime &end );

	void UTIL_EXPORT printfStr( const std::string &s, const std::string &msg );

	//generate about <n> distinct/unique uniform distributed random number in [a,b]
	void UTIL_EXPORT genUniformDistinctRands( std::vector<int> &v, const int n, const int a, const int b );
	void UTIL_EXPORT dumpCorners(const std::string &fPath, const std::vector<cv::Point2f> &corners);
	void UTIL_EXPORT loadCorners(const std::string &fPath, std::vector<cv::Point2f> &corners);

	uint32_t UTIL_EXPORT ipConvertStr2Num(const std::string &ip);
	std::string  UTIL_EXPORT ipConvertNum2Str(const uint32_t ip);
	std::string UTIL_EXPORT  appToString(const char *fmt, ...);
	// return true if x \in [low, up), otherwise false. (Python rule)
	bool  UTIL_EXPORT inRange( const int x, const int low, const int up);

}

#endif
