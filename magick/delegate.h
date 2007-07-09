/*
  Copyright (C) 2003 GraphicsMagick Group
  Copyright (C) 2002 ImageMagick Studio
 
  This program is covered by multiple licenses, which are described in
  Copyright.txt. You should have received a copy of Copyright.txt with this
  package; otherwise see http://www.graphicsmagick.org/www/Copyright.html.
 
  Methods to Read/Write/Invoke Delegates.
*/
#ifndef _MAGICK_DELEGATE_H
#define _MAGICK_DELEGATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*
  Delegate structure definitions.
*/
typedef struct _DelegateInfo
{
  char
    *path,
    *decode,
    *encode;

  char
    *commands;

  int
    mode;

  unsigned int
    stealth,
    spawn;

  unsigned long
    signature;

  struct _DelegateInfo
    *previous,
    *next;
} DelegateInfo;

#if defined(MAGICK_IMPLEMENTATION)

#if defined(HasGS)
#include "ps/iapi.h"
#include "ps/errors.h"
#endif

#ifndef gs_main_instance_DEFINED
# define gs_main_instance_DEFINED
typedef struct gs_main_instance_s gs_main_instance;
#endif

#if !defined(MagickDLLCall)
#  if defined(MSWINDOWS)
#    define MagickDLLCall __stdcall
#  else
#    define MagickDLLCall
#  endif
#endif

/*
  Define a vector of Ghostscript library callback functions so that
  DLL/shared and static Ghostscript libbraries may be handled identically.
  These definitions must be compatible with those in the Ghostscript API
  headers (which we don't require).
  */
typedef struct _GhostscriptVectors
{
  /* Exit the interpreter */
  int  (MagickDLLCall *exit)(gs_main_instance *instance);
  /* Destroy instance of Ghostscript.  Call exit first! */
  void (MagickDLLCall *delete_instance)(gs_main_instance *instance);
  /* Initialize the Ghostscript interpreter */
  int  (MagickDLLCall *init_with_args)(gs_main_instance *instance,int argc,
                                       char **argv);
  /* Create a new instance of the Ghostscript interpreter */
  int  (MagickDLLCall *new_instance)(gs_main_instance **pinstance,
                                     void *caller_handle);
  /* Execute string command in Ghostscript interpreter */
  int  (MagickDLLCall *run_string)(gs_main_instance *instance,const char *str,
                                   int user_errors,int *pexit_code);
} GhostscriptVectors;
#endif /* MAGICK_IMPLEMENTATION */

/*
  Magick delegate methods.
*/
extern MagickExport char
  *GetDelegateCommand(const ImageInfo *image_info,Image *image,
                      const char *decode,const char *encode,
                      ExceptionInfo *exception);

extern MagickExport const DelegateInfo
  *GetDelegateInfo(const char *decode,const char *encode,
                   ExceptionInfo *exception);

extern MagickExport DelegateInfo
  *SetDelegateInfo(DelegateInfo *);

extern MagickExport unsigned int
  InvokePostscriptDelegate(const unsigned int verbose,const char *command),
  InvokeDelegate(ImageInfo *image_info,Image *image,const char *decode,
                 const char *encode,ExceptionInfo *exception),
  ListDelegateInfo(FILE *file,ExceptionInfo *exception);

extern MagickExport void
  DestroyDelegateInfo(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
