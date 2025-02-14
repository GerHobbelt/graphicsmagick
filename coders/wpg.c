/*
% Copyright (C) 2003-2021 GraphicsMagick Group
% Copyright (C) 2002 ImageMagick Studio
%
% This program is covered by multiple licenses, which are described in
% Copyright.txt. You should have received a copy of Copyright.txt with this
% package; otherwise see http://www.graphicsmagick.org/www/Copyright.html.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                            W   W  PPPP    GGGG                              %
%                            W   W  P   P  G                                  %
%                            W W W  PPPP   G GGG                              %
%                            WW WW  P      G   G                              %
%                            W   W  P       GGG                               %
%                                                                             %
%                                                                             %
%                       Read WordPerfect Image Format.                        %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                              Jaroslav Fojtik                                %
%                              June 2000 - 2018                               %
%                         Rework for GraphicsMagick                           %
%                              Bob Friesenhahn                                %
%                               Feb-May 2003                                  %
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
#include "magick/blob.h"
#include "magick/colormap.h"
#include "magick/constitute.h"
#include "magick/log.h"
#include "magick/magic.h"
#include "magick/magick.h"
#include "magick/pixel_cache.h"
#include "magick/shear.h"
#include "magick/tempfile.h"
#include "magick/transform.h"
#include "magick/utility.h"


typedef struct
   {
   unsigned char Red;
   unsigned char Blue;
   unsigned char Green;
   } RGB_Record;

/* Default palette for WPG level 1 */
static const RGB_Record WPG1_Palette[256]={
{  0,  0,  0},          {  0,  0,168},
{  0,168,  0},          {  0,168,168},
{168,  0,  0},          {168,  0,168},
{168, 84,  0},          {168,168,168},
{ 84, 84, 84},          { 84, 84,252},
{ 84,252, 84},          { 84,252,252},
{252, 84, 84},          {252, 84,252},
{252,252, 84},          {252,252,252},  /*16*/
{  0,  0,  0},          { 20, 20, 20},
{ 32, 32, 32},          { 44, 44, 44},
{ 56, 56, 56},          { 68, 68, 68},
{ 80, 80, 80},          { 96, 96, 96},
{112,112,112},          {128,128,128},
{144,144,144},          {160,160,160},
{180,180,180},          {200,200,200},
{224,224,224},          {252,252,252},  /*32*/
{  0,  0,252},          { 64,  0,252},
{124,  0,252},          {188,  0,252},
{252,  0,252},          {252,  0,188},
{252,  0,124},          {252,  0, 64},
{252,  0,  0},          {252, 64,  0},
{252,124,  0},          {252,188,  0},
{252,252,  0},          {188,252,  0},
{124,252,  0},          { 64,252,  0},  /*48*/
{  0,252,  0},          {  0,252, 64},
{  0,252,124},          {  0,252,188},
{  0,252,252},          {  0,188,252},
{  0,124,252},          {  0, 64,252},
{124,124,252},          {156,124,252},
{188,124,252},          {220,124,252},
{252,124,252},          {252,124,220},
{252,124,188},          {252,124,156},  /*64*/
{252,124,124},          {252,156,124},
{252,188,124},          {252,220,124},
{252,252,124},          {220,252,124},
{188,252,124},          {156,252,124},
{124,252,124},          {124,252,156},
{124,252,188},          {124,252,220},
{124,252,252},          {124,220,252},
{124,188,252},          {124,156,252},  /*80*/
{180,180,252},          {196,180,252},
{216,180,252},          {232,180,252},
{252,180,252},          {252,180,232},
{252,180,216},          {252,180,196},
{252,180,180},          {252,196,180},
{252,216,180},          {252,232,180},
{252,252,180},          {232,252,180},
{216,252,180},          {196,252,180},  /*96*/
{180,220,180},          {180,252,196},
{180,252,216},          {180,252,232},
{180,252,252},          {180,232,252},
{180,216,252},          {180,196,252},
{0,0,112},              {28,0,112},
{56,0,112},             {84,0,112},
{112,0,112},            {112,0,84},
{112,0,56},             {112,0,28},     /*112*/
{112,0,0},              {112,28,0},
{112,56,0},             {112,84,0},
{112,112,0},            {84,112,0},
{56,112,0},             {28,112,0},
{0,112,0},              {0,112,28},
{0,112,56},             {0,112,84},
{0,112,112},            {0,84,112},
{0,56,112},             {0,28,112},     /*128*/
{56,56,112},            {68,56,112},
{84,56,112},            {96,56,112},
{112,56,112},           {112,56,96},
{112,56,84},            {112,56,68},
{112,56,56},            {112,68,56},
{112,84,56},            {112,96,56},
{112,112,56},           {96,112,56},
{84,112,56},            {68,112,56},    /*144*/
{56,112,56},            {56,112,69},
{56,112,84},            {56,112,96},
{56,112,112},           {56,96,112},
{56,84,112},            {56,68,112},
{80,80,112},            {88,80,112},
{96,80,112},            {104,80,112},
{112,80,112},           {112,80,104},
{112,80,96},            {112,80,88},    /*160*/
{112,80,80},            {112,88,80},
{112,96,80},            {112,104,80},
{112,112,80},           {104,112,80},
{96,112,80},            {88,112,80},
{80,112,80},            {80,112,88},
{80,112,96},            {80,112,104},
{80,112,112},           {80,114,112},
{80,96,112},            {80,88,112},    /*176*/
{0,0,64},               {16,0,64},
{32,0,64},              {48,0,64},
{64,0,64},              {64,0,48},
{64,0,32},              {64,0,16},
{64,0,0},               {64,16,0},
{64,32,0},              {64,48,0},
{64,64,0},              {48,64,0},
{32,64,0},              {16,64,0},      /*192*/
{0,64,0},               {0,64,16},
{0,64,32},              {0,64,48},
{0,64,64},              {0,48,64},
{0,32,64},              {0,16,64},
{32,32,64},             {40,32,64},
{48,32,64},             {56,32,64},
{64,32,64},             {64,32,56},
{64,32,48},             {64,32,40},     /*208*/
{64,32,32},             {64,40,32},
{64,48,32},             {64,56,32},
{64,64,32},             {56,64,32},
{48,64,32},             {40,64,32},
{32,64,32},             {32,64,40},
{32,64,48},             {32,64,56},
{32,64,64},             {32,56,64},
{32,48,64},             {32,40,64},     /*224*/
{44,44,64},             {48,44,64},
{52,44,64},             {60,44,64},
{64,44,64},             {64,44,60},
{64,44,52},             {64,44,48},
{64,44,44},             {64,48,44},
{64,52,44},             {64,60,44},
{64,64,44},             {60,64,44},
{52,64,44},             {48,64,44},     /*240*/
{44,64,44},             {44,64,48},
{44,64,52},             {44,64,60},
{44,64,64},             {44,60,64},
{44,55,64},             {44,48,64},
{0,0,0},                {0,0,0},
{0,0,0},                {0,0,0},
{0,0,0},                {0,0,0},
{0,0,0},                {0,0,0}         /*256*/
};


static int ApproveFormatForWPG(const char *Format)
{
  if(!strcmp(Format,"PFB")) return 0;
  return 1;
}



/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   I s W P G                                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  IsWPG() returns True if the image format type, identified by the magick
%  string, is WPG.
%
%  The format of the IsWPG method is:
%
%      unsigned int IsWPG(const unsigned char *magick,const size_t length)
%
%  A description of each parameter follows:
%
%    o status:  Method IsWPG returns True if the image format type is WPG.
%
%    o magick: This string is generally the first few bytes of an image file
%      or blob.
%
%    o length: Specifies the length of the magick string.
%
%
*/
static unsigned int IsWPG(const unsigned char *magick,const size_t length)
{
  if (length < 4)
    return(False);
  if (memcmp(magick,"\377WPC",4) == 0)
    return(True);
  return(False);
}

static MagickPassFail ReallocColormap(Image *image,unsigned int colors)
{
  PixelPacket *colormap;

  /* FIXME: This implementation would be better using a true realloc */
  colormap=MagickAllocateClearedArray(PixelPacket *,colors,sizeof(PixelPacket));
  if (colormap != (PixelPacket *) NULL)
    {
      if (image->colormap != (PixelPacket *) NULL)
        {
          (void) memcpy(colormap,image->colormap,
                        (size_t) Min(image->colors,colors)*sizeof(PixelPacket));
          MagickFreeMemory(image->colormap);
        }
      image->colormap = colormap;
      image->colors = colors;
      return MagickPass;
    }

  return MagickFail;
}

static int Rd_WP_DWORD(Image *image, unsigned long *d)
{
  unsigned char b;

  b = ReadBlobByte(image);
  *d = b;
  if (b < 0xFFU)
    return 1;
  b = ReadBlobByte(image);
  *d = (unsigned long) b;
  b = ReadBlobByte(image);
  *d+=(unsigned long) b*256l;
  if (*d < 0x8000)
    return 3;
  *d=(*d & 0x7FFF) << 16;
  b = ReadBlobByte(image);
  *d += (unsigned long) b;
  b=ReadBlobByte(image);
  *d += (unsigned long) b*256l;
  return 5;
}


static MagickPassFail InsertRow(unsigned char *p,unsigned long y, Image *image, int bpp)
{
  unsigned long
    x;
  register PixelPacket
    *q;
  MagickPassFail RetVal = MagickFail;
  IndexPacket index;
  IndexPacket *indexes;

  if (image->logging)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                          "Insert row %ld of %lu...", y, image->rows);

  q = SetImagePixels(image,0,y,image->columns,1);
  if(q == (PixelPacket *) NULL)
    return MagickFail;

  switch (bpp)
    {
    case 1:  /* Convert bitmap scanline. WP seems to ignore palette even if it is present. */
        RetVal = ImportImagePixelArea(image,GrayQuantum,bpp,p,NULL,0);
        break;

    case 4:  /* Convert PseudoColor scanline. */
    case 8:  /* Convert PseudoColor scanline. */
        RetVal = ImportImagePixelArea(image,IndexQuantum,bpp,p,NULL,0);
        break;

    case 2:  /* Convert PseudoColor scanline. */
      {
        indexes=AccessMutableIndexes(image);
        if ((image->storage_class != PseudoClass) ||
            (indexes == (IndexPacket *) NULL))
          {
            if (image->logging)
              (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                                    "Image has no colormap, skipping...");
          return MagickFail;
          }
        x = 0;
        while(x+3 < image->columns)
          {
            index = (IndexPacket)((*p >> 6) & 0x3);
            VerifyColormapIndex(image,index);
            indexes[x++]=index;
            *q++=image->colormap[index];
            index = (IndexPacket)((*p >> 4) & 0x3);
            VerifyColormapIndex(image,index);
            indexes[x++]=index;
            *q++=image->colormap[index];
            index = (IndexPacket)((*p >> 2) & 0x3);
            VerifyColormapIndex(image,index);
            indexes[x++]=index;
            *q++ = image->colormap[index];
            index = (IndexPacket)((*p) & 0x3);
            VerifyColormapIndex(image,index);
            indexes[x++]=index;
            *q++ = image->colormap[index];
            p++;
          }
        if(x < image->columns)
          {
            index = (IndexPacket) ((*p >> 6) & 0x3);
            VerifyColormapIndex(image,index);
            indexes[x++] = index;
            *q++=image->colormap[index];
            if(x < image->columns)
              {
                index = (IndexPacket) ((*p >> 4) & 0x3);
                VerifyColormapIndex(image,index);
                indexes[x++] = index;
                *q++=image->colormap[index];
                if(x < image->columns)
                  {
                    index = (IndexPacket)((*p >> 2) & 0x3);
                    VerifyColormapIndex(image,index);
                    indexes[x] = index;
                    *q++=image->colormap[index];
                  }
              }
            /* p++; */
          }
        RetVal = MagickPass;
        break;
      }

    case 24:     /*  Convert DirectColor scanline.  */
      RetVal = ImportImagePixelArea(image,RGBQuantum,8,p,NULL,0);
      break;

    default:
      if (image->logging)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                              "Unsupported bits per pixel %u",bpp);

      return MagickFail;  /* emit some error here */
    }


  if(RetVal==MagickFail)
  {
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),"ImportImagePixelArea failed for row: %lu, bpp: %d", y, bpp);
    return MagickFail;
  }

  if(!SyncImagePixels(image))
  {
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),"SyncImagePixels failed for row: %ld, bpp: %d", y, bpp);
    return MagickFail;
  }

return RetVal;
}


/* Helper for WPG1 raster reader. */
#define InsertByte(b) \
{ \
  BImgBuff[x]=b; \
  x++; \
  if((long) x>=ldblk) \
  { \
    if(InsertRow(BImgBuff,y,image,bpp)==MagickFail) { RetVal=-6; goto unpack_wpg_raser_error; } \
    x=0; \
    y++; \
    if(y>=image->rows) break; \
    } \
}


/** Call this function to ensure that all data matrix is filled with something. This function
 * is used only to error recovery. */
static MagickPassFail ZeroFillMissingData(unsigned char *BImgBuff,unsigned long x, unsigned long y, Image *image,
                                          int bpp, long ldblk)
{
  MagickPassFail
    status = MagickPass;

  while(y<image->rows && image->exception.severity!=UndefinedException)
  {
    if((long) x<ldblk) 
    {
      memset(BImgBuff+x, 0, (size_t)ldblk-(size_t)x);
      if(x == 0)
        x = ldblk;	/* Do not memset any more */
      else
        x = 0;		/* Next pass will need to clear whole row */
    }
    if(InsertRow(BImgBuff,y,image,bpp) == MagickFail)
      {
        status = MagickFail;
        break;
      }
    y++;
  }
  return status;
}


/* WPG1 raster reader.
 * @return      0 - OK; -2 - alocation failure; -3 unaligned column; -4 - image row overflowl
                -5 - blob read error; -6 - row insert problem  */
static int UnpackWPGRaster(Image *image,int bpp)
{
  unsigned long
    x,
    y;

  int
    i;
  int RetVal = 0;

  unsigned char
    bbuf,
    *BImgBuff,
    RunCount;

  long
    ldblk;

  x=0;
  y=0;

  ldblk = (long)((bpp*image->columns+7)/8);
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                        "Raster allocation size: %ld byte%s",
                        ldblk, (ldblk > 1 ? "s" : ""));
  BImgBuff=MagickAllocateResourceLimitedMemory(unsigned char *,(size_t) ldblk);
  if(BImgBuff==NULL) return(-2);
  (void) memset(BImgBuff,0,(size_t) ldblk);

  while(y<image->rows)
    {
      i = ReadBlobByte(image);
      if(i==EOF)
        {
          ZeroFillMissingData(BImgBuff,x,y,image,bpp,ldblk);
          MagickFreeResourceLimitedMemory(BImgBuff);
          return(-5);
        }
      bbuf = i;

      RunCount=bbuf & 0x7F;
      if(bbuf & 0x80)
        {
          if(RunCount)  /* repeat next byte runcount * */
            {
              bbuf=ReadBlobByte(image);
              for(i=0;i<(int) RunCount;i++) InsertByte(bbuf);
            }
          else {  /* read next byte as RunCount; repeat 0xFF runcount* */
            RunCount=ReadBlobByte(image);
            for(i=0;i<(int) RunCount;i++) InsertByte(0xFF);
          }
        }
      else {
        if(RunCount)   /* next runcount byte are readed directly */
          {
            for(i=0;i < (int) RunCount;i++)
              {
                bbuf=ReadBlobByte(image);
                InsertByte(bbuf);
              }
          }
        else {  /* repeat previous line runcount* */
          i = ReadBlobByte(image);
          if(i==EOF)
          {
            ZeroFillMissingData(BImgBuff,x,y,image,bpp,ldblk);
            MagickFreeResourceLimitedMemory(BImgBuff);
            return -7;
          }
          RunCount = i;
          if(x!=0) {    /* attempt to duplicate row from x position: */
                        /* I do not know what to do here */
            if (InsertRow(BImgBuff,y,image,bpp) == MagickPass)   /* May be line flush can fix a situation. */
              {
                x=0;
                y++;
                ZeroFillMissingData(BImgBuff,x,y,image,bpp,ldblk);
              }
            MagickFreeResourceLimitedMemory(BImgBuff);
            return(-3);
          }
          for(i=0; i<(int)RunCount; i++)
            {           /* Here I need to duplicate previous row RUNCOUNT* */
                        /* when x=0; y points to a new empty line. For y=0 zero line will be populated. */
              if(y>=image->rows)
                {                  
                  MagickFreeResourceLimitedMemory(BImgBuff);
                  return(-4);
                }
              if(InsertRow(BImgBuff,y,image,bpp)==MagickFail)
                { 
                  MagickFreeResourceLimitedMemory(BImgBuff);
                  return(-6);
                }
              y++;
            }
        }
      }
    }
unpack_wpg_raser_error:;
  MagickFreeResourceLimitedMemory(BImgBuff);
  return(RetVal);
}


/* Helper for WPG2 reader. */
#define InsertByte6(b) \
{ \
  if(XorMe)\
    BImgBuff[x] = b ^ UpImgBuff[x];\
  else\
    BImgBuff[x] = b;\
  x++; \
  if((long) x >= ldblk) \
  { \
    if(InsertRow(BImgBuff,(long) y,image,bpp)==MagickFail) { RetVal=-6; goto unpack_wpg2_error; } \
    x=0; \
    y++; \
    XorMe = 0; \
    tmpImgBuff = BImgBuff; \
    BImgBuff = UpImgBuff; \
    UpImgBuff = tmpImgBuff; \
   } \
}

#define FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff) \
  do \
    { \
      MagickFreeResourceLimitedMemory(BImgBuff); \
      MagickFreeResourceLimitedMemory(UpImgBuff); \
    } while(0);

/* WPG2 raster reader. */
static int UnpackWPG2Raster(Image *image, int bpp)
{
  unsigned int
    SampleSize=1;

  unsigned char
    bbuf,
    *BImgBuff = (unsigned char *) NULL,   /* Buffer for a current line. */
    *UpImgBuff = (unsigned char *) NULL,  /* Buffer for previous line. */
    *tmpImgBuff = (unsigned char *) NULL,
    RunCount,
    SampleBuffer[8];

  unsigned long
    x,
    y;

  unsigned int
    i;

  long
    ldblk;

  int XorMe = 0;
  int c;
  int RetVal = 0;

  x=0;
  y=0;
  ldblk=(long) ((bpp*image->columns+7)/8);
  BImgBuff=MagickAllocateResourceLimitedMemory(unsigned char *,(size_t) ldblk);
  if(BImgBuff==NULL)
    return(-2);
  UpImgBuff=MagickAllocateResourceLimitedMemory(unsigned char *,(size_t) ldblk);
  if(UpImgBuff==NULL)
  {
    FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
    return(-2);
  }
  (void) memset(UpImgBuff,0,ldblk);
  (void) memset(SampleBuffer,0,sizeof(SampleBuffer));

  while( y< image->rows)
    {
      bbuf=ReadBlobByte(image);

      switch(bbuf)
        {
        case 0x7D:
          if ((c = ReadBlobByte(image)) == EOF) /* DSZ */
            {
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-4);
            }
          SampleSize=c;
          if(SampleSize>8)
            {
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-2);
            }
          if(SampleSize<1)
            {
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-2);
            }
          break;
        case 0x7E:
          if(y==0)                         /* XOR */
            (void)fprintf(stderr,"\nWPG token XOR on the first line is not supported, please report!");
          XorMe=!XorMe; /* or XorMe=1 ?? */
          break;
        case 0x7F:
          if ((c = ReadBlobByte(image)) == EOF) /* BLK */
            {
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-4);
            }
          RunCount=c;
          for(i=0; i < SampleSize*(RunCount+1); i++)
            {
              InsertByte6(0);
            }
          break;
        case 0xFD:
          if ((c = ReadBlobByte(image)) == EOF) /* EXT */
            {
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-4);
            }
          RunCount=c;
          for(i=0; i<= RunCount;i++)
            for(bbuf=0; bbuf < SampleSize; bbuf++)
              InsertByte6(SampleBuffer[bbuf]);
          break;
        case 0xFE:
          if ((c = ReadBlobByte(image)) == EOF)  /* RST */
            {
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-4);
            }
          RunCount = c;
          if(x!=0)
            {
              (void) fprintf(stderr,
                             "\nUnsupported WPG2 unaligned token RST x=%lu, please report!\n"
                             ,x);
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-3);
            }
          {
            /* duplicate the previous row RunCount x */
            for(i=0;i<=RunCount;i++)
              {
                if(InsertRow(UpImgBuff,(long)((image->rows>y) ? y : image->rows-1),image,bpp) == MagickFail)
                  {
                    FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
                    return(-4);
                  }
                y++;
              }
          }
          break;
        case 0xFF:
          if ((c = ReadBlobByte(image)) == EOF)  /* WHT */
            {
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-4);
            }
          RunCount=c;
          for(i=0; i < SampleSize*(RunCount+1); i++)
            {
              InsertByte6(0xFF);
            }
          break;
        default:
          RunCount=bbuf & 0x7F;

          if(bbuf & 0x80)                /* REP */
            {
              for(i=0; i < SampleSize; i++)
                SampleBuffer[i]=ReadBlobByte(image);
              for(i=0;i<=RunCount;i++)
                for(bbuf=0;bbuf<SampleSize;bbuf++)
                  InsertByte6(SampleBuffer[bbuf]);
            }
          else {                        /* NRP */
            for(i=0; i< SampleSize*(RunCount+1);i++)
              {
                bbuf=ReadBlobByte(image);
                InsertByte6(bbuf);
              }
          }
          if (EOFBlob(image))
            {
              FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
              return(-4);
            }
        }
    }
unpack_wpg2_error:;
  FreeUnpackWPG2RasterAllocs(BImgBuff,UpImgBuff);
  return(RetVal);
}


typedef float tCTM[3][3];

static unsigned LoadWPG2Flags(Image *image,char Precision,float *Angle,tCTM *CTM)
{
const unsigned char TPR=1,TRN=2,SKW=4,SCL=8,ROT=0x10,OID=0x20,LCK=0x80;
long x;
unsigned DenX;
unsigned Flags;

 (void) memset(*CTM,0,sizeof(*CTM));     /*CTM.erase();CTM.resize(3,3);*/
 (*CTM)[0][0]=1;
 (*CTM)[1][1]=1;
 (*CTM)[2][2]=1;

 Flags=ReadBlobLSBShort(image);
 if(Flags & LCK) /*x=*/ (void) ReadBlobLSBLong(image);  /*Edit lock*/
 if(Flags & OID)
        {
        if(Precision==0)
          {/*x=*/ (void) ReadBlobLSBShort(image);}      /*ObjectID*/
        else
          {/*x=*/ (void) ReadBlobLSBLong(image);}       /*ObjectID (Double precision)*/
        }
 if(Flags & ROT)
        {
        x=ReadBlobLSBLong(image);       /*Rot Angle*/
        if(Angle) *Angle=(float) (x/65536.0);
        }
 if(Flags & (ROT|SCL))
        {
        x=ReadBlobLSBLong(image);       /*Sx*cos()*/
        (*CTM)[0][0] = (float)x/0x10000;
        x=ReadBlobLSBLong(image);       /*Sy*cos()*/
        (*CTM)[1][1] = (float)x/0x10000;
        }
 if(Flags & (ROT|SKW))
        {
        x=ReadBlobLSBLong(image);       /*Kx*sin()*/
        (*CTM)[1][0] = (float)x/0x10000;
        x=ReadBlobLSBLong(image);       /*Ky*sin()*/
        (*CTM)[0][1] = (float)x/0x10000;
        }
 if(Flags & TRN)
        {
        x=ReadBlobLSBLong(image); DenX=ReadBlobLSBShort(image);  /*Tx*/
        if(x>=0) (*CTM)[0][2] = (float)x+(float)DenX/0x10000;
            else (*CTM)[0][2] = (float)x-(float)DenX/0x10000;
        x=ReadBlobLSBLong(image); DenX=ReadBlobLSBShort(image);  /*Ty*/
        (*CTM)[1][2]=(float)x + ((x>=0)?1:-1)*(float)DenX/0x10000;
        if(x>=0) (*CTM)[1][2] = (float)x+(float)DenX/0x10000;
            else (*CTM)[1][2] = (float)x-(float)DenX/0x10000;
        }
 if(Flags & TPR)
        {
        x=ReadBlobLSBShort(image); DenX=ReadBlobLSBShort(image);  /*Px*/
        (*CTM)[2][0] = x + (float)DenX/0x10000;;
        x=ReadBlobLSBShort(image);  DenX=ReadBlobLSBShort(image); /*Py*/
        (*CTM)[2][1] = x + (float)DenX/0x10000;
        }
 return(Flags);
}


static Image *ExtractPostscript(Image *image,const ImageInfo *image_info,
                                ExtendedSignedIntegralType PS_Offset,
                                size_t PS_Size,ExceptionInfo *exception)
{
  ImageInfo
    *clone_info;

  Image
    *image2;

  unsigned char
    header_magick[2*MaxTextExtent];

  void
    *ps_data,
    *ps_data_alloc = (unsigned char *) NULL;

  char
    format[MaxTextExtent];

  size_t
    header_magick_size;

  magick_off_t
    filesize;

  if (image->logging)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                          "ExtractPostscript(): PS_Offset=%"MAGICK_OFF_F"d, PS_Size=%"MAGICK_SIZE_T_F"u",
                          (magick_off_t) PS_Offset, (MAGICK_SIZE_T) PS_Size);

  /*
    Validate that claimed subordinate image data is contained in file size
  */
  filesize = GetBlobSize(image);
  if ((PS_Offset > filesize) || ((size_t) (filesize - PS_Offset) < PS_Size))
    {
      if (image->logging)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                              "ExtractPostscript(): Failed to seek to PS_Offset=%"MAGICK_OFF_F"d",
                              (magick_off_t) PS_Offset);
      ThrowException(exception,CorruptImageError,UnexpectedEndOfFile,image->filename);
      return image;
    }

  /*
    Get subordinate file header magick and use it to identify file format
  */
  if (SeekBlob(image,PS_Offset,SEEK_SET) != PS_Offset)
    {
      if (image->logging)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                              "ExtractPostscript(): Failed to seek to PS_Offset=%"MAGICK_OFF_F"d",
                              (magick_off_t) PS_Offset);
      ThrowException(exception,CorruptImageError,UnexpectedEndOfFile,image->filename);
      return image;
    }
  header_magick_size = ReadBlob(image, Min(sizeof(header_magick),PS_Size), header_magick);
  format[0]='\0';
  /*
    MagickExport MagickPassFail
    GetMagickFileFormat(const unsigned char *header, const size_t header_length,
      char *format, const size_t format_length,
      ExceptionInfo *exception)
  */
  if (GetMagickFileFormat(header_magick,header_magick_size,format,
                          sizeof(format),exception) == MagickFail)
    {
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                            "Failed to identify embedded file type!");
      ThrowException(exception,CorruptImageError,UnableToReadImageHeader,image->filename);
      return image;
    }

  /*
    Verify if this is an allowed subordinate image format
  */
  if(!ApproveFormatForWPG(format))
  {
    (void) LogMagickEvent(CoderEvent, GetMagickModule(),
                        "Format \"%s\" cannot be embedded inside WPG.", format);
    ThrowException(exception,CorruptImageError,UnableToReadImageHeader,image->filename);
    return image;
  }

  /*
    Restore seek offset after reading header
  */
  if (SeekBlob(image,PS_Offset,SEEK_SET) != PS_Offset)
    {
      if (image->logging)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                              "ExtractPostscript(): Failed to seek to PS_Offset=%"MAGICK_OFF_F"d",
                              (magick_off_t) PS_Offset);
      ThrowException(exception,CorruptImageError,UnexpectedEndOfFile,image->filename);
      return image;
    }
  /*
    Allocate buffer if zero-copy read is not possible.
  */
  if (GetBlobStreamData(image) == (unsigned char *) NULL)
    {
      ps_data_alloc=MagickAllocateResourceLimitedMemory(unsigned char *, PS_Size);
      if (ps_data_alloc == (unsigned char *) NULL)
        {
          if (image->logging)
            (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                                  "ExtractPostscript(): Failed to allocate "
                                  "%"MAGICK_SIZE_T_F"u bytes of memory",
                                  (MAGICK_SIZE_T) PS_Size);
          ThrowException(exception,ResourceLimitError,MemoryAllocationFailed,image->filename);
          return image;
        }
    }
  /*
    Use a zero-copy read when possible to access data
  */
  ps_data=ps_data_alloc;
  if (ReadBlobZC(image,PS_Size,&ps_data) != PS_Size)
    {
      MagickFreeResourceLimitedMemory(ps_data_alloc);
      if (image->logging)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                              "ExtractPostscript(): Failed to read %"MAGICK_SIZE_T_F"u bytes of data at"
                              " offset=%"MAGICK_OFF_F"d",
                              (MAGICK_SIZE_T) PS_Size, (magick_off_t) PS_Offset);
      ThrowException(exception,CorruptImageError,UnexpectedEndOfFile,image->filename);
      return image; /* return (Image *) NULL; */
    }
  if (ps_data_alloc != ps_data)
    {
      if (image->logging)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                              "ExtractPostscript(): Zero copy read.");
    }
#if 0
  /*
    Write in-memory blob to file for test purposes.
  */
  {
    char file_name[MaxTextExtent];
    FILE *file;

    FormatString(file_name,"wpg-blob.%s",format);
    if ((file=fopen(file_name,"w")) != (FILE *) NULL)
      {
        if (image->logging)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                                "Writing %s...", file_name);
        (void) fwrite(ps_data, 1, PS_Size, file);
        (void) fclose(file);
      }
    SeekBlob(image,PS_Offset,SEEK_SET);
  }
#endif
  /*
    Read nested image from blob, forcing read as Postscript format
  */
  if ((clone_info=CloneImageInfo(image_info)) == NULL)
    {
      MagickFreeResourceLimitedMemory(ps_data_alloc);
      return(image);
    }
  clone_info->blob=(void *) NULL;
  /* clone_info->length=0; */
  (void) strlcpy(clone_info->magick, format, sizeof(clone_info->magick));
  (void) strcpy(clone_info->filename, "");
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                        "Reading embedded \"%s\" content from blob...", clone_info->magick);
  image2 = BlobToImage(clone_info, ps_data, PS_Size, &image->exception );
  MagickFreeResourceLimitedMemory(ps_data_alloc);
  if (!image2)
    {
      goto FINISH_UNL;
    }
  if(exception->severity>=ErrorException) /* When exception is raised, destroy image2 read. */
  {
    CloseBlob(image2);
    DestroyImageList(image2);
    goto FINISH_UNL;
  }

  /*
    Replace current image with new image while copying base image
    attributes.
  */
  {
    Image *p;
    p = image2;
    do
    {
      (void) strlcpy(p->filename,image->filename,MaxTextExtent);
      (void) strlcpy(p->magick_filename,image->magick_filename,MaxTextExtent);
      (void) strlcpy(p->magick,image->magick,MaxTextExtent);
      /*image2->depth=image->depth;*/   /* !!!! The image2 depth should not be modified here. Image2 is completely different. */
      DestroyBlob(p);

      if(p->rows==0 || p->columns==0)
      {
        DeleteImageFromList(&p);
        if(p==NULL)
        {
          image2 = NULL;
          goto FINISH_UNL;      /* Nothing to add, skip. */
        }
      }
      else
      {
        p->blob = ReferenceBlob(image->blob);
        p = p->next;
      }
    } while(p!=NULL);
  }

  if((image->rows==0 || image->columns==0) && (image->previous!=NULL || image->next!=NULL))
  {
    DeleteImageFromList(&image);
  }

  AppendImageToList(&image,image2);     /* This should append list 'image2' to the list 'image', image2 accepts NULL. */
  while(image->next != NULL)
    image = image->next;                /* Rewind the cursor to the end. */

 FINISH_UNL:
  DestroyImageInfo(clone_info);
  return(image);
}


/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d W P G I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method ReadWPGImage reads an WPG X image file and returns it.  It
%  allocates the memory necessary for the new Image structure and returns a
%  pointer to the new image.
%
%  The format of the ReadWPGImage method is:
%
%    Image *ReadWPGImage(const ImageInfo *image_info,ExceptionInfo *exception)
%
%  A description of each parameter follows:
%
%    o image:  Method ReadWPGImage returns a pointer to the image after
%      reading. A null image is returned if there is a memory shortage or if
%      the image cannot be read.
%
%    o image_info: Specifies a pointer to a ImageInfo structure.
%
%    o exception: return any errors or warnings in this structure.
%
%
*/
static Image *ReadWPGImage(const ImageInfo *image_info,
  ExceptionInfo *exception)
{
  typedef struct
  {
    unsigned long FileId;
    ExtendedSignedIntegralType DataOffset;  /* magick_uint32_t */
    unsigned int ProductType;
    unsigned int FileType;
    unsigned char MajorVersion;
    unsigned char MinorVersion;
    unsigned int EncryptKey;
    unsigned int Reserved;
  } WPGHeader;

  typedef struct
  {
    unsigned char RecType;
    unsigned long RecordLength;
  } WPGRecord;

  typedef struct
  {
    unsigned char Class;
    unsigned char RecType;
    unsigned long Extension;
    unsigned long RecordLength;
  } WPG2Record;

  typedef struct
  {
    unsigned    HorizontalUnits;
    unsigned    VerticalUnits;
    unsigned char PosSizePrecision;
  } WPG2Start;

  typedef struct
  {
    unsigned int Width;
    unsigned int Heigth;
    unsigned int Depth;
    unsigned int HorzRes;
    unsigned int VertRes;
  } WPGBitmapType1;

  typedef struct
  {
    unsigned int Width;
    unsigned int Heigth;
    unsigned char Depth;
    unsigned char Compression;
  } WPG2BitmapType1;

  typedef struct
  {
    unsigned int RotAngle;
    unsigned int LowLeftX;
    unsigned int LowLeftY;
    unsigned int UpRightX;
    unsigned int UpRightY;
    unsigned int Width;
    unsigned int Heigth;
    unsigned int Depth;
    unsigned int HorzRes;
    unsigned int VertRes;
  } WPGBitmapType2;

  typedef struct
  {
    unsigned int StartIndex;
    unsigned int NumOfEntries;
  } WPGColorMapRec;

  /*
  typedef struct {
    unsigned long PS_unknown1;
    unsigned int PS_unknown2;
    unsigned int PS_unknown3;
  } WPGPSl1Record;
  */

  Image
    *image,
    *rotated_image;

  unsigned int
    status;

  WPGHeader
    Header;

  WPGRecord
    Rec;

  WPG2Record
    Rec2;

  WPG2Start StartWPG;

  WPGBitmapType1
    BitmapHeader1;

  WPG2BitmapType1
    Bitmap2Header1;

  WPGBitmapType2
    BitmapHeader2;

  WPGColorMapRec
    WPG_Palette;

  int
    i,
    bpp;

  int logging;

  long
    ldblk;

  unsigned char
    *BImgBuff;
  BlobInfo *TmpBlob;
  magick_off_t FilePos, filesize;

  tCTM CTM;         /*current transform matrix*/

  /*
    Open image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickSignature);

  logging = LogMagickEvent(CoderEvent,GetMagickModule(),"enter");

  image=AllocateImage(image_info);
  image->depth=8;
  status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
  if (status == False)
    ThrowReaderException(FileOpenError,UnableToOpenFile,image);

  /*
    Read WPG image.
  */
  Header.FileId=ReadBlobLSBLong(image);
  Header.DataOffset=(ExtendedSignedIntegralType) ReadBlobLSBLong(image);
  Header.ProductType=ReadBlobLSBShort(image);
  Header.FileType=ReadBlobLSBShort(image);
  Header.MajorVersion=ReadBlobByte(image);
  Header.MinorVersion=ReadBlobByte(image);
  Header.EncryptKey=ReadBlobLSBShort(image);
  Header.Reserved=ReadBlobLSBShort(image);

  if (Header.FileId!=0x435057FF || (Header.ProductType>>8)!=0x16)
    ThrowReaderException(CorruptImageError,ImproperImageHeader,image);
  if (Header.EncryptKey!=0)
    ThrowReaderException(CoderError,EncryptedWPGImageFileNotSupported,image);

  image->colors = 0;
  image->storage_class = DirectClass;
  bpp=0;

  if (logging) (void)LogMagickEvent(CoderEvent,GetMagickModule(),
          "File type: %d", Header.FileType);

  /* Determine file size. */
  filesize = GetBlobSize(image);              /* zero is returned if the size cannot be determined. */
  if(filesize>0 && BlobIsSeekable(image))
  {
    if(filesize > (magick_off_t)0xFFFFFFFF)
        filesize = (magick_off_t)0xFFFFFFFF;  /* More than 4GiB are not supported in WPG! */
  }
  else
  {
    filesize = (magick_off_t)0xFFFFFFFF;
    if (logging) (void)LogMagickEvent(CoderEvent,GetMagickModule(),
          "Blob is not seekable, WPG reader could fail.");
    ThrowReaderException(CorruptImageError,AnErrorHasOccurredReadingFromFile,image);
  }

  switch(Header.FileType)
    {
    case 1:     /* WPG level 1 */
      BitmapHeader2.RotAngle = 0;
      FilePos = Header.DataOffset;
      while(!EOFBlob(image)) /* object parser loop */
        {
          if(SeekBlob(image,FilePos,SEEK_SET) != FilePos)
            break;

          Rec.RecType = (i=ReadBlobByte(image));
          if(i==EOF) break;
          FilePos += 1;

          FilePos += Rd_WP_DWORD(image,&Rec.RecordLength);
          if((magick_off_t)Rec.RecordLength > filesize)
            ThrowReaderException(CorruptImageError,ImproperImageHeader,image);
          if(EOFBlob(image)) break;

          FilePos += (magick_off_t)Rec.RecordLength;
          if(FilePos>filesize || FilePos<Header.DataOffset)
          {
            if (logging) (void)LogMagickEvent(CoderEvent,GetMagickModule(),
                "Invalid record length: %X", (unsigned)Rec.RecType);
            break;
          }
          Header.DataOffset = FilePos;

          if (logging) (void)LogMagickEvent(CoderEvent,GetMagickModule(),
            "Parsing object: %X", Rec.RecType);
          /* printf("\nParsing object: %u:%X", (unsigned)FilePos, Rec.RecType); */

          switch(Rec.RecType)
            {
            case 0x0B: /* bitmap type 1 */
              BitmapHeader1.Width=ReadBlobLSBShort(image);
              BitmapHeader1.Heigth=ReadBlobLSBShort(image);
              if ((BitmapHeader1.Width == 0) || (BitmapHeader1.Heigth == 0))
                ThrowReaderException(CorruptImageError,ImproperImageHeader,image);
              BitmapHeader1.Depth=ReadBlobLSBShort(image);
              BitmapHeader1.HorzRes=ReadBlobLSBShort(image);
              BitmapHeader1.VertRes=ReadBlobLSBShort(image);

              if(BitmapHeader1.HorzRes && BitmapHeader1.VertRes)
                {
                  image->units=PixelsPerCentimeterResolution;
                  image->x_resolution=BitmapHeader1.HorzRes/470.0;
                  image->y_resolution=BitmapHeader1.VertRes/470.0;
                }
              image->columns=BitmapHeader1.Width;
              image->rows=BitmapHeader1.Heigth;
              bpp=BitmapHeader1.Depth;

              goto UnpackRaster;

            case 0x0E:  /*Color palette */
              WPG_Palette.StartIndex=ReadBlobLSBShort(image);
              WPG_Palette.NumOfEntries=ReadBlobLSBShort(image);

              image->colors=WPG_Palette.NumOfEntries;
              if (!AllocateImageColormap(image,image->colors))
                goto NoMemory;
              image->storage_class = PseudoClass;
              for (i=WPG_Palette.StartIndex;
                   i < (int)WPG_Palette.NumOfEntries; i++)
                {
                  image->colormap[i].red=ScaleCharToQuantum(ReadBlobByte(image));
                  image->colormap[i].green=ScaleCharToQuantum(ReadBlobByte(image));
                  image->colormap[i].blue=ScaleCharToQuantum(ReadBlobByte(image));
                  image->colormap[i].opacity = OpaqueOpacity;
                }
              break;

            case 0x11:  /* Start PS l1 */
              if(Rec.RecordLength > 8)
                image=ExtractPostscript(image,image_info,
                                        TellBlob(image)+8,   /* skip PS header in the wpg */
                                        (size_t) (Rec.RecordLength-8),exception);
              break;

            case 0x14:  /* bitmap type 2 */
              BitmapHeader2.RotAngle=ReadBlobLSBShort(image);
              BitmapHeader2.LowLeftX=ReadBlobLSBShort(image);
              BitmapHeader2.LowLeftY=ReadBlobLSBShort(image);
              BitmapHeader2.UpRightX=ReadBlobLSBShort(image);
              BitmapHeader2.UpRightY=ReadBlobLSBShort(image);
              BitmapHeader2.Width=ReadBlobLSBShort(image);
              BitmapHeader2.Heigth=ReadBlobLSBShort(image);
              if ((BitmapHeader2.Width == 0) || (BitmapHeader2.Heigth == 0))
                ThrowReaderException(CorruptImageError,ImproperImageHeader,image);
              BitmapHeader2.Depth=ReadBlobLSBShort(image);
              BitmapHeader2.HorzRes=ReadBlobLSBShort(image);
              BitmapHeader2.VertRes=ReadBlobLSBShort(image);

              image->units=PixelsPerCentimeterResolution;
              image->page.width=(unsigned int)
                ((BitmapHeader2.LowLeftX-BitmapHeader2.UpRightX)/470.0);
              image->page.height=(unsigned int)
                ((BitmapHeader2.LowLeftX-BitmapHeader2.UpRightY)/470.0);
              image->page.x=(int) (BitmapHeader2.LowLeftX/470.0);
              image->page.y=(int) (BitmapHeader2.LowLeftX/470.0);
              if(BitmapHeader2.HorzRes && BitmapHeader2.VertRes)
                {
                  image->x_resolution=BitmapHeader2.HorzRes/470.0;
                  image->y_resolution=BitmapHeader2.VertRes/470.0;
                }
              image->columns=BitmapHeader2.Width;
              image->rows=BitmapHeader2.Heigth;
              bpp=BitmapHeader2.Depth;

UnpackRaster:
              if(bpp>24)
                {ThrowReaderException(CoderError,ColorTypeNotSupported,image)}

              if ((image->storage_class != PseudoClass) && (bpp != 24))
                {
                  image->colors=1 << bpp;
                  if (!AllocateImageColormap(image,image->colors))
                    {
                    NoMemory:
                      ThrowReaderException(ResourceLimitError,MemoryAllocationFailed,
                                           image)
                    }
                  image->storage_class = PseudoClass;
                  /* printf("Load default colormap \n"); */
                  for (i=0; (i < (int) image->colors) && (i < 256); i++)
                    {
                      image->colormap[i].red=ScaleCharToQuantum(WPG1_Palette[i].Red);
                      image->colormap[i].green=ScaleCharToQuantum(WPG1_Palette[i].Green);
                      image->colormap[i].blue=ScaleCharToQuantum(WPG1_Palette[i].Blue);
                      image->colormap[i].opacity = OpaqueOpacity;
                    }
                }
              else
                {
                  if (bpp < 24)
                    if ( (image->colors < (1UL<<bpp)) && (bpp != 24) )
                      if (!ReallocColormap(image,1U<<bpp))
                        goto NoMemory;
                }

              if(bpp == 1)
                {
                  if(image->colors<=0)
                                  {
                                image->colormap[0].red =
                        image->colormap[0].green =
                        image->colormap[0].blue = 0;
                      image->colormap[0].opacity = OpaqueOpacity;
                                  }
                  if(image->colors<=1 ||        /* Realloc has been enforced and value [1] remains uninitialised, or .. */
                                           (image->colormap[0].red==0 && image->colormap[0].green==0 && image->colormap[0].blue==0 &&
                        image->colormap[1].red==0 && image->colormap[1].green==0 && image->colormap[1].blue==0))
                    {  /* fix crippled monochrome palette */
                      image->colormap[1].red =
                        image->colormap[1].green =
                        image->colormap[1].blue = MaxRGB;
                      image->colormap[1].opacity = OpaqueOpacity;
                    }
                }

              if(!image_info->ping)
                if(UnpackWPGRaster(image,bpp) < 0)
                {               /* The raster cannot be unpacked */
                DecompressionFailed:
                  ThrowReaderException(CoderError,UnableToDecompressImage,image)
                }

              if(Rec.RecType==0x14 && BitmapHeader2.RotAngle!=0 && !image_info->ping)
                {
                  /* flop command */
                  if(BitmapHeader2.RotAngle & 0x8000)
                    {
                      rotated_image = FlopImage(image, exception);
                      if (rotated_image != (Image *)NULL)
                        {
                          TmpBlob = rotated_image->blob;
                          rotated_image->blob = image->blob;
                          image->blob = TmpBlob;
                          ReplaceImageInList(&image,rotated_image);
                        }
                    }
                  /* flip command */
                  if(BitmapHeader2.RotAngle & 0x2000)
                    {
                      rotated_image = FlipImage(image, exception);
                      if (rotated_image != (Image *) NULL)
                        {
                          TmpBlob = rotated_image->blob;
                          rotated_image->blob = image->blob;
                          image->blob = TmpBlob;
                          ReplaceImageInList(&image,rotated_image);
                        }
                    }

                  /* rotate command */
                  if(BitmapHeader2.RotAngle & 0x0FFF)
                    {
                      rotated_image = RotateImage(image,
                                                  (BitmapHeader2.RotAngle & 0x0FFF),
                                                  exception);
                      if (rotated_image != (Image *) NULL)
                        {
                          TmpBlob = rotated_image->blob;
                          rotated_image->blob = image->blob;
                          image->blob = TmpBlob;
                          ReplaceImageInList(&image,rotated_image);
                        }
                    }
                }

              StopTimer(&image->timer);

              if (image_info->subrange != 0)
                if (image->scene >= (image_info->subimage+image_info->subrange-1))
                  goto Finish;

              /* Allocate next image structure. */
              AllocateNextImage(image_info,image);
              image->depth=8;
              if (image->next == (Image *) NULL)
                goto Finish;
              image=SyncNextImageInList(image);
              image->columns=image->rows=0;
              image->colors=0;
              break;

            case 0x1B:  /* Postscript l2 */
              if(Rec.RecordLength>0x3C)
                image=ExtractPostscript(image,image_info,
                                        TellBlob(image)+0x3C,   /* skip PS l2 header in the wpg */
                                        (size_t) (Rec.RecordLength-0x3C),exception);
              break;
            }
        }
      break;

    case 2:  /* WPG level 2 */
      (void) memset(CTM,0,sizeof(CTM));
      StartWPG.PosSizePrecision = 0;
      while(!EOFBlob(image)) /* object parser loop */
        {
          if(SeekBlob(image,Header.DataOffset,SEEK_SET) != Header.DataOffset)
            break;
          if(EOFBlob(image))
            break;

          Rec2.Class=(i=ReadBlobByte(image));
          if(i==EOF)
            break;
          Rec2.RecType=(i=ReadBlobByte(image));
          if(i==EOF)
            break;
          Rd_WP_DWORD(image,&Rec2.Extension);
          Rd_WP_DWORD(image,&Rec2.RecordLength);
          if(EOFBlob(image))
            break;

          Header.DataOffset=TellBlob(image)+Rec2.RecordLength;

          if(logging) (void)LogMagickEvent(CoderEvent,GetMagickModule(),
            "Parsing object: %X", Rec2.RecType);

          switch(Rec2.RecType)
            {
            case 1:
              StartWPG.HorizontalUnits=ReadBlobLSBShort(image);
              StartWPG.VerticalUnits=ReadBlobLSBShort(image);
              StartWPG.PosSizePrecision=ReadBlobByte(image);
              break;
            case 0x0C:    /* Color palette */
              WPG_Palette.StartIndex=ReadBlobLSBShort(image);
              WPG_Palette.NumOfEntries=ReadBlobLSBShort(image);

              /* Sanity check for amount of palette entries. */
              if (WPG_Palette.NumOfEntries == 0)
                ThrowReaderException(CorruptImageError,UnrecognizedNumberOfColors,image);

              if (WPG_Palette.NumOfEntries > MaxMap+1)
                ThrowReaderException(CorruptImageError,ColormapExceedsColorsLimit,image);

              if ( (WPG_Palette.StartIndex > WPG_Palette.NumOfEntries) ||
                   (((WPG_Palette.NumOfEntries-WPG_Palette.StartIndex) >
                     ((Rec2.RecordLength-2-2) / 3))) )
                 ThrowReaderException(CorruptImageError,InvalidColormapIndex,image);

              image->colors=WPG_Palette.NumOfEntries;
              if (!AllocateImageColormap(image,image->colors))
                ThrowReaderException(ResourceLimitError,MemoryAllocationFailed,image);

              for (i=WPG_Palette.StartIndex;
                   i < (int)WPG_Palette.NumOfEntries; i++)
                {
                  image->colormap[i].red=ScaleCharToQuantum(ReadBlobByte(image));
                  image->colormap[i].green=ScaleCharToQuantum(ReadBlobByte(image));
                  image->colormap[i].blue=ScaleCharToQuantum(ReadBlobByte(image));
                  image->colormap[i].opacity = OpaqueOpacity;
                  (void) ReadBlobByte(image);   /*Opacity??*/
                }
              break;
            case 0x0E:
              Bitmap2Header1.Width=ReadBlobLSBShort(image);
              Bitmap2Header1.Heigth=ReadBlobLSBShort(image);
              if ((Bitmap2Header1.Width == 0) || (Bitmap2Header1.Heigth == 0))
                ThrowReaderException(CorruptImageError,ImproperImageHeader,image);
              Bitmap2Header1.Depth=ReadBlobByte(image);
              Bitmap2Header1.Compression=ReadBlobByte(image);

              if(Bitmap2Header1.Compression > 1)
                continue; /*Unknown compression method */
              switch(Bitmap2Header1.Depth)
                {
                case 1: bpp=1;
                  break;
                case 2: bpp=2;
                  break;
                case 3: bpp=4;
                  break;
                case 4: bpp=8;
                  break;
                case 8: bpp=24;
                  break;
                default:
                  continue;  /*Ignore raster with unknown depth*/
                }
              image->columns=Bitmap2Header1.Width;
              image->rows=Bitmap2Header1.Heigth;

              if ((image->colors == 0) && (bpp != 24))
                {
                  image->colors=1 << bpp;
                  if (!AllocateImageColormap(image,image->colors))
                    goto NoMemory;
                  image->storage_class = PseudoClass;
                }
              else
                {
                  if(bpp < 24)
                    if( image->colors<(1UL<<bpp) && bpp!=24 )
                      if (!ReallocColormap(image,1U<<bpp))
                        goto NoMemory;
                }


              switch(Bitmap2Header1.Compression)
                {
                case 0:    /*Uncompressed raster*/
                  {
                    ldblk=(long) ((bpp*image->columns+7)/8);
                    BImgBuff=MagickAllocateResourceLimitedMemory(unsigned char *,(size_t) ldblk);
                    if(BImgBuff == (unsigned char *) NULL)
                      goto NoMemory;

                    for(i=0; i< (long) image->rows; i++)
                      {
                        if (ReadBlob(image,ldblk,(char *) BImgBuff) != (size_t) ldblk)
                          {
                            MagickFreeResourceLimitedMemory(BImgBuff);
                            ThrowException(exception,CorruptImageError,UnexpectedEndOfFile,image->filename);
                            goto DecompressionFailed;
                          }
                        if(InsertRow(BImgBuff,i,image,bpp) == MagickFail)
                        {
                          if(BImgBuff) MagickFreeResourceLimitedMemory(BImgBuff);
                          goto DecompressionFailed;
                        }
                      }

                    if(BImgBuff) MagickFreeResourceLimitedMemory(BImgBuff);
                    break;
                  }
                case 1:    /*RLE for WPG2 */
                  {
                    if( UnpackWPG2Raster(image,bpp) < 0)
                      goto DecompressionFailed;
                    break;
                  }
                }


              if(CTM[0][0]<0 && !image_info->ping)
                {               /*?? RotAngle=360-RotAngle;*/
                  rotated_image = FlopImage(image, exception);
                  if (rotated_image != (Image *) NULL)
                    {
                      TmpBlob = rotated_image->blob;
                      rotated_image->blob = image->blob;
                      image->blob = TmpBlob;
                      ReplaceImageInList(&image,rotated_image);
                    }
                  /* Try to change CTM according to Flip - I am not sure, must be checked.
                     Tx(0,0)=-1;      Tx(1,0)=0;   Tx(2,0)=0;
                     Tx(0,1)= 0;      Tx(1,1)=1;   Tx(2,1)=0;
                     Tx(0,2)=(WPG._2Rect.X_ur+WPG._2Rect.X_ll);
                     Tx(1,2)=0;   Tx(2,2)=1; */
                }
              if(CTM[1][1]<0 && !image_info->ping)
                {               /*?? RotAngle=360-RotAngle;*/
                  rotated_image = FlipImage(image, exception);
                  if (rotated_image != (Image *) NULL)
                    {
                      TmpBlob = rotated_image->blob;
                      rotated_image->blob = image->blob;
                      image->blob = TmpBlob;
                      ReplaceImageInList(&image,rotated_image);
                    }
                  /* Try to change CTM according to Flip - I am not sure, must be checked.
                     float_matrix Tx(3,3);
                     Tx(0,0)= 1;   Tx(1,0)= 0;   Tx(2,0)=0;
                     Tx(0,1)= 0;   Tx(1,1)=-1;   Tx(2,1)=0;
                     Tx(0,2)= 0;   Tx(1,2)=(WPG._2Rect.Y_ur+WPG._2Rect.Y_ll);
                     Tx(2,2)=1; */
                }

              StopTimer(&image->timer);

              if (image_info->subrange != 0)
                if (image->scene >= (image_info->subimage+image_info->subrange-1))
                  goto Finish;

              /* Allocate next image structure. */
              AllocateNextImage(image_info,image);
              image->depth=8;
              if (image->next == (Image *) NULL)
                goto Finish;
              image=SyncNextImageInList(image);
              image->columns=image->rows=0;
              image->colors=0;
              break;

            case 0x12:  /* Postscript WPG2*/
              i=ReadBlobLSBShort(image);
              if(Rec2.RecordLength > ((unsigned long) i+2))
                image=ExtractPostscript(image,image_info,
                                        TellBlob(image)+i,              /*skip PS header in the wpg2*/
                                        (size_t)Rec2.RecordLength-i-2,exception);
              break;

            case 0x1B:          /*bitmap rectangle*/
              (void) LoadWPG2Flags(image,StartWPG.PosSizePrecision,NULL,&CTM); /* WPG2Flags */
              break;
            }
        }

      break;

    default:
      {
        ThrowReaderException(CoderError,DataEncodingSchemeIsNotSupported,image);
      }
    }

 Finish:
  CloseBlob(image);

  {
    Image
      *p;

    long
      scene=0;

    /*
      Rewind list, removing any empty images while rewinding.
    */
    p=image;
    image=NULL;
    while (p != (Image *) NULL)
      {
        Image *tmp=p;
        if ((p->rows == 0) || (p->columns == 0)) {
          p=p->previous;
          DeleteImageFromList(&tmp);
        } else {
          image=p;
          p=p->previous;
        }
      }

    /*
      Fix scene numbers
    */
    for (p=image; p != (Image *) NULL; p=p->next)
      p->scene=scene++;
  }

  if (logging) (void)LogMagickEvent(CoderEvent,GetMagickModule(),"return");
  if(image==NULL)
    ThrowReaderException(CorruptImageError,ImageFileDoesNotContainAnyImageData,image);
  return(image);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e g i s t e r W P G I m a g e                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method RegisterWPGImage adds attributes for the WPG image format to
%  the list of supported formats.  The attributes include the image format
%  tag, a method to read and/or write the format, whether the format
%  supports the saving of more than one frame to the same file or blob,
%  whether the format supports native in-memory I/O, and a brief
%  description of the format.
%
%  The format of the RegisterWPGImage method is:
%
%      RegisterWPGImage(void)
%
*/
ModuleExport void RegisterWPGImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("WPG");
  entry->decoder=(DecoderHandler) ReadWPGImage;
  entry->magick=(MagickHandler) IsWPG;
  entry->description="Word Perfect Graphics";
  entry->module="WPG";
  entry->seekable_stream=True;
  entry->coder_class=UnstableCoderClass;
  (void) RegisterMagickInfo(entry);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U n r e g i s t e r W P G I m a g e                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method UnregisterWPGImage removes format registrations made by the
%  WPG module from the list of supported formats.
%
%  The format of the UnregisterWPGImage method is:
%
%      UnregisterWPGImage(void)
%
*/
ModuleExport void UnregisterWPGImage(void)
{
  (void) UnregisterMagickInfo("WPG");
}
