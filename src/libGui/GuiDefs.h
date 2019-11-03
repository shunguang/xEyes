#ifndef _GUI_DEFS_H
#define _GUI_DEFS_H

#ifdef _WINDOWS
//------- win32 dll ------------
# if _USRDLL
#	ifdef GUI_EXPORTS
#		define GUI_EXPORT __declspec( dllexport )
#	else
#		define GUI_EXPORT __declspec( dllimport )
#	endif
# else
#	define GUI_EXPORT
# endif
#else
//------- linux static ------------
#	define GUI_EXPORT
#endif
#endif


