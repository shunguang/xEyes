#ifndef _DSP_DEFS_H
#define _DSP_DEFS_H

#ifdef _WINDOWS
//------- win32 dll ------------
# if _USRDLL
#	ifdef DSP_EXPORTS
#		define DSP_EXPORT __declspec( dllexport )
#	else
#		define DSP_EXPORT __declspec( dllimport )
#	endif
# else
#	define DSP_EXPORT
# endif
#else
//------- linux static ------------
#	define DSP_EXPORT
#endif
#endif


