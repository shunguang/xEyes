#ifndef _CFG_DEFS_H
#define _CFG_DEFS_H

#ifdef _WINDOWS
//------- win32 dll ------------
# if _USRDLL
#	ifdef CFG_EXPORTS
#		define CFG_EXPORT __declspec( dllexport )
#	else
#		define CFG_EXPORT __declspec( dllimport )
#	endif
# else
#	define CFG_EXPORT
# endif
//------ win32 static lib ------
#else
#	define CFG_EXPORT
#endif

#endif


