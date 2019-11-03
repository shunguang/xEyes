/*
 *----------------------------------------------------------------------------------
 * DataTypes.h - the defines of data types
 *
 * date:		9/28/2019
 * author:		shunguang@yahoo.com
 *
 *
 * Copyright 2019 xEyes.com
 *----------------------------------------------------------------------------------
 */

#ifndef __DATA_TYPES_H__
#define __DATA_TYPES_H__

#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <assert.h>

#include <cstddef>  
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <deque>
#include <locale>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>  

//---------- boost ---------
#define APP_USE_CUDA 0
#define APP_USE_BOOST 1
#if APP_USE_BOOST
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

//only used in Cfg project to/read xml files
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#endif

//---------- opencv ---------
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/videostab/global_motion.hpp>
#include <opencv2/features2d.hpp>
//#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

//--------- my owen defs ----------------
#define float32		float
#define float64		double

#define APP_EPS		    (1e-15)
#define APP_EPS2		(1e-6)

#define APP_REALMIN_32F (1e-38f)
#define APP_REALMAX_32F (1e+38f)
#define APP_REALMIN_64F (1e-308)
#define APP_REALMAX_64F (1e+308)

#define APP_MAX_uint16_t_t (0xFFFF)
#define APP_MAX_uint32_t_t (0xFFFFFFFF)
#define APP_MAX_UINT64 (0xFFFFFFFFFFFFFFFF)
#define APP_SEQ_END	   (0xFFFFFFFF)
#define APP_NAN_uint32_t_t (0xFFFFFFFF)

#define APP_HALF_PI       (1.57079632679490)
#define APP_PI            (3.14159265358979)
#define APP_TWO_PI        (6.28318530717959)
#define APP_D2R           (0.01745329251994)
#define APP_R2D           (57.29577951308232)

#define APP_ROUND(x)	( (int) floor( x + 0.500 ) )
#define APP_NAN			( sqrt(-1.0) )
#define APP_ISNAN(x)	( x != x )

#define APP_MAX(a,b)	( (a) > (b) ? (a) : (b) )
#define APP_MIN(a,b)	( (a) > (b) ? (b) : (a) )

#define USLEEP_1_SEC		1000000
#define USLEEP_1_MILSEC		1000

#if APP_USE_BOOST
#   define APP_LOCAL_TIME	    boost::posix_time::microsec_clock::local_time()
#   define APP_UNIVERSAL_TIME   boost::posix_time::microsec_clock::universal_time()
#   define THREAD_SLEEP( ms )   boost::this_thread::sleep(boost::posix_time::milliseconds(ms))
#else
#   define THREAD_SLEEP( ms )   std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#   define APP_LOCAL_TIME	    std::chrono::system_clock::now()
#   define APP_UNIVERSAL_TIME   std::chrono::system_clock::now()
#endif
#define LOG_MAX_MSG_LEN         2048
#endif
