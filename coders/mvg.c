/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                            M   M  V   V   GGGG                              %
%                            MM MM  V   V  G                                  %
%                            M M M  V   V  G GG                               %
%                            M   M   V V   G   G                              %
%                            M   M    V     GGG                               %
%                                                                             %
%                    Read/Write ImageMagick Image Format.                     %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                 April 2000                                  %
%                                                                             %
%                                                                             %
%  Copyright (C) 2000 ImageMagick Studio, a non-profit organization dedicated %
%  to making software imaging solutions freely available.                     %
%                                                                             %
%  Permission is hereby granted, free of charge, to any person obtaining a    %
%  copy of this software and associated documentation files ("ImageMagick"),  %
%  to deal in ImageMagick without restriction, including without limitation   %
%  the rights to use, copy, modify, merge, publish, distribute, sublicense,   %
%  and/or sell copies of ImageMagick, and to permit persons to whom the       %
%  ImageMagick is furnished to do so, subject to the following conditions:    %
%                                                                             %
%  The above copyright notice and this permission notice shall be included in %
%  all copies or substantial portions of ImageMagick.                         %
%                                                                             %
%  The software is provided "as is", without warranty of any kind, express or %
%  implied, including but not limited to the warranties of merchantability,   %
%  fitness for a particular purpose and noninfringement.  In no event shall   %
%  ImageMagick Studio be liable for any claim, damages or other liability,    %
%  whether in an action of contract, tort or otherwise, arising from, out of  %
%  or in connection with ImageMagick or the use or other dealings in          %
%  ImageMagick.                                                               %
%                                                                             %
%  Except as contained in this notice, the name of the ImageMagick Studio     %
%  shall not be used in advertising or otherwise to promote the sale, use or  %
%  other dealings in ImageMagick without prior written authorization from the %
%  ImageMagick Studio.                                                        %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/

/*
  Include declarations.
*/
#include "magick.h"
#include "defines.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d M V G I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method ReadMVGImage creates a gradient image and initializes it to
%  the X server color range as specified by the filename.  It allocates the
%  memory necessary for the new Image structure and returns a pointer to the
%  new image.
%
%  The format of the ReadMVGImage method is:
%
%      Image *ReadMVGImage(const ImageInfo *image_info,ExceptionInfo *exception)
%
%  A description of each parameter follows:
%
%    o image:  Method ReadMVGImage returns a pointer to the image after
%      creating it. A null image is returned if there is a memory shortage
%      or if the image cannot be read.
%
%    o image_info: Specifies a pointer to an ImageInfo structure.
%
%    o exception: return any errors or warnings in this structure.
%
%
*/
static Image *ReadMVGImage(const ImageInfo *image_info,ExceptionInfo *exception)
{
  char
    filename[MaxTextExtent];

  DrawInfo
    *draw_info;

  Image
    *image;

  /*
    Initialize Image structure.
  */
  image=AllocateImage(image_info);
  (void) strcpy(image->filename,image_info->filename);
  if (image->columns == 0)
    image->columns=640;
  if (image->rows == 0)
    image->rows=480;
  SetImage(image,OpaqueOpacity);
  draw_info=CloneDrawInfo(image_info,(DrawInfo *) NULL);
  (void) strcpy(filename,"@");
  (void) strcat(filename,image_info->filename);
  (void) CloneString(&draw_info->primitive,filename);
  (void) DrawImage(image,draw_info);
  DestroyDrawInfo(draw_info);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e g i s t e r M V G I m a g e                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method RegisterMVGImage adds attributes for the MVG image format
%  to the list of supported formats.  The attributes include the image format
%  tag, a method to read and/or write the format, whether the format
%  supports the saving of more than one frame to the same file or blob,
%  whether the format supports native in-memory I/O, and a brief
%  description of the format.
%
%  The format of the RegisterMVGImage method is:
%
%      RegisterMVGImage(void)
%
*/
ModuleExport void RegisterMVGImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("MVG");
  entry->decoder=ReadMVGImage;
  entry->adjoin=False;
  entry->description=AllocateString("Magick Vector Graphics");
  entry->module=AllocateString("MVG");
  RegisterMagickInfo(entry);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U n r e g i s t e r M V G I m a g e                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method UnregisterMVGImage removes format registrations made by the
%  MVG module from the list of supported formats.
%
%  The format of the UnregisterMVGImage method is:
%
%      UnregisterMVGImage(void)
%
*/
ModuleExport void UnregisterMVGImage(void)
{
  UnregisterMagickInfo("MVG");
}
