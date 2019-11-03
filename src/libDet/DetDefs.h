#ifndef _DET_DEFS_H
#define _DET_DEFS_H

#ifdef _WINDOWS
//------- win32 dll ------------
# if _USRDLL
#	ifdef DET_EXPORTS
#		define DET_EXPORT __declspec( dllexport )
#	else
#		define DET_EXPORT __declspec( dllimport )
#	endif
# else
#	define DET_EXPORT
# endif
#else
//------- linux static ------------
#	define DET_EXPORT
#endif
#endif


