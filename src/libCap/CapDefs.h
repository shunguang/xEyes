#ifndef _CAP_DEFS_H
#define _CAP_DEFS_H

#ifdef _WINDOWS
//------- win32 dll ------------
# if _USRDLL
#	ifdef CAP_EXPORTS
#		define CAP_EXPORT __declspec( dllexport )
#	else
#		define CAP_EXPORT __declspec( dllimport )
#	endif
# else
#	define CAP_EXPORT
# endif
#else
//------- linux static ------------
#	define CAP_EXPORT
#endif
#endif


