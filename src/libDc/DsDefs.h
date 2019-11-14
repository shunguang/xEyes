/**
*------------------------------------------------------------------------------
* @file   DsDefs.h
* @author Shunguang.Wu@gmail.com
* @date   10/05, 2019
* 
*
* Copyright � xEyes.com 2019-today
*------------------------------------------------------------------------------
**/

#ifndef __DS_DEFS_H__
#define __DS_DEFS_H__

#ifdef _WINDOWS
#if _USRDLL
#		ifdef DS_EXPORTS
#			define DS_EXPORT __declspec( dllexport )
#		else
#			define DS_EXPORT __declspec( dllimport )
#		endif
#	else
#		define DS_EXPORT
#	endif
#else
#	define DS_EXPORT
#endif
#endif

