1.3.35 (February 23, 2020)
==========================

Special Issues:

* It has been discovered that the 'ICU' library (a perhaps 30MB C++
  library) which is now often a libxml2 dependendency causes huge
  process initialization overhead.  This is noticed as unexpected
  slowness when GraphicsMagick utilities are used to process small to
  medium sized files.  The time to initialize the 'ICU' library is
  often longer than the time that GraphicsMagick would otherwise
  require to read the input file, process the image, and write the
  output file.  If the 'ICU' dependency can not be avoided, then make
  sure to use the modules build so there is only impact for file
  formats which require libxml2.  Please lobby the 'ICU' library
  developers to change their implementation to avoid long start-up
  times due to merely linking with the library.

Security Fixes:

* GraphicsMagick is now participating in Google's oss-fuzz project due
  to the contributions and assistance of Alex Gaynor. Since February 4
  2018, 398 issues have been opened by oss-fuzz (some of which were
  benign build issues) and 11 issues remain open.
  The issues list is available at
  https://bugs.chromium.org/p/oss-fuzz/issues/list under search term
  "graphicsmagick".  Issues are available for anyone to view and
  duplicate if they have been in "Verified" status for 30 days, or if
  they have been in "New" status for 90 days.  There are too many
  fixes to list here.  Please consult the GraphicsMagick ChangeLog
  file, Mercurial repository commit log, and the oss-fuzz issues list
  for details.

Bug fixes:

* Fix broken definition of ResourceInfinity which resulted in that
  GetMagickResource() would return -1 rather than the maximum range
  value for the return type as documented. (problem added by the
  1.3.32 release).

* ModifyCache(): Re-open the pixel cache if the cache rows/columns do
  not match the owning image rows/columns.

* Fix DisplayImages() return status.  The return status was inverted.

* HISTOGRAM: Histogram once again includes the histogram as a text
  comment.  This became broken by previous security fixes.

* PICT: Fixed heap buffer overuns reported multiple sources.

* JNG: Detect when JPEG encoder has failed and throw an exception.

* MVG/DrawImage(): Performs even more parsing validations.

* Clang static analyzer fixes: A great many fixes were made based on
  problem reports by the Clang static analyzer.

* Visual Studio static analyzer fixes: A great many fixes were made
  based on problem reports by the Visual Studio 2019 static analyzer.
  Many of these may improve the robustness of 64-bit code.

New Features:

* GRADIENT/GradientImage(): Improved accuracy of gradient levels as
  well as dramaticaly improving performance.  Output PseudoClass
  images if we can.  Add support for using the image 'gravity'
  attribute as well as the "gradient:direction" definition to produce
  gradient vector directions corresponding to SouthGravity (the
  previously-existing default), NorthGravity, WestGravity,
  EastGravity, NorthWestGravity, NorthEastGravity, SouthWestGravity,
  and SouthEastGravity.

API Updates:

* InitializeMagickEx(): New function which may be used in place of
  InitializeMagick() to initialize GraphicsMagick.  This
  initialization function returns an error status value, may update a
  passed ExceptionInfo structure with error information, and provides
  an options parameter which supports simple bit-flags to tailor
  initialization.  The signal handler registrations are skipped if the
  MAGICK_OPT_NO_SIGNAL_HANDER flag is set in the options.

Feature improvements:

* Replace use of non-reentrant legacy POSIX functions with reentrant
  equivalents.

* Timing of image reads should now be very accurate.  The timer was
  sometimes not stopped as soon as it should be.

* PICT: The PICT reader is working pretty good now.  It handles all
  the PICT image files I have available to me.

Windows Delegate Updates/Additions:

* None

Build Changes:

* Visual Studio Build: Configure program now provides a checkbox to
  enable common optimizations for better performance.

Behavior Changes:

* POSIX Signals: Use the normal termination signal handler for SIGXCPU
  and SIGXFSZ so that ulimit or setrlimit(2) may be used to apply CPU
  (RLIMIT_CPU) and output file size (RLIMIT_FSIZE) limits with the
  normal cleanup, and without dumping core. Note that any output files
  currently being written may be truncated and files being written by
  external programs (e.g. Ghostscript) might be left behind unless
  they are to a temporary file assigned by GraphicsMagick.

* Some private string and integer constants were removed from the
  apparent library ABI.  Some private functions were marked static and
  removed from the apparent library ABI.  This is mentioned because
  someone is sure to notice and be concerned about it.

* The remaining private content in installed header files was moved
  into -private.h header files which are not installed.  This should
  not be cause for concern but is mentiond because someone is sure to
  notice and be concerned about it.

1.3.34 (December 24, 2019)
==========================

Special Issues:

* It has been discovered that the 'ICU' library (a perhaps 30MB C++
  library) which is now often a libxml2 dependendency causes huge
  process initialization overhead.  This is noticed as unexpected
  slowness when GraphicsMagick utilities are used to process small to
  medium sized files.  The time to initialize the 'ICU' library is
  often longer than the time that GraphicsMagick would otherwise
  require to read the input file, process the image, and write the
  output file.  If the 'ICU' dependency can not be avoided, then make
  sure to use the modules build so there is only impact for file
  formats which require libxml2.  Please lobby the 'ICU' library
  developers to change their implementation to avoid long start-up
  times due to merely linking with the library.

Security Fixes:

* GraphicsMagick is now participating in Google's oss-fuzz project due
  to the contributions and assistance of Alex Gaynor. Since February 4
  2018, 386 issues have been opened by oss-fuzz (some of which were
  benign build issues) and 376 of those issues have been resolved.
  The issues list is available at
  https://bugs.chromium.org/p/oss-fuzz/issues/list under search term
  "graphicsmagick".  Issues are available for anyone to view and
  duplicate if they have been in "Verified" status for 30 days, or if
  they have been in "New" status for 90 days.  There are too many
  fixes to list here.  Please consult the GraphicsMagick ChangeLog
  file, Mercurial repository commit log, and the oss-fuzz issues list
  for details.

Bug fixes:

* DPS: Eliminate a memory leak.

* Debug Trace: Only output text to terminate an XML format log file if
  XML format is active.

* EXIF Parser: Detect non-terminal parsing and report an error.

* EXIF Parser: Eliminate heap buffer overflows.

* HuffmanDecodeImage(): Fix heap overflow in 32-bit applications.

* MAT: Implement subimage/subrange support.

* MVG: Address non-terminal loops, excessive run-time, thrown
  assertions, divide-by-zero, heap overflow, and memory leaks.

* OpenModule(): Now properly case-insensitive, as it used to be.

* PCX: Verify that pixel region is not negative. Assure that opacity
  channel is initialized to opaqueOpacity.  Update DirectClass
  representation while PseudoClass representation is updated.  Improve
  read performance with uncompressed PCX.

* PICT: Fix heap overflow in PICT writer.

* PNG: Fix validation of raw profile length.

* PNG: Skip coalescing layers if there is only one layer.

* PNM: Fix denial of service opportunity by limiting the length of PNM
  comment text.

* WPG: Avoid Avoid dereferencing a null pointer.

* WPG: Implement subimage/subrange support.

* WPG: Improve performance when reading an embedded image.

* Wand library: In MagickClearException(), destroy any existing
  exception info before re-initializing the exception info or else
  there will be a memory leak.

* XPM: Rquire that image properties appear in the first 512 bytes of
  the XPM file header.

New Features:

* Visual Studio build supports JBIG and WebP compression in TIFF format.

API Updates:

* None

Feature improvements:

* Compliles clean using GCC 9.

Windows Delegate Updates/Additions:

* bzlib: bzip is updated to 1.0.8 release.

* jbig: jbigkit is updated to 2.1 release.

* lcms: lcms2 is updated to 2.9 release.

* libxml: libxml2 is updated to 2.9.10 release.

* png: libpng is updated to 1.6.37 release.

* tiff: libtiff is updated to 4.1.0 release.

* webp: libwebp is updated to the 1.0.3 release.

* zlib: zlib is updated to 1.2.11 release.

* TIFF: Now also supports reading JBIG-compressed TIFF, and
  reading/writing WebP-compressed TIFF.  A number of libtiff feature
  options which are now commonly enabled were disabled and are now
  enabled by default.

Build Changes:

* MinGW: Static and shared library builds were not working.  Only the
  modules build was actually working!

* Python scripts related to the build (enabled by
  --enable-maintainer-mode) are now compatible with Python 3.

* Now supports using Google gperftools tcmalloc library for the memory
  allocator.  This improves performance for certain repetitive
  work-loads and heavily-threaded algorithms.

* Configure now reports the status of zstd (FaceBook Zstandard)
  compression in its configuration summary.

* TclMagick: Address many issues mentioned by SourceForge issue #420
  "TclMagick issues and patch".

Behavior Changes:

* PNG: Post-processing to convert the image type in the PNG reader
  based on a specified magick prefix string is now disabled.  This can
  (and should) be done after the image has been returned.

* Trace Logging: The compiled-in logging default is always to stderr,
  which may be over-ridden using log.mgk as soon as it is loaded.

* Windows Build: Search registry key HKEY_CURRENT_USER as well as
  HKEY_LOCAL_MACHINE when searching for Ghostscript.  By following the
  procedure documented in SourceForge bug 615 "GhostScript
  installation check", this allows for local user installations
  without "administrator" privileges.

1.3.33 (July 20, 2019)
==========================

Special Issues:

* It has been discovered that the 'ICU' library (a perhaps 30MB C++
  library) which is now often a libxml2 dependendency causes huge
  process initialization overhead.  This is noticed as unexpected
  slowness when GraphicsMagick utilities are used to process small to
  medium sized files.  The time to initialize is often longer than the
  time to read the input file, process the image, and write the output
  file.  If the 'ICU' dependency can not be avoided, then make sure to
  use the modules build.  Please lobby the 'ICU' library developers to
  change their implementation to avoid long start-up times due to
  merely linking with the library.

Security Fixes:

* GraphicsMagick is now participating in Google's oss-fuzz project due
  to the contributions and assistance of Alex Gaynor. Since February 4
  2018, 353 issues have been opened by oss-fuzz and 338 of those
  issues have been resolved.  The issues list is available at
  https://bugs.chromium.org/p/oss-fuzz/issues/list under search term
  "graphicsmagick".  Issues are available for anyone to view and
  duplicate if they have been in "Verified" status for 30 days, or if
  they have been in "New" status for 90 days.  There are too many
  fixes to list here.  Please consult the GraphicsMagick ChangeLog
  file, Mercurial repository commit log, and the oss-fuzz issues list
  for details.

* Documentation has been added regarding security hazards due to
  commands which support a '@filename' syntax.

* MontageImages(): Fix wrong length argument to strlcat() when
  building montage directory, which could allow heap overwrite.

Bug fixes:

* PNG: Pass correct size value to strlcat() in module registration
  code.  This bug is noticed to cause problems for Apple's OS X and
  Linux Alpine with musl libc.  This fixes a regression introduced by
  the 1.3.32 release.

* Re-implement command-line utility `'@'` file inclusion support for
  `-comment`, `-draw`, `-format`, and `-label` which was removed for
  the 1.3.32 release.  The new implementation is isolated to
  command-line utility implementation code rather than being deeply
  embedded in the library and exposed in other usage contexts.  This
  fixes a regression introduced by the 1.3.32 release.

* CAPTION: The The CAPTION reader did not appear to work at all any
  more.  Now it works again, but still not very well.

* MagickXDisplayImage(): Fix heap overwrite of windows->image.name and
  windows->image.icon_name buffers.  This bug has surely existed since
  early GraphicsMagick releases.

* MagickXAnimateImages(): Fix memory leak of scene_info.pixels.

* AcquireTemporaryFileDescriptor(): Fix compilation under Cygwin. This
  fixes a regression introduced by the 1.3.32 release.

* PNG: Fix saving to palette when mage has an alpha channel but no
  color is marked as transparent.

* Compilation warnings in the Visual Studio WIN64 build due to the
  'long' type being only 32-bits have been addressed.

New Features:

* None

API Updates:

* None

Feature improvements:

* None

Windows Delegate Updates/Additions:

* None

Build Changes:

* None

Behavior Changes:

* Support for `'@'` file inclusion support for `-comment`, `-draw`,
  `-format`, and `-label` has been restored.

1.3.32 (June 15, 2019)
==========================

Special Issues:

* It has been discovered that the 'ICU' library (a perhaps 30MB C++
  library) which is now often a libxml2 dependendency causes huge
  process initialization overhead.  This is noticed as unexpected
  slowness when GraphicsMagick utilities are used to process small to
  medium sized files.  The time to initialize is often longer than the
  time to read the input file, process the image, and write the output
  file.  If the 'ICU' dependency can not be avoided, then make sure to
  use the modules build.  Please lobby the 'ICU' library developers to
  change their implementation to avoid long start-up times due to
  merely linking with the library.

Security Fixes:

* GraphicsMagick is now participating in Google's oss-fuzz project due
  to the contributions and assistance of Alex Gaynor. Since February 4
  2018, 343 issues have been opened by oss-fuzz and 331 of those
  issues have been resolved.  The issues list is available at
  https://bugs.chromium.org/p/oss-fuzz/issues/list under search term
  "graphicsmagick".  Issues are available for anyone to view and
  duplicate if they have been in "Verified" status for 30 days, or if
  they have been in "New" status for 90 days.  There are too many
  fixes to list here.  Please consult the GraphicsMagick ChangeLog
  file, Mercurial repository commit log, and the oss-fuzz issues list
  for details.

* BMP reader: Fix heap overflow in 32-bit build due to arithmetic
  overflow. Only happens if limits are changed from defaults.

* BMP reader/writer: Improve buffer-size calculations to guard against
  buffer overflows.

* DIB reader: Reject files which claim more than 8-bits per pixel but
  also claim to be colormapped.

* DIB reader/writer: Improve buffer-size calculations to guard against
  buffer overflows.

* MIFF reader: Detect end of file while reading RLE packets.

* MIFF reader: Fix heap overflow (for some files using RLE
  compression) caused by a typo in the code.

* MAT writer: Added missing error handling to avoid heap overflow.

* MNG reader: Fixed a small heap buffer overflow.

* SVG reader: Fixed a stack buffer overflow.

* TGA writer: Fix heap overflow when image rows/columns are larger
  than 65535.

* TIFF reader: Rationalize tile width/height to reject large tile
  sizes which are much larger than the image dimensions.

* TIFF reader: Apply memory resource limits to strip and tile allocations.

* WMF reader: Fixed a division by zero problem.

* XWD reader: Many heap buffer overflows and uses of uninitialized data were fixed.

* Pixel cache: Now apply resource limits to pixel nexus allocations
  using the same limits (total pixels, width, height, memory) as
  applied to the whole image since some requests are directly
  influenced by the input file.  More tests are added for arithmetic
  overflow.  Care was taken to minimize performance impact due to the
  many extra checks.

Bug fixes:

* See above note about oss-fuzz fixes.

* Fixed include order of magick/api.h vs wand/wand_symbols.h.

* WriteImage(): Eliminate use of just-freed memory in
  clone_info->magick when throwing exception due to no support for
  format.

* Magick++/lib/Magick++/Drawable.h: Fix use of clang diagnostic syntax.

* DIB: Preserve PseudoClass opaque representation if ICO mask is opaque.

* JPEG reader: Restore ability to access detailed image properties
  while in 'ping' mode.

* JPEG reader: Base test for "Unreasonable dimensions" on original
  JPEG dimensions and not the scaled dimensions.

* JPEG reader: Allow input files to have a compression ratio as high
  as 2500.  Extremely compressed files were being rejected.

* FreeType renderer: Fixed a memory leak.

* PDF writer: Fixed a memory leak.

* PDF writer: Fixed a thread safety problem.

* PICT reader: Fix a thread safety problem.

* Exception reporting: Throwing an exception was not thread safe.  Now it is.

* Exception reporting: Handle the case where some passed character
  strings refer to existing exception character strings.

* Command-line parser now does not attempt to read a list of filenames
  from a file in '@name' syntax if the path '@name' exists.
  Previously it would attempt to read a list of file names from 'name'
  even if '@name' did exist.

* Rendering: Short-circuit path parsing and return and error
  immediately if an error occurs.

New Features:

* Added support for writing the Braille image format (by Samuel
  Thibault).

* WebP writer: Support WebP 'use_sharp_yuv' option ("if needed, use
  sharp (and slow) RGB->YUV conversion") via `-define
  webp:use-sharp-yuv=true`.

* The version command output now reports the OpenMP specification
  number rather than just the integer version identifier.

API Updates:

* ReallocateImageColormap() added to re-allocate an existing colormap.

* Some improperly-exposed globals are now static as they should have
  been.

Feature improvements:

* Microsoft Windows timing information now uses
  QueryPerformanceFrequency() and QueryPerformanceCounter() for
  increased precision.

* The 'benchmark' command now shows 6 digits (microseconds) of elapsed
  time indication.

* The 'time' command now shows 6 digits (microseconds) of elapsed time
  indication.

* The logging facility now shows 6 digits (microseconds) of time
  resolulution

* Dcraw: When QuantumDepth is greater than 8, pass -6 option to dcraw
  so that it returns a 16-bit/sample image.

* Dcraw: If Dcraw supports TIFF format, then request TIFF format in
  order to be able to acquire more metatdata.

* Scale algorithm: Eliminate artifacts when scaling an image with
  semi-transparent pixels.

* Library metrics: The number of shared library relocations and the
  amount of initialized data has been signficantly reduced by
  following recommendations from Ulrich Drepper's document `How To
  Write Shared Libraries <https://akkadia.org/drepper/dsohowto.pdf>`_.

  For comparison, these are the differences in library metrics between
  the 1.3.31 and 1.3.32 releases for a simple shared library with all
  features supported:

  +---------+-------------+------------------+------------+
  | Release | Relocations | Initialized Data | Total Size |
  +=========+=============+==================+============+
  | 1.3.31  |      12,432 |          506,496 |  3,587,227 |
  +---------+-------------+------------------+------------+
  | 1.3.32  |         747 |          127,936 |  3,033,279 |
  +---------+-------------+------------------+------------+

  and these are the differences in library metrics between 1.3.31 and
  1.3.32 for a shared library using the modules option (recommended!)
  with all features supported:

  +---------+-------------+------------------+------------+
  | Release | Relocations | Initialized Data | Total Size |
  +=========+=============+==================+============+
  | 1.3.31  |       5,370 |          176,784 |  1,940,620 |
  +---------+-------------+------------------+------------+
  | 1.3.32  |         367 |          119,472 |  1,825,651 |
  +---------+-------------+------------------+------------+

  As can be seen, the number of relocations was extreme and has been
  reduced to reasonable levels while also diminishing the amount of
  initialized data and the total size of the library/program.  Most of
  the remaining initialized data (106,648 bytes) and some of the
  relocations (65 relocations) may be attributed to the optional X11
  animate/display/import support.

Windows Delegate Updates/Additions:

* None

Build Changes:

* The test suite now passes even if no fonts are found.

* Configure script does better at finding Windows fonts on non-Windows systems.

* The configure script now supports the option --with-mtmalloc to
  enable use of the mtmalloc library as found on Solaris-derived
  systems.

Behavior Changes:

* AnnotateImage(): No longer implicitly call TranslateText() since
  this is not suitable for most use-cases and causes additional
  performance impact.  The API user can perform such translations in
  advance on the text string using TranslateText() if need be.


1.3.31 (November 17, 2018)
==========================

Special Issues:

* Firmware and operating system updates to address the Spectre
  vulnerability (and possibly to some extent the Meltdown
  vulnerability) have substantially penalized GraphicsMagick's OpenMP
  performance.  Performance is reduced even with GCC 7 and 8's
  improved optimizers. There does not appear to be anything we can do
  about this.

Security Fixes:

* GraphicsMagick is now participating in Google's oss-fuzz project due
  to the contributions and assistance of Alex Gaynor. Since February 4
  2018, 292 issues have been opened by oss-fuzz and 279 of those
  issues have been resolved.  The issues list is available at
  https://bugs.chromium.org/p/oss-fuzz/issues/list under search term
  "graphicsmagick".  Issues are available for anyone to view and
  duplicate if they have been in "Verified" status for 30 days, or if
  they have been in "New" status for 90 days.  There are too many
  fixes to list here.  Please consult the GraphicsMagick ChangeLog
  file, Mercurial repository commit log, and the oss-fuzz issues list
  for details.

Bug fixes:

* See above note about oss-fuzz fixes.

* CINEON: Fix unexpected hang on a crafted Cineon image.  SourceForge
  issue 571.

* Drawing recursion is limited to 100 and may be tuned via the
  MAX_DRAWIMAGE_RECURSION pre-processor definition.

* Fix reading MIFF files using legacy keyword 'color-profile' for ICC
  color profile as was used by ImageMagick 4.2.9.

* Fix reading/writing files when 'magick' is specified in lower case.
  This bug was a regression in 1.3.30.


New Features:

* TIFF: Support Zstd compression in TIFF.  This requires libtiff
  4.0.10 or later.

* TIFF: Support WebP compression in TIFF.  This requires libtiff
  4.0.10 or later.

API Updates:

* MagickMonitor() is marked as deprecated.  Code should not be using
  this function any more.

Feature improvements:

* The progress monitor callbacks (registered using MagickMonitor() or
  MagickMonitorFormatted()) are serialized via a common semaphore
  rather than via critical sections in OpenMP loops.  OpenMP loops are
  updated to use OpenMP 'atomic' and 'flush' to update shared loop
  variables rather than using a OpenMP 'critical' construct, reducing
  contention.  Performance on some targets is observed to have been
  improved by this change.

Windows Delegate Updates/Additions:

* None

Build Changes:

* There was already a 'compare' command installed with the
  '--enable-magick-compat' configure option was used but it did not
  function.  Now it functions.  There was no `compare` command in
  ImageMagick 5.5.2 and this compare command is only roughly similar
  to a `compare` command in some subsequent ImageMagick release.

* Removed Remove Ghostscript library support (--with-gslib) from
  configure script.  The 'HasGS' pre-processor defines which were
  enabled by this remain in the source code so it is still possible to
  use this library if absolutely necessary (e.g. CPPFLAGS=-DHasGS
  LIBS=-lgs).

* No longer explicitly link with the OpenMP library when it will be
  supplied already due to CFLAGS.

Behavior Changes:

* JPEG: Libjpeg-turbo is allowed 1/5th the memory resource limit
  provided for Graphicsmagick via the cinfo->mem->max_memory_to_use
  option, which is part of the IJG JPEG API/ABI, but usually not
  supported there.  This feature works for libjpeg-turbo 1.5.2 and
  later.  Limiting the memory usage is useful since libjpeg-turbo may
  otherwise consume arbitrary amounts of memory even before
  Graphicsmagick is informed of the image dimensions.

* JPEG: The maximum number of JPEG progressive scans is limited to 50.
  Otherwise some technically valid files could be read for almost
  forever.


1.3.30 (June 23, 2018)
=========================

Special Issues:

* None

Security Fixes:

* GraphicsMagick is now participating in Google's oss-fuzz project due
  to the contributions and assistance of Alex Gaynor. Since February 4
  2018, 238 issues have been opened by oss-fuzz and 230 of those
  issues have been resolved.  The issues list is available at
  https://bugs.chromium.org/p/oss-fuzz/issues/list under search term
  "graphicsmagick".  Issues are available for anyone to view and
  duplicate if they have been in "Verified" status for 30 days, or if
  they have been in "New" status for 90 days.  There are too many
  fixes to list here.  Please consult the GraphicsMagick ChangeLog
  file, Mercurial repository commit log, and the oss-fuzz issues list
  for details.

* SVG/Rendering: Fix heap write overflow of PrimitiveInfo and
  PointInfo arrays.  This is another manefestation of CVE-2016-2317,
  which should finally be fixed correctly due to active
  detection/correction of pending overflow rather than using
  estimation.

Bug fixes:

* Many oss-fuzz fixes are bug fixes.

* Drawing/Rendering: Many more fixes by Gregory J Wolfe (see the ChangeLog).

* MIFF: Detect end of file while reading image directory.

* SVG: Many more fixes by Gregory J Wolfe (see the ChangeLog).

* The AlphaCompositePixel macro was producing wrong results when the
  output alpha value was not 100% opaque. This is a regression
  introduced in 1.3.29.

* TILE: Fix problem with tiling JPEG images because the size request
  used by the TILE algorithm was also causing re-scaling in the JPEG
  reader.  The problem is solved by stripping the size request before
  reading the image.

New Features:

* None

API Updates:

* The size of PrimitiveInfo (believed to be an internal/private
  structure but in a header which is installed, has been increased to
  store a 'flags' argument. This is intended to be an internal
  interface but but may be detected as an ABI change.

Feature improvements:

* None

Windows Delegate Updates/Additions:

* None

Build Changes:

* The oss-fuzz build script (fuzzing/oss-fuzz-build.sh) now includes
  many delegate libraries such as zlib, libpng, libtiff, libjpeg, and
  freetype, resulting in more comprehensive testing.  The Q16 build is
  now being tested rather than the 'configure' default of Q8.

Behavior Changes:

* JPEG: The JPEG reader now allows 3 warnings of any particular type
  before giving up on reading and throwing an exception.  This choice
  was made after observing files which produce hundreds of warnings
  and consume massive amounts of memory before reading the image data
  has even started.  It is currently unknown how many files which were
  previously accepted will be rejected by default.  The number of
  allowed warnings may be adjusted using '-define
  jpeg:max-warnings=<value>'.  The default limit will be adjusted
  based on reported user experiences and may be adjusted prior to
  compilation via the MaxWarningCount definition in coders/jpeg.c.


1.3.29 (April 29, 2018)
=========================

Special Issues:

* None

Security Fixes:

* GraphicsMagick is now participating in Google's oss-fuzz project due
  to the contributions and assistance of Alex Gaynor. Since February 4
  2018, 180 issues have been opened by oss-fuzz and 173 of those
  issues have been resolved.  The issues list is available at
  https://bugs.chromium.org/p/oss-fuzz/issues/list under search term
  "graphicsmagick".  Issues are available for anyone to view and
  duplicate if they have been in "Verified" status for 30 days, or if
  they have been in "New" status for 90 days.  There are too many
  fixes to list here.  Please consult the GraphicsMagick ChangeLog
  file, Mercurial repository commit log, and the oss-fuzz issues list
  for details.

* JNG: Require that the embedded JPEG image have the same dimensions
  as the JNG image as provided by JHDR. Avoids a heap write overflow.

* MNG: Arbitrarily limit the number of loops which may be requested by
  the MNG LOOP chunk to 512 loops, and provide the '-define
  mng:maximum-loops=value' option in case the user wants to change the
  limit.  This fixes a denial of service caused by large LOOP
  specifications.

Bug fixes:

* Many oss-fuzz fixes are bug fixes.

* DICOM: Pre/post rescale functions are temporarily disabled (until
  the implementation is fixed).

* JPEG: Fix regression in last release in which reading some JPEG
  files produces the error "Improper call to JPEG library in state
  201".

* ICON: Some DIB-based Windows ICON files were reported as corrupt to
  an unexpectedly missing opacity mask image.

* In-memory Blob I/O: Don't implicitly increase the allocation size
  due to seek offsets.

* MNG: Detect and handle failure to allocate global PLTE. Fix divide
  by zero.

* DrawGetStrokeDashArray(): Check for failure to allocate memory.

* BlobToImage(): Now produces useful exception reports to cover the
  cases where 'magick' was not set and the file format could not be
  deduced from its header.

New Features:

* None

API Updates:

* Wand API: Added MagickIsPaletteImage(), MagickIsOpaqueImage(),
  MagickIsMonochromeImage(), MagickIsGrayImage(), MagickHasColormap()
  based on contributions by Troy Patteson.

* New structure ImageExtra added and Image 'clip_mask' member is
  replaced by 'extra' which points to private ImageExtra allocation.
  The ImageGetClipMask() function now provides access to the clip mask
  image.

* New structure DrawInfoExtra and DrawInfo 'clip_path' is replaced by
  'extra' which points to private DrawInfoExtra allocation.  The
  DrawInfoGetClipPath() function now provides access to the clip path.

* New core library functions: GetImageCompositeMask(),
  CompositeMaskImage(), CompositePathImage(), SetImageCompositeMask(),
  ImageGetClipMask(), ImageGetCompositeMask(), DrawInfoGetClipPath(),
  DrawInfoGetCompositePath()

* Deprecated core library functions: RegisterStaticModules(),
  UnregisterStaticModules().

Feature improvements:

* Static modules (in static library or shared library without
  dynamically loadable modules) are now lazy-loaded using the same
  external interface as the lazy-loader for dynamic modules.  This
  results in more similarity between the builds and reduces the fixed
  initialization overhead by only initializing the modules which are
  used.

* SVG: The quality of SVG support has been significantly improved due
  to the efforts of Greg Wolfe.

* FreeType/TTF rendering: Rendering fixes for opacity.

Windows Delegate Updates/Additions:

* None

Build Changes:

* None

Behavior Changes:

* None
