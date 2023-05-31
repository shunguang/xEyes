/*
*------------------------------------------------------------------------
*popt_pp.h
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
#ifndef _INCLUDED_POPT_PP_H_
#define _INCLUDED_POPT_PP_H_

#include <popt.h>

class POpt{
protected:
  poptContext con;
public:
  // creation and deletion
  POpt(const char *name, int argc, const char **argv,
       const poptOption *options, int flags)
    {con = poptGetContext(name,argc,argv,options,flags);}
  POpt(const char *name, int argc, char **argv,
       const poptOption *options, int flags)
    {con = poptGetContext(name,argc,(const char **)argv,options,flags);}
  ~POpt()
    {poptFreeContext(con);}

  // functions for processing options
  int getNextOpt()
    {return(poptGetNextOpt(con));}
  void ignoreOptions()
    {while(getNextOpt() >= 0);}
  const char *getOptArg()
    {return(poptGetOptArg(con));}
  const char *strError(int error)
    {return(poptStrerror(error));}
  const char *badOption(int flags = POPT_BADOPTION_NOALIAS)
    {return(poptBadOption(con,flags));}

  // processing other arguments
  const char *getArg()
    {return(poptGetArg(con));}
  void ignoreArgs()
    {while(getArg());}
};

#endif
