/*
% Copyright (C) 2003 GraphicsMagick Group
% Copyright (C) 2003 ImageMagick Studio
%
% This program is covered by multiple licenses, which are described in
% Copyright.txt. You should have received a copy of Copyright.txt with this
% package; otherwise see http://www.graphicsmagick.org/www/Copyright.html.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%         IIIII  M   M       CCCC    OOO   M   M  PPPP    AAA   TTTTT         %
%           I    MM MM      C       O   O  MM MM  P   P  A   A    T           %
%           I    M M M      C       O   O  M M M  PPPP   AAAAA    T           %
%           I    M   M      C       O   O  M   M  P      A   A    T           %
%         IIIII  M   M       CCCC    OOO   M   M  P      A   A    T           %
%                                                                             %
%                                                                             %
%                     ImageMagick Compatability Methods                       %
%                                                                             %
%                                                                             %
%                             Software Design                                 %
%                               John Cristy                                   %
%                              January 2003                                   %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/

/*
  Include declarations.
*/
#include "magick/studio.h"
#include "magick/constitute.h"
#include "magick/error.h"
#include "magick/utility.h"
#include "wand/wand_api.h"
#include "wand/magick_compat.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   C o p y M a g i c k S t r i n g                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  CopyMagickString() copies the source string to the destination string.  The
%  destination buffer is always null-terminated even if the string must be
%  truncated.
%
%  The format of the CopyMagickString method is:
%
%      size_t CopyMagickString(const char *destination,char *source,
%        const size_t length)
%
%  A description of each parameter follows:
%
%    o destination: The destination string.
%
%    o source: The source string.
%
%    o length: The length of the destination string.
%
%
*/
WandExport size_t CopyMagickString(char *destination,const char *source,
  const size_t length)
{
#if defined(HAVE_STRLCPY)
  return(strlcpy(destination,source,length));
#else
  {
    register char
      *q;

    register const char
      *p;

    register size_t
      i;

    p=source;
    q=destination;
    i=length;
    if ((i != 0) && (--i != 0))
      do
      {
        if ((*q++=(*p++)) == '\0')
          break;
      } while (--i != 0);
    if (i == 0)
      {
        if (length != 0)
          *q='\0';
        while (*p++ != '\0');
      }
    return(p-source-1);
  }
#endif
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  F o r m a t M a g i c k S t r i n g                                        %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  FormatMagickString() prints formatted output of a variable argument list.
%
%  The format of the FormatMagickString method is:
%
%      void FormatMagickString(char *string,const size_t length,
%        const char *format,...)
%
%  A description of each parameter follows.
%
%   o string:  FormatMagickString() returns the formatted string in this
%     character buffer.
%
%   o length: The maximum length of the string.
%
%   o format:  A string describing the format to use to write the remaining
%     arguments.
%
%
*/
WandExport int FormatMagickString(char *string,const size_t length,
  const char *format,...)
{
  int
    count;

  va_list
    operands;

  va_start(operands,format);
#if defined(HAVE_VSNPRINTF)
  count=vsnprintf(string,length,format,operands);
#else
  count=vsprintf(string,format,operands);
#endif
  va_end(operands);
  return(count);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   C o n c a t e n a t e M a g i c k S t r i n g                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ConcatenateMagickString() concatenates the source string to the destination
%  string.  The destination buffer is always null-terminated even if the
%  string must be truncated.
%
%  The format of the ConcatenateMagickString method is:
%
%      size_t ConcatenateMagickString(char *destination,const char *source,
%        const size_t length)
%
%  A description of each parameter follows:
%
%    o destination: The destination string.
%
%    o source: The source string.
%
%    o length: The length of the destination string.
%
%
*/
WandExport size_t ConcatenateMagickString(char *destination,
  const char *source,const size_t length)
{
#if defined(HAVE_STRLCAT)
  return(strlcat(destination,source,length));
#else
  {
    register char
      *q;

    register const char
      *p;

    register size_t
      i;

    size_t
      count;

    p=source;
    q=destination;
    i=length;
    while ((i-- != 0) && (*q != '\0'))
      q++;
    count=q-destination;
    i=length-count;
    if (i == 0)
      return(count+strlen(p));
    while (*p != '\0')
    {
      if (i != 1)
        {
          *q++=(*p);
          i--;
        }
      p++;
    }
    *q='\0';
    return(count+(p-source));
  }
#endif
}
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   I m p o r t I m a g e P i x e l s                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ImportImagePixels() accepts pixel data and stores in the image at the
%  location you specify.  The method returns True on success otherwise False
%  if an error is encountered.  The pixel data can be either char, short int,
%  int, long, float, or double in the order specified by map.
%
%  Suppose your want want to upload the first scanline of a 640x480 image from
%  character data in red-green-blue order:
%
%      ImportImagePixels(image,0,0,640,1,"RGB",CharPixel,pixels);
%
%  The format of the ImportImagePixels method is:
%
%      unsigned int ImportImagePixels(Image *image,const long x_offset,
%        const long y_offset,const unsigned long columns,
%        const unsigned long rows,const char *map,const StorageType type,
%        const void *pixels)
%
%  A description of each parameter follows:
%
%    o image: The image.
%
%    o x_offset, y_offset, columns, rows:  These values define the perimeter
%      of a region of pixels you want to define.
%
%    o map:  This string reflects the expected ordering of the pixel array.
%      It can be any combination or order of R = red, G = green, B = blue,
%      A = alpha, C = cyan, Y = yellow, M = magenta, K = black, or
%      I = intensity (for grayscale).
%
%    o type: Define the data type of the pixels.  Float and double types are
%      normalized to [0..1] otherwise [0..MaxRGB].  Choose from these types:
%      CharPixel, ShortPixel, IntegerPixel, LongPixel, FloatPixel, or
%      DoublePixel.
%
%    o pixels: This array of values contain the pixel components as defined by
%      map and type.  You must preallocate this array where the expected
%      length varies depending on the values of width, height, map, and type.
%
%
*/
WandExport unsigned int ImportImagePixels(Image *image,const long x_offset,
  const long y_offset,const unsigned long columns,const unsigned long rows,
  const char *map,const StorageType type,const void *pixels)
{
  Image
    *constitute_image;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);

  constitute_image=
    ConstituteImage(columns,rows,map,type,pixels,&image->exception);
  if (constitute_image)
    {
      CompositeImage(image,CopyCompositeOp,constitute_image,x_offset,
                     y_offset);
      DestroyImage(constitute_image);
      return (image->exception.severity == UndefinedException);
    }
  return (False);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   P a r s e A b s o l u t e G e o m e t r y                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ParseAbsoluteGeometry() returns a region as defined by the geometry string.
%
%  The format of the ParseAbsoluteGeometry method is:
%
%      unsigned int ParseAbsoluteGeometry(const char *geometry,
%        RectangeInfo *region_info)
%
%  A description of each parameter follows:
%
%    o geometry:  The geometry (e.g. 100x100+10+10).
%
%    o region_info: The region as defined by the geometry string.
%
%
*/
WandExport unsigned int ParseAbsoluteGeometry(const char *geometry,
  RectangleInfo *region_info)
{
  unsigned int
    flags;

  flags=GetGeometry(geometry,&region_info->x,&region_info->y,
    &region_info->width,&region_info->height);
  return(flags);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   P a r s e G e o m e t r y                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ParseGeometry() parses a geometry specification and returns the sigma,
%  rho, xi, and psi values.  It also returns flags that indicates which
%  of the four values (sigma, rho, xi, psi) were located in the string, and
%  whether the xi or pi values are negative.  In addition, there are flags to
%  report any meta characters (%, !, <, or >).
%
%  The format of the ParseGeometry method is:
%
%      unsigned int ParseGeometry(const char *geometry,
%        GeometryInfo *geometry_info)
%
%  A description of each parameter follows:
%
%    o geometry:  The geometry.
%
%    o geometry_info:  returns the parsed width/height/x/y in this structure.
%
%
*/
WandExport unsigned int ParseGeometry(const char *geometry,
  GeometryInfo *geometry_info)
{
  char
    *p,
    pedantic_geometry[MaxTextExtent],
    *q;

  unsigned int
    flags;

  /*
    Remove whitespaces meta characters from geometry specification.
  */
  assert(geometry_info != (GeometryInfo *) NULL);
  flags=NoValue;
  if ((geometry == (char *) NULL) || (*geometry == '\0'))
    return(flags);
  if (strlen(geometry) >= MaxTextExtent)
    return(flags);
  (void) strncpy(pedantic_geometry,geometry,MaxTextExtent-1);
  for (p=pedantic_geometry; *p != '\0'; )
  {
    if (isspace((int) (*p)))
      {
        (void) strcpy(p,p+1);
        continue;
      }
    switch (*p)
    {
      case '%':
      {
        flags|=PercentValue;
        (void) strcpy(p,p+1);
        break;
      }
      case '!':
      {
        flags|=AspectValue;
        (void) strcpy(p,p+1);
        break;
      }
      case '<':
      {
        flags|=LessValue;
        (void) strcpy(p,p+1);
        break;
      }
      case '>':
      {
        flags|=GreaterValue;
        (void) strcpy(p,p+1);
        break;
      }
      case '@':
      {
        flags|=AreaValue;
        (void) strcpy(p,p+1);
        break;
      }
      case '-':
      case '.':
      case '+':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case 'x':
      case 'X':
      case ',':
      case '/':
      {
        p++;
        break;
      }
      default:
      {
        ExceptionInfo
          exception;

        Image
          *image;

        ImageInfo
          *image_info;

        /*
          See if we can grab geometry from an image.
        */
        GetExceptionInfo(&exception);
        image_info=CloneImageInfo((ImageInfo *) NULL);
        (void) strncpy(image_info->filename,geometry,MaxTextExtent-1);
        image=PingImage(image_info,&exception);
        if (image != (Image *) NULL)
          {
            geometry_info->rho=image->columns;
            geometry_info->sigma=image->rows;
            flags|=RhoValue | SigmaValue;
            DestroyImage(image);
          }
        DestroyImageInfo(image_info);
        DestroyExceptionInfo(&exception);
        return(flags);
      }
    }
  }
  /*
    Parse rho, sigma, xi, and psi.
  */
  p=pedantic_geometry;
  if (*p == '\0')
    return(flags);
  q=p;
  (void) strtod(p,&q);
  if ((*q == 'x') || (*q == 'X') || (*q == '/') || (*q == ',') || (*q =='\0'))
    {
      /*
        Parse rho.
      */
      q=p;
      if (LocaleNCompare(p,"0x",2) == 0)
        geometry_info->rho=strtol(p,&p,10);
      else
        geometry_info->rho=strtod(p,&p);
      if (p != q)
        flags|=RhoValue;
    }
  if ((*p == 'x') || (*p == 'X') || (*p == '/') || (*p == ','))
    {
      /*
        Parse sigma.
      */
      p++;
      q=p;
      geometry_info->sigma=strtod(p,&p);
      if (p != q)
        flags|=SigmaValue;
    }
  if ((*p == '+') || (*p == '-') || (*p == ',') || (*p == '/'))
    {
      /*
        Parse xi value.
      */
      if ((*p == ',') || (*p == '/'))
        p++;
      q=p;
      geometry_info->xi=strtod(p,&p);
      if (p != q)
        {
          flags|=XiValue;
          if (*q == '-')
            flags|=XiNegative;
        }
      if ((*p == '+') || (*p == '-') || (*p == ',') || (*p == '/'))
        {
          /*
            Parse psi value.
          */
          if ((*p == ',') || (*p == '/'))
            p++;
          q=p;
          geometry_info->psi=strtod(p,&p);
          if (p != q)
            {
              flags|=PsiValue;
              if (*q == '-')
                flags|=PsiNegative;
            }
        }
    }
  return(flags);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e l i n q u i s h M a g i c k M e m o r y                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  RelinquishMagickMemory() frees memory that has already been allocated with
%  AcquireMagickMemory().
%
%  The format of the RelinquishMagickMemory method is:
%
%      void *RelinquishMagickMemory(void *memory)
%
%  A description of each parameter follows:
%
%    o memory: A pointer to a block of memory to free for reuse.
%
%
*/
WandExport void *RelinquishMagickMemory(void *memory)
{
  if (memory == (void *) NULL)
    return((void *) NULL);
  free(memory);
  return((void *) NULL);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e s i z e M a g i c k M e m o r y                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ResizeMagickMemory() changes the size of the memory and returns a pointer to
%  the (possibly moved) block.  The contents will be unchanged up to the
%  lesser of the new and old sizes.
%
%  The format of the ResizeMagickMemory method is:
%
%      void *ResizeMagickMemory(void *memory,const size_t size)
%
%  A description of each parameter follows:
%
%    o memory: A pointer to a memory allocation.  On return the pointer
%      may change but the contents of the original allocation will not.
%
%    o size: The new size of the allocated memory.
%
%
*/
WandExport void *ResizeMagickMemory(void *memory,const size_t size)
{
  void
    *allocation;

  if (memory == (void *) NULL)
    return(AcquireMagickMemory(size));
  allocation=realloc(memory,size);
  if (allocation == (void *) NULL)
    RelinquishMagickMemory(memory);
  return(allocation);
}

