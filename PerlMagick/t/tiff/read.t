#!/usr/local/bin/perl
#
# Test reading TIFF images
#
# Contributed by Bob Friesenhahn <bfriesen@simple.dallas.tx.us>
#
BEGIN { $| = 1; $test=1; print "1..3\n"; }
END {print "not ok $test\n" unless $loaded;}

use Image::Magick;
$loaded=1;

require 't/subroutines.pl';

chdir 't/tiff' || die 'Cd failed';

#
# 1) Test Reading Monochrome
# 
print("Monochrome ...\n");
testRead ( 'input_mono.tiff', '8023e6a23a15ab1240b48ec6c085ed40',
                              'caff31e82385063ac81515b4bd2c5985' );

#
# 2) Test reading PseudoColor
#
++$test;
print("PseudoColor ...\n");
testRead( 'input_256.tiff', 'f90f998ebc41c7c9833ea2a454791d70',
                            '2be64c10552ee126ad6eaa2c1439fc0a' );

#
# 3) Test Reading TrueColor
# 
++$test;
print("TrueColor image ...\n");
testRead( 'input_truecolor.tiff', '4d3c3e472ace236f15b27c7af6e3d464',
                                  '716850136a25ff5ec5735ac9f26faa2f' );

