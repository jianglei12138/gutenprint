/*
 *   Print plug-in CANON BJL driver for the GIMP.
 *
 *   Copyright 1997-2000 Michael Sweet (mike@easysw.com),
 *	Robert Krawitz (rlk@alum.mit.edu) and
 *      Andy Thaller (thaller@ph.tum.de)
 *   Copyright (c) 2006 - 2007 Sascha Sommer (saschasommer@freenet.de)
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *   for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* This file contains definitions for the various inks
*/

#ifndef GUTENPRINT_INTERNAL_CANON_INKS_H
#define GUTENPRINT_INTERNAL_CANON_INKS_H

/* ink definition: 
 *  ink dots can be printed in various sizes 
 *  one size is called level
 *  every level is represented by a bitcombination and a density
 *  the density ranges from 0 (no dot is printed) to 1.0 (maximum dot size)
 *
 *  an ink is therefore defined by the number of bits used for the bitpattern (bitdepth) and the number of possible levels:
 *    a 1 bit ink can have 2 possible levels 0 and 1
 *    a 2 bit ink can have 2*2=4 possible levels with the bitpatterns 0,1,2 and 3 
 *    a 3 bit ink can have 2*2*2=8 possible levels with the bitpatterns 0 to 7
 *    ...
 *  some inks use less levels than possible with the given bitdepth
 *  some inks use special compressions to store for example 5 3 level pixels in 1 byte  
 * naming:
 *  dotsizes are named dotsizes_xl where x is the number of levels (number of dotsizes + 1)
 *  inks are named canon_xb_yl_ink where x is the number of bits representing the y possible ink levels
 *  inks that contain special (compression) flags are named canon_xb_yl_c_ink
 * order:
 *  dotsizes are ordered ascending by the number of dots
 *
*/


typedef struct {
  const int bits;                     /* bitdepth */
  const int flags;                    /* flags:   */
#define INK_FLAG_5pixel_in_1byte 0x1  /*  use special compression where 5 3level pixels get stored in 1 byte */
#define INK_FLAG_3pixel5level_in_1byte 0x2  /*  use special compression where 3 5level pixels get stored in 1 byte */
#define INK_FLAG_3pixel6level_in_1byte 0x4  /*  use special compression where 3 6level pixels get stored in 1 byte */
  int numsizes;                       /* number of possible {bit,density} tuples */
  const stp_dotsize_t *dot_sizes;     /* pointer to an array of {bit,density} tuples */ 
} canon_ink_t;

/* declare a standard ink */
#define DECLARE_INK(bits,levels)      \
static const canon_ink_t canon_##bits##b_##levels##l_ink = {              \
  bits,0,                                  \
  sizeof(dotsizes_##levels##l)/sizeof(stp_dotsize_t), dotsizes_##levels##l   \
}

/* declare an ink with flags */
#define DECLARE_INK_EXTENDED(bits,levels,flags)      \
static const canon_ink_t canon_##bits##b_##levels##l_c_ink = {              \
  bits,flags,                                  \
  sizeof(dotsizes_##levels##l)/sizeof(stp_dotsize_t), dotsizes_##levels##l   \
}



/* NOTE  NOTE  NOTE  NOTE  NOTE  NOTE  NOTE  NOTE  NOTE  NOTE  NOTE  NOTE
 *
 * Some of the bitpattern/density combinations were taken from print-escp2.c 
 * and do NOT represent the requirements of canon inks. Feel free to play
 * with them and send a patch to gimp-print-devel@lists.sourceforge.net
 */


static const stp_dotsize_t dotsizes_2l[] = {
  { 0x1, 1.0 }
};

DECLARE_INK(1,2);

/* test */
DECLARE_INK(2,2);

static const stp_dotsize_t dotsizes_3l[] = {
  { 0x1, 0.5  },
  { 0x2, 1.0  }
};

DECLARE_INK(2,3);

/*under development*/
DECLARE_INK(4,3);

DECLARE_INK_EXTENDED(2,3,INK_FLAG_5pixel_in_1byte);

static const stp_dotsize_t dotsizes_4l[] = {
  { 0x1, 0.45 },
  { 0x2, 0.68 },
  { 0x3, 1.0 }
};

DECLARE_INK(2,4);

/*under development*/
DECLARE_INK(4,4);

static const stp_dotsize_t dotsizes_5l[] = {
  { 0x1, 0.45 },
  { 0x2, 0.55 },
  { 0x3, 0.68 },
  { 0x4, 1.0 }
};

/*under development*/
DECLARE_INK(4,5);
DECLARE_INK_EXTENDED(4,5,INK_FLAG_3pixel5level_in_1byte);

static const stp_dotsize_t dotsizes_6l[] = {
  { 0x1, 0.2 },
  { 0x2, 0.4 },
  { 0x3, 0.6 },
  { 0x4, 0.8 },
  { 0x5, 1.0 }
};

/*under development*/
DECLARE_INK(4,6);
DECLARE_INK_EXTENDED(4,6,INK_FLAG_3pixel6level_in_1byte);

static const stp_dotsize_t dotsizes_7l[] = {
  { 0x1, 0.45 },
  { 0x2, 0.55 },
  { 0x3, 0.66 },
  { 0x4, 0.77 },
  { 0x5, 0.88 },
  { 0x6, 1.0 }
};

DECLARE_INK(3,7);

/*under development*/
DECLARE_INK(4,7);

static const stp_dotsize_t dotsizes_8l[] = {
  { 0x1, 0.14 },
  { 0x2, 0.29 },
  { 0x3, 0.43 },
  { 0x4, 0.58 },
  { 0x5, 0.71 },
  { 0x6, 0.86 },
  { 0x7, 1.00 }
};

DECLARE_INK(4,8);

static const stp_dotsize_t dotsizes_9l[] = {
  { 0x1, 0.14 },
  { 0x2, 0.29 },
  { 0x3, 0.43 },
  { 0x4, 0.55 },
  { 0x5, 0.66 },
  { 0x6, 0.71 },
  { 0x7, 0.88 },
  { 0x8, 1.00 },
};

DECLARE_INK(4,9);
DECLARE_INK(8,9);

static const stp_dotsize_t dotsizes_14l[] = {
  { 0x1, 0.07 },
  { 0x2, 0.13 },
  { 0x3, 0.20 },
  { 0x4, 0.27 },
  { 0x5, 0.33 },
  { 0x6, 0.40 },
  { 0x7, 0.47 },
  { 0x8, 0.53 },
  { 0x9, 0.60 },
  { 0xA, 0.67 },
  { 0xB, 0.75 },
  { 0xC, 0.87 },
  { 0xD, 1.00 },
};

DECLARE_INK(4,14);

static const stp_dotsize_t dotsizes_16l[] = {
  { 0x1, 0.07 },
  { 0x2, 0.13 },
  { 0x3, 0.20 },
  { 0x4, 0.27 },
  { 0x5, 0.33 },
  { 0x6, 0.40 },
  { 0x7, 0.47 },
  { 0x8, 0.53 },
  { 0x9, 0.60 },
  { 0xA, 0.67 },
  { 0xB, 0.73 },
  { 0xC, 0.80 },
  { 0xD, 0.87 },
  { 0xE, 0.93 },
  { 0xF, 1.00 }
};

DECLARE_INK(4,16);
DECLARE_INK(8,16);


/* A inkset is a list of inks and their (relative) densities 
 * For printers that use the extended SetImage command t)
 * the inkset will be used to build the parameter list
 * therefore invalid inksets will let the printer fallback
 * to a default mode which will then lead to wrong output
 * use {0,0.0,NULL} for undefined placeholder inks
 * set density to 0.0 to disable certain inks
 * the paramters will then still occure in the t) command 
 * 
 * names:
 * inksets are named canon_X_ where X is the number of possible inks in the set
 * followed by YZ combinations for every defined ink where Y is the letter
 * representing the color and Z the maximum level of the color
 * if an inkset contains one or more compressed inks a _c is appended
 * the inkset name ends with _inkset
 * see the examples below
 * order:
 *  inksets are ordered by ascending number of possible inks, used inks, compression
 *
 */


typedef struct {
   const int channel;
   const double density;
   const canon_ink_t* ink;
} canon_inkset_t;


/* Inkset for printing in K and 1bit/pixel */
static const canon_inkset_t canon_1_K2_inkset[] = {
        {'K',1.0,&canon_1b_2l_ink}
};

/* Inkset for printing in CMY and 1bit/pixel */
static const canon_inkset_t canon_3_C2M2Y2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink}
};


/* Inkset for printing in CMY and 2bit/pixel */
static const canon_inkset_t canon_3_C4M4Y4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink}
};

static const canon_inkset_t canon_4_K2_inkset[] = {
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'K',1.0,&canon_1b_2l_ink},
};

/* Inkset for printing in CMYK and 1bit/pixel */
static const canon_inkset_t canon_4_C2M2Y2K2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink}
};

static const canon_inkset_t canon_4_C3M3Y3K3_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_2b_3l_c_ink},
        {'K',1.0,&canon_2b_3l_c_ink},
};

/* Inkset for printing in CMYK and 2bit/pixel */
static const canon_inkset_t canon_4_C4M4Y4K4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_2b_4l_ink}
};

/*
 * Dither ranges specifically for any Color and 3bit/pixel
 * (see NOTE above)
 *
 * BIG NOTE: The bjc8200 has this kind of ink. One Byte seems to hold
 *           drop sizes for 3 pixels in a 3/2/2 bit fashion.
 *           Size values for 3bit-sized pixels range from 1 to 7,
 *           size values for 2bit-sized picels from 1 to 3 (kill msb).
 *
 *
 */

/* Inkset for printing in CMYK and 3bit/pixel */
static const canon_inkset_t canon_4_C7M7Y7K7_inkset[] = {
        {'C',1.0,&canon_3b_7l_ink},
        {'M',1.0,&canon_3b_7l_ink},
        {'Y',1.0,&canon_3b_7l_ink},
        {'K',1.0,&canon_3b_7l_ink}
};

/* Inkset for printing in CMYKcm and 1bit/pixel */
/* FIXME is it really correct that the density of the CM inks is lowered? */
static const canon_inkset_t canon_6_C2M2Y2K2c2m2_inkset[] = {
        {'C',0.25,&canon_1b_2l_ink},
        {'M',0.26,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_1b_2l_ink},
        {'m',1.0,&canon_1b_2l_ink}
};

/* Inkset for printing in CMYKcm and 2bit/pixel */
/* FIXME is it really correct that the density of the CM inks is lowered? */
static const canon_inkset_t canon_6_C4M4Y4K4c4m4_inkset[] = {
        {'C',0.33,&canon_2b_4l_ink},
        {'M',0.33,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_2b_4l_ink},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink}
};

/* Inkset for printing in CMYKcm and 3bit/pixel */
/* FIXME is it really correct that the density of the CM inks is lowered? */
static const canon_inkset_t canon_6_C7M7Y7K7c7m7_inkset[] = {
        {'C',0.33,&canon_3b_7l_ink},
        {'M',0.33,&canon_3b_7l_ink},
        {'Y',1.0,&canon_3b_7l_ink},
        {'K',1.0,&canon_3b_7l_ink},
        {'c',1.0,&canon_3b_7l_ink},
        {'m',1.0,&canon_3b_7l_ink}
};

static const canon_inkset_t canon_7_K2C2M2Y2_inkset[] = {
        {'K',1.0,&canon_1b_2l_ink},
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,0},
        {0,0.0,0},
	{0,0.0,0},
};

static const canon_inkset_t canon_7_K2C3M3Y2_inkset[] = {
        {'K',1.0,&canon_1b_2l_ink},
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,0},
        {0,0.0,0},
	{0,0.0,0},
};

static const canon_inkset_t canon_7_K3C4M4Y3_inkset[] = {
        {'K',1.0,&canon_2b_3l_ink},
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {0,0.0,0},
        {0,0.0,0},
	{0,0.0,0},
};

static const canon_inkset_t canon_7_C4M4Y4_inkset[] = {
        {0,0.0,0},
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,0},
        {0,0.0,0},
	{0,0.0,0},
};

static const canon_inkset_t canon_7_C4M4Y4c4m4k4K4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {'k',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_2b_4l_ink},
};

static const canon_inkset_t canon_7_C14M14Y14c14m14y14_inkset[] = {
        {0,0.0,0},
        {'C',1.0,&canon_4b_14l_ink},
        {'M',1.0,&canon_4b_14l_ink},
        {'Y',1.0,&canon_4b_14l_ink},
        {'c',1.0,&canon_4b_14l_ink},
        {'m',1.0,&canon_4b_14l_ink},
        {'k',1.0,&canon_4b_14l_ink},
};

static const canon_inkset_t canon_9_K2_inkset[] = {
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_9_K5_c_inkset[] = {
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'K',1.0,&canon_4b_5l_c_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_9_K9_inkset[] = {
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'K',1.0,&canon_4b_9l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_9_C2M2Y2K2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C2M2Y2K2c2m2y2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_1b_2l_ink},
        {'m',1.0,&canon_1b_2l_ink},
        {'y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_c3m3y3_inkset[] = {
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,NULL},
	{'c',1.0,&canon_2b_3l_ink},
	{'m',1.0,&canon_2b_3l_ink},
	{'y',1.0,&canon_2b_3l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_9_C3M3Y2K2h_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_2b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C3M3Y2K2_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C3M3Y2K2_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C3M3Y3K3_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {'K',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C3M3Y2K2k3_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_3l_c_ink},  /* even though we won't use the photo black in this mode its parameters have to be set */
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C3M3Y2K2k3on_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C3M3Y2K2k3photo_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* switch off */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C3M3Y3K2c3m3_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_2b_3l_c_ink},
        {'K',0.0,&canon_1b_2l_ink},
        {'c',0.5,&canon_2b_3l_c_ink},
        {'m',0.5,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C3M3Y3K2c3m3k3_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_2b_3l_c_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* switch off */
        {'c',0.5,&canon_2b_3l_c_ink},
        {'m',0.5,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K2_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C4M4Y4K2k4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C4M4Y4K3_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C4M4Y4K2c4m4plain_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K2c4m4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink},
        {'c',0.5,&canon_2b_4l_ink},
        {'m',0.5,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K2c4m4k4p_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_4l_ink}, /* do not use but need to define in inkset */
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K2c4m4k4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink},
        {'c',0.5,&canon_2b_4l_ink},
        {'m',0.5,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K2c4m4k4on_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',0.5,&canon_2b_4l_ink},
        {'m',0.5,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K2c4m4y4twobit_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {'y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K4c4m4_inkset[] = {
        {'C',1.0,&canon_4b_4l_ink},
        {'M',1.0,&canon_4b_4l_ink},
        {'Y',1.0,&canon_4b_4l_ink},
        {'K',0.0,&canon_2b_4l_ink},
        {'c',1.0,&canon_4b_4l_ink},
        {'m',1.0,&canon_4b_4l_ink},
        {'y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4c4m4y4_inkset[] = {
        {'C',1.0,&canon_4b_4l_ink},
        {'M',1.0,&canon_4b_4l_ink},
        {'Y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {'c',0.5,&canon_4b_4l_ink},
        {'m',0.5,&canon_4b_4l_ink},
        {'y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K2c4m4y4plain_inkset[] = {
        {'C',1.0,&canon_4b_4l_ink},
        {'M',1.0,&canon_4b_4l_ink},
        {'Y',1.0,&canon_4b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_4b_4l_ink},
        {'m',1.0,&canon_4b_4l_ink},
        {'y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C4M4Y4K2c4m4y4photo_inkset[] = {
        {'C',1.0,&canon_4b_4l_ink},
        {'M',1.0,&canon_4b_4l_ink},
        {'Y',1.0,&canon_4b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink},/* Photo modes do not use K */
        {'c',0.5,&canon_4b_4l_ink},
        {'m',0.5,&canon_4b_4l_ink},
        {'y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C5M5Y5_inkset[] = {
        {'C',1.0,&canon_4b_5l_ink},
        {'M',1.0,&canon_4b_5l_ink},
        {'Y',1.0,&canon_4b_5l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C5M5Y5K2_inkset[] = {
        {'C',1.0,&canon_4b_5l_ink},
        {'M',1.0,&canon_4b_5l_ink},
        {'Y',1.0,&canon_4b_5l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* for CMY only, so no use */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C5M5Y5K2plain_inkset[] = {
        {'C',1.0,&canon_4b_5l_ink},
        {'M',1.0,&canon_4b_5l_ink},
        {'Y',1.0,&canon_4b_5l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C5M5Y5K5_c_inkset[] = {
        {'C',1.0,&canon_4b_5l_c_ink},
        {'M',1.0,&canon_4b_5l_c_ink},
        {'Y',1.0,&canon_4b_5l_c_ink},
        {'K',1.0,&canon_4b_5l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C5M5Y5K5c5m5_c_inkset[] = {
        {'C',1.0,&canon_4b_5l_c_ink},
        {'M',1.0,&canon_4b_5l_c_ink},
        {'Y',1.0,&canon_4b_5l_c_ink},
        {'K',1.0,&canon_4b_5l_c_ink},
        {'c',1.0,&canon_4b_5l_c_ink},
        {'m',1.0,&canon_4b_5l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C5M5Y5K5c9m9_c_inkset[] = {
        {'C',1.0,&canon_4b_5l_c_ink},
        {'M',1.0,&canon_4b_5l_c_ink},
        {'Y',1.0,&canon_4b_5l_c_ink},
        {'K',1.0,&canon_4b_5l_c_ink},
        {'c',1.0,&canon_4b_9l_ink},
        {'m',1.0,&canon_4b_9l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C5M5Y5K9c5m5_c_inkset[] = {
        {'C',1.0,&canon_4b_5l_c_ink},
        {'M',1.0,&canon_4b_5l_c_ink},
        {'Y',1.0,&canon_4b_5l_c_ink},
        {'K',1.0,&canon_4b_9l_ink},
        {'c',1.0,&canon_4b_5l_c_ink},
        {'m',1.0,&canon_4b_5l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C6M6Y6K6_c_inkset[] = {
        {'C',1.0,&canon_4b_6l_c_ink},
        {'M',1.0,&canon_4b_6l_c_ink},
        {'Y',1.0,&canon_4b_6l_c_ink},
        {'K',1.0,&canon_4b_6l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C6M6Y6K6c6m6_c_inkset[] = {
        {'C',1.0,&canon_4b_6l_c_ink},
        {'M',1.0,&canon_4b_6l_c_ink},
        {'Y',1.0,&canon_4b_6l_c_ink},
        {'K',1.0,&canon_4b_6l_c_ink},
        {'c',1.0,&canon_4b_6l_c_ink},
        {'m',1.0,&canon_4b_6l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C6M6Y6K9c6m6_c_inkset[] = {
        {'C',1.0,&canon_4b_6l_c_ink},
        {'M',1.0,&canon_4b_6l_c_ink},
        {'Y',1.0,&canon_4b_6l_c_ink},
        {'K',1.0,&canon_4b_9l_ink},
        {'c',1.0,&canon_4b_6l_c_ink},
        {'m',1.0,&canon_4b_6l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C6M6Y6K6c16m16_c_inkset[] = {
        {'C',1.0,&canon_4b_6l_c_ink},
        {'M',1.0,&canon_4b_6l_c_ink},
        {'Y',1.0,&canon_4b_6l_c_ink},
        {'K',1.0,&canon_4b_6l_c_ink},
        {'c',1.0,&canon_4b_16l_ink},
        {'m',1.0,&canon_4b_16l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C7M7Y7K2_inkset[] = {
        {'C',1.0,&canon_4b_7l_ink},
        {'M',1.0,&canon_4b_7l_ink},
        {'Y',1.0,&canon_4b_7l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_9_C8M8Y4K4c8m8_inkset[] = {
        {'C',1.0,&canon_4b_8l_ink},
        {'M',1.0,&canon_4b_8l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_2b_4l_ink},
        {'c',1.0,&canon_4b_8l_ink},
        {'m',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C8M8Y8c16m16_inkset[] = {
        {'C',1.0,&canon_4b_8l_ink},
        {'M',1.0,&canon_4b_8l_ink},
        {'Y',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {'c',0.5,&canon_8b_16l_ink},
        {'m',0.5,&canon_8b_16l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C8M8Y8c16m16k8_inkset[] = {
        {'C',1.0,&canon_4b_8l_ink},
        {'M',1.0,&canon_4b_8l_ink},
        {'Y',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {'c',0.5,&canon_4b_16l_ink},
        {'m',0.5,&canon_4b_16l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_4b_8l_ink}, 
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C9M9Y9K2_inkset[] = {
        {'C',1.0,&canon_4b_9l_ink},
        {'M',1.0,&canon_4b_9l_ink},
        {'Y',1.0,&canon_4b_9l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C9M9Y9K9c9m9_inkset[] = {
        {'C',1.0,&canon_4b_9l_ink},
        {'M',1.0,&canon_4b_9l_ink},
        {'Y',1.0,&canon_4b_9l_ink},
        {'K',1.0,&canon_4b_9l_ink},
        {'c',1.0,&canon_4b_9l_ink},
        {'m',1.0,&canon_4b_9l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C9M9Y9K2c9m9y9_inkset[] = {
        {'C',1.0,&canon_4b_9l_ink},
        {'M',1.0,&canon_4b_9l_ink},
        {'Y',1.0,&canon_4b_9l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* for PPpro, so no use */
        {'c',1.0,&canon_4b_9l_ink},
        {'m',1.0,&canon_4b_9l_ink},
        {'y',1.0,&canon_4b_9l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C9M9Y9K2c9m9y9plain8_inkset[] = {
        {'C',1.0,&canon_8b_9l_ink},
        {'M',1.0,&canon_8b_9l_ink},
        {'Y',1.0,&canon_8b_9l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_8b_9l_ink},
        {'m',1.0,&canon_8b_9l_ink},
        {'y',1.0,&canon_8b_9l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_C9M9Y9K2c9m9y9photo8_inkset[] = {
        {0,0.0,&canon_8b_9l_ink},
        {0,0.0,&canon_8b_9l_ink},
        {0,0.0,&canon_8b_9l_ink},
        {0,0.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_8b_9l_ink},
        {'m',1.0,&canon_8b_9l_ink},
        {'y',1.0,&canon_8b_9l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_9_c9m9y9_inkset[] = {
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,NULL},
	{'c',1.0,&canon_8b_9l_ink},
	{'m',1.0,&canon_8b_9l_ink},
	{'y',1.0,&canon_8b_9l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_11_K2_inkset[] = {
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C2M2Y2K2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C3M3Y2K2_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C4M4Y4K2_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C5M5Y3k4_inkset[] = {
        {'C',1.0,&canon_4b_5l_ink},
        {'M',1.0,&canon_4b_5l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C5M5Y4k4_inkset[] = {
        {'C',1.0,&canon_4b_5l_ink},
        {'M',1.0,&canon_4b_5l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C5M5Y5K5c5m5_c_inkset[] = {
        {'C',1.0,&canon_4b_5l_c_ink},
        {'M',1.0,&canon_4b_5l_c_ink},
        {'Y',1.0,&canon_4b_5l_c_ink},
        {'K',1.0,&canon_4b_5l_c_ink},
        {'c',1.0,&canon_4b_5l_c_ink},
        {'m',1.0,&canon_4b_5l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C6M6Y4K2_inkset[] = {
        {'C',1.0,&canon_4b_6l_ink},
        {'M',1.0,&canon_4b_6l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C6M6Y4k4_inkset[] = {
        {'C',1.0,&canon_4b_6l_ink},
        {'M',1.0,&canon_4b_6l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C6M6Y6K6_c_inkset[] = {
        {'C',1.0,&canon_4b_6l_c_ink},
        {'M',1.0,&canon_4b_6l_c_ink},
        {'Y',1.0,&canon_4b_6l_c_ink},
        {'K',1.0,&canon_4b_6l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C6M6Y6K6c6m6_c_inkset[] = {
        {'C',1.0,&canon_4b_6l_c_ink},
        {'M',1.0,&canon_4b_6l_c_ink},
        {'Y',1.0,&canon_4b_6l_c_ink},
        {'K',1.0,&canon_4b_6l_c_ink},
        {'c',1.0,&canon_4b_6l_c_ink},
        {'m',1.0,&canon_4b_6l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C6M6Y6K6c16m16_c_inkset[] = {
        {'C',1.0,&canon_4b_6l_c_ink},
        {'M',1.0,&canon_4b_6l_c_ink},
        {'Y',1.0,&canon_4b_6l_c_ink},
        {'K',1.0,&canon_4b_6l_c_ink},
        {'c',1.0,&canon_4b_16l_ink},
        {'m',1.0,&canon_4b_16l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C6M6Y6K9c6m6_c_inkset[] = {
        {'C',1.0,&canon_4b_6l_c_ink},
        {'M',1.0,&canon_4b_6l_c_ink},
        {'Y',1.0,&canon_4b_6l_c_ink},
        {'K',1.0,&canon_4b_9l_ink},
        {'c',1.0,&canon_4b_6l_c_ink},
        {'m',1.0,&canon_4b_6l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C9M9Y4k6_inkset[] = {
        {'C',1.0,&canon_4b_9l_ink},
        {'M',1.0,&canon_4b_9l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C16M16Y16k16_inkset[] = {
        {'C',1.0,&canon_4b_16l_ink},
        {'M',1.0,&canon_4b_16l_ink},
        {'Y',1.0,&canon_4b_16l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_4b_16l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_11_C16M16Y16K16c16m16_inkset[] = {
        {'C',1.0,&canon_4b_16l_ink},
        {'M',1.0,&canon_4b_16l_ink},
        {'Y',1.0,&canon_4b_16l_ink},
        {'K',1.0,&canon_4b_16l_ink},
        {'c',1.0,&canon_4b_16l_ink},
        {'m',1.0,&canon_4b_16l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}
};

static const canon_inkset_t canon_13_K2_inkset[] = {
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_K3_inkset[] = {
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'K',1.0,&canon_2b_3l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C2M2Y2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C2M2Y2K2_inkset[] = {
	{'C',1.0,&canon_1b_2l_ink},
	{'M',1.0,&canon_1b_2l_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C2M2Y2K2off_inkset[] = {
	{'C',1.0,&canon_1b_2l_ink},
	{'M',1.0,&canon_1b_2l_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{'K',0.0,&canon_1b_2l_ink}, /* declared in inkset but not used */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C2M2Y2k2_inkset[] = {
	{'C',1.0,&canon_1b_2l_ink},
	{'M',1.0,&canon_1b_2l_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C2M2Y2K2k2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_1b_2l_ink}, /* swap y and k */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2b_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_2b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K2_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K3_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K3off_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',0.0,&canon_2b_3l_ink}, /* declared in inkset but not used */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K2b_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_2b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K2y3_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
	{'k',0.0,&canon_2b_3l_c_ink}, /* swap y for k, switch off */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K2k3on_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink}, /* swap y for k */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K2k3photo_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* switched off */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink}, /* swap y for k */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K3k3_c_inkset[] = {
	{'C',1.0,&canon_2b_3l_c_ink},
	{'M',1.0,&canon_2b_3l_c_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{'K',1.0,&canon_2b_3l_c_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',0.0,&canon_2b_3l_c_ink},/* swapped y and k */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K3k3on_c_inkset[] = {
	{'C',1.0,&canon_2b_3l_c_ink},
	{'M',1.0,&canon_2b_3l_c_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{'K',1.0,&canon_2b_3l_c_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_3l_c_ink},/* swapped y and k */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y2K3k3photo_c_inkset[] = {
	{'C',1.0,&canon_2b_3l_c_ink},
	{'M',1.0,&canon_2b_3l_c_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{'K',0.0,&canon_2b_3l_c_ink},/* switched off */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_3l_c_ink},/* swapped y and k */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C3M3Y3k3c3m3_inkset[] = {
	{'C',1.0,&canon_2b_3l_ink},
	{'M',1.0,&canon_2b_3l_ink},
	{'Y',1.0,&canon_2b_3l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_3l_ink},
	{'c',1.0,&canon_2b_3l_ink},
	{'m',1.0,&canon_2b_3l_ink},
};

static const canon_inkset_t canon_13_C3M3Y3k4_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y3K3_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {'K',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y3K4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {'K',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4K2_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4K3k4off_c_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_4l_ink}, /* swapped y and k */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4K3k4_c_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink}, /* swapped y and k */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4k4_inkset[] = {
	{'C',1.0,&canon_2b_4l_ink},
	{'M',1.0,&canon_2b_4l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4k4c4m4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4K2c4m4y4on2_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* switch off */
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {'k',1.0,&canon_2b_4l_ink}, /* swap to k */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4K2c4m4y4_inkset[] = {
        {'C',1.0,&canon_4b_4l_ink},
        {'M',1.0,&canon_4b_4l_ink},
        {'Y',1.0,&canon_4b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_4b_4l_ink},
        {'m',1.0,&canon_4b_4l_ink},
        {'y',1.0,&canon_4b_4l_ink}, /* output uses y */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4K2c4m4k4_inkset[] = {
        {'C',1.0,&canon_4b_4l_ink},
        {'M',1.0,&canon_4b_4l_ink},
        {'Y',1.0,&canon_4b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* do not use for photo modes */
        {'c',1.0,&canon_4b_4l_ink},
        {'m',1.0,&canon_4b_4l_ink},
        {'k',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_c3m3y3_inkset[] = {
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,&canon_2b_3l_ink},
	{0,0.0,NULL},
	{'c',1.0,&canon_2b_3l_ink},
	{'m',1.0,&canon_2b_3l_ink},
	{'y',1.0,&canon_2b_3l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C4M4Y4c4m4y4_inkset[] = {
        {'C',1.0,&canon_4b_4l_ink},
        {'M',1.0,&canon_4b_4l_ink},
        {'Y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {'c',1.0,&canon_4b_4l_ink},
        {'m',1.0,&canon_4b_4l_ink},
        {'y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C5M5Y4k4_inkset[] = {
        {'C',1.0,&canon_4b_5l_ink},
        {'M',1.0,&canon_4b_5l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink}, /* swap y and k */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C5M5Y5_inkset[] = {
        {'C',1.0,&canon_4b_5l_ink},
        {'M',1.0,&canon_4b_5l_ink},
        {'Y',1.0,&canon_4b_5l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_13_C6M6Y2K2k4_inkset[] = {
	{'C',1.0,&canon_4b_6l_ink},
	{'M',1.0,&canon_4b_6l_ink},
	{'Y',1.0,&canon_2b_2l_ink},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink}, /* swap y and k */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C6M6Y4k4_inkset[] = {
	{'C',1.0,&canon_4b_6l_ink},
	{'M',1.0,&canon_4b_6l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink}, /* set y to k for photo modes */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C6M6Y4K2y4_inkset[] = {
	{'C',1.0,&canon_4b_6l_ink},
	{'M',1.0,&canon_4b_6l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',0.0,&canon_2b_4l_ink}, /* set y to k, switched off */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C6M6Y4K2k4on_inkset[] = {
	{'C',1.0,&canon_4b_6l_ink},
	{'M',1.0,&canon_4b_6l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C6M6Y4K3k4_c_inkset[] = {
	{'C',1.0,&canon_4b_6l_ink},
	{'M',1.0,&canon_4b_6l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{'K',1.0,&canon_2b_3l_c_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',0.0,&canon_2b_4l_ink},/* swapped y and k */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C6M6Y4K3k4on_c_inkset[] = {
	{'C',1.0,&canon_4b_6l_ink},
	{'M',1.0,&canon_4b_6l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{'K',1.0,&canon_2b_3l_c_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink},/* swapped y and k */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C8M8Y4k4_inkset[] = {
	{'C',1.0,&canon_4b_8l_ink},
	{'M',1.0,&canon_4b_8l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink}, /* set y to k for photo modes */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C8M8Y4K4k4_inkset[] = {
	{'C',1.0,&canon_4b_8l_ink},
	{'M',1.0,&canon_4b_8l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{'K',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_c9m9y9_inkset[] = {
	{0,0.0,&canon_8b_9l_ink},
	{0,0.0,&canon_8b_9l_ink},
	{0,0.0,&canon_8b_9l_ink},
	{0,0.0,NULL},
	{'c',1.0,&canon_8b_9l_ink},
	{'m',1.0,&canon_8b_9l_ink},
	{'y',1.0,&canon_8b_9l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_13_C9M9Y9k9c9m9_inkset[] = {
	{'C',1.0,&canon_8b_9l_ink},
	{'M',1.0,&canon_8b_9l_ink},
	{'Y',1.0,&canon_8b_9l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_8b_9l_ink},
	{'c',1.0,&canon_8b_9l_ink},
	{'m',1.0,&canon_8b_9l_ink},
};

static const canon_inkset_t canon_13_C14M14Y14c14m14y14_inkset[] = {
	{'C',1.0,&canon_4b_14l_ink},
	{'M',1.0,&canon_4b_14l_ink},
	{'Y',1.0,&canon_4b_14l_ink},
	{0,0.0,NULL},
	{'c',1.0,&canon_4b_14l_ink},
	{'m',1.0,&canon_4b_14l_ink},
	{'y',1.0,&canon_4b_14l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_16_C2M2Y2K2_inkset[] = {
	{'C',1.0,&canon_1b_2l_ink},
	{'M',1.0,&canon_1b_2l_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_16_C3M3Y2K2k3off_c_inkset[] = {
	{'C',1.0,&canon_2b_3l_c_ink},
	{'M',1.0,&canon_2b_3l_c_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',0.0,&canon_2b_3l_c_ink}, /* y and k swapped */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_16_C3M3Y2K2k3on_c_inkset[] = {
	{'C',1.0,&canon_2b_3l_c_ink},
	{'M',1.0,&canon_2b_3l_c_ink},
	{'Y',1.0,&canon_1b_2l_ink},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_3l_c_ink}, /* y and k swapped */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_16_C5M5Y4k4_inkset[] = {
	{'C',1.0,&canon_4b_5l_ink},
	{'M',1.0,&canon_4b_5l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink}, /* y and k swapped */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_16_C6M6Y4k4_inkset[] = {
	{'C',1.0,&canon_4b_6l_ink},
	{'M',1.0,&canon_4b_6l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink}, /* y and k swapped */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_16_C6M6Y4K2k4off_inkset[] = {
	{'C',1.0,&canon_4b_6l_ink},
	{'M',1.0,&canon_4b_6l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{'K',1.0,&canon_1b_2l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',0.0,&canon_2b_4l_ink}, /* y and k swapped */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_16_C8M8Y4k4_inkset[] = {
	{'C',1.0,&canon_4b_8l_ink},
	{'M',1.0,&canon_4b_8l_ink},
	{'Y',1.0,&canon_2b_4l_ink},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{'k',1.0,&canon_2b_4l_ink}, /* y and k swapped */
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
	{0,0.0,NULL},
};

static const canon_inkset_t canon_19_C2M2Y2K2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C2M2Y2k2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_1b_2l_ink},/* swap y and k */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C3M3Y3k3_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_ink}, /* swap y and k */
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C3M3Y3K2k3off_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_3l_ink},/* need to swap y -> k, switch off */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C3M3Y3K2k3_inkset[] = {
        {'C',1.0,&canon_2b_3l_ink},
        {'M',1.0,&canon_2b_3l_ink},
        {'Y',1.0,&canon_2b_3l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_ink},/* need to swap y -> k */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C4M4Y4k4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {'k',1.0,&canon_2b_4l_ink}, /* swap y and k */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C4M4Y4K2k4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* not used */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {'k',1.0,&canon_2b_4l_ink}, /* change y to k */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C4M4Y4K2k4off_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {'k',0.0,&canon_2b_4l_ink}, /* switch off */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C4M4Y4K2k4on_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {'k',1.0,&canon_2b_4l_ink}, /* change y to k */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C4M4Y4c4m4k4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {'k',1.0,&canon_2b_4l_ink}, /* change y to k  */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C4M4Y4c4m4k4CD_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {'k',1.0,&canon_2b_4l_ink}, /* swap y and k  */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C4M4Y4K2c4m4k4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* works when K is set to 0 */
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {'k',1.0,&canon_2b_4l_ink}, /* change y to k */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C6M6Y4K2_inkset[] = {
        {'C',1.0,&canon_4b_6l_ink},
        {'M',1.0,&canon_4b_6l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C6M6Y4c6m6k4_inkset[] = {
        {'C',1.0,&canon_4b_6l_ink},
        {'M',1.0,&canon_4b_6l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'c',0.0,&canon_4b_6l_ink}, /* not used */
        {'m',0.0,&canon_4b_6l_ink}, /* not used */
        {'k',1.0,&canon_2b_4l_ink}, /* swap y and k */ 
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C6M6Y4c6m6k4photo_inkset[] = {
        {'C',1.0,&canon_4b_6l_ink},
        {'M',1.0,&canon_4b_6l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'c',1.0,&canon_4b_6l_ink},
        {'m',1.0,&canon_4b_6l_ink},
        {'k',1.0,&canon_2b_4l_ink}, /* swap y and k */ 
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C6M6Y4K2c6m6k4hagaki_inkset[] = {
        {'C',1.0,&canon_4b_6l_ink},
        {'M',1.0,&canon_4b_6l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',0.0,&canon_4b_6l_ink}, /* will not use, so have to set to 0 */
        {'m',0.0,&canon_4b_6l_ink}, /* will not use, so have to set to 0 */
        {'k',1.0,&canon_2b_4l_ink}, /* change y to k */ 
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C6M6Y4K2c6m6k4_inkset[] = {
        {'C',1.0,&canon_4b_6l_ink},
        {'M',1.0,&canon_4b_6l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* will not use K */
        {'c',1.0,&canon_4b_6l_ink},
        {'m',1.0,&canon_4b_6l_ink},
        {'k',1.0,&canon_2b_4l_ink}, /* change y to k */ 
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C6M6Y4K2c6m6k4off_inkset[] = {
        {'C',1.0,&canon_4b_6l_ink},
        {'M',1.0,&canon_4b_6l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_4b_6l_ink},
        {'m',1.0,&canon_4b_6l_ink},
        {'k',0.0,&canon_2b_4l_ink}, /* change y to k, switch off */ 
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C7M7Y4c7m7k4_inkset[] = {
        {'C',1.0,&canon_4b_7l_ink},
        {'M',1.0,&canon_4b_7l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'c',1.0,&canon_4b_7l_ink},
        {'m',1.0,&canon_4b_7l_ink},
        {'k',1.0,&canon_2b_4l_ink}, /* swap y and k */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_19_C7M7Y4K2c7m7k4_inkset[] = {
        {'C',1.0,&canon_4b_7l_ink},
        {'M',1.0,&canon_4b_7l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* will not use K */
        {'c',1.0,&canon_4b_7l_ink},
        {'m',1.0,&canon_4b_7l_ink},
        {'k',1.0,&canon_2b_4l_ink}, /* change y to k */
        {0,0.0,NULL},
        {0,0.0,NULL}, 
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C2M2Y2K2_inkset[] = {
        {'C',1.0,&canon_1b_2l_ink},
        {'M',1.0,&canon_1b_2l_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C3M3Y2K2_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C3M3Y2K2photo_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* set to 0*/
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C3M3Y2K2k3_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_3l_c_ink}, /* off for plain modes */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C3M3Y2K2k3photo_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* set to 0 */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C3M3Y2K2k3on_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_1b_2l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C3M3Y3K2c3m3_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_2b_3l_c_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* set to 0 */
        {'c',1.0,&canon_2b_3l_c_ink},
        {'m',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C3M3Y3K2c3m3k3_c_inkset[] = {
        {'C',1.0,&canon_2b_3l_c_ink},
        {'M',1.0,&canon_2b_3l_c_ink},
        {'Y',1.0,&canon_2b_3l_c_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* set to 0 */
        {'c',1.0,&canon_2b_3l_c_ink},
        {'m',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
	{'k',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C4M4Y4K2_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C4M4Y4K2k4one_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* no K */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C4M4Y4K2k4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C4M4Y4K2c4m4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C4M4Y4K2c4m4photo_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',0.0,&canon_1b_2l_ink}, /* set to 0 */
        {'c',1.0,&canon_2b_4l_ink},
        {'m',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C4M4Y4K2c4m4k4_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',0.5,&canon_2b_4l_ink},
        {'m',0.5,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_4l_ink},  /* even though we won't use the photo black in this mode its parameters have to be set */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_22_C4M4Y4K2c4m4k4on_inkset[] = {
        {'C',1.0,&canon_2b_4l_ink},
        {'M',1.0,&canon_2b_4l_ink},
        {'Y',1.0,&canon_2b_4l_ink},
        {'K',1.0,&canon_1b_2l_ink},
        {'c',0.5,&canon_2b_4l_ink},
        {'m',0.5,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};


/* fast */
/* MP990, MG6100, MG6200, MG6900, MG7700, MG8100, MG8200 */
/* reorder: KCcMmYyk*H* not sure what the 2 missing ones are but they are only needed for ud1 anyway */
static const canon_inkset_t canon_30_K2C2M2Y2_inkset[] = {
        {'K',1.0,&canon_1b_2l_ink},
        {'C',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* standard */
/* MP990, MG6100, MG6200, MG8100, MG8200 */
/* reorder: KCcMmYyk*H* not sure what the 2 missing ones are but they are only needed for ud1 anyway */
static const canon_inkset_t canon_30_K2C3M3Y2k3_c_inkset[] = {
        {'K',1.0,&canon_1b_2l_ink},
        {'C',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_3l_c_ink}, /* will not use it, but need to specify it */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* K and k both */
static const canon_inkset_t canon_30_K2C3M3Y2k3on_c_inkset[] = {
        {'K',1.0,&canon_1b_2l_ink},
        {'C',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink}, /* on */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* high */
/* MP990, MG6100, MG6200, MG8100, MG8200 */
/* reorder: KCcMmYyk*H* not sure what the 2 missing ones are but they are only needed for ud1 anyway */
/*static const canon_inkset_t canon_30_C2M6K6m4k4_inkset[] = {*/
static const canon_inkset_t canon_30_K2C6M6Y4k4_inkset[] = {
        {'K',1.0,&canon_1b_2l_ink},
        {'C',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_4l_ink}, /* will not use it, but need to specify it */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* standard */
/* MG6900, MG7700 */
static const canon_inkset_t canon_30_K3C3M3Y2k3off2bit_inkset[] = {
        {'K',1.0,&canon_2b_3l_ink},
        {'C',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_2l_ink},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_3l_ink}, /* will not use it, but need to specify it */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* standard */
/* MG6300, MG6500, MG7100, iP8700 */
static const canon_inkset_t canon_30_K3C3M3Y2k3off_c_inkset[] = {
        {'K',1.0,&canon_2b_3l_c_ink},
        {'C',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_3l_c_ink}, /* will not use it, but need to specify it */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* standard duplex */
/* MG6900, MG7700 */
static const canon_inkset_t canon_30_K3C3M3Y2k3on2bit_inkset[] = {
        {'K',1.0,&canon_2b_3l_ink},
        {'C',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_2l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* standard for Duplex */
/* MG6300, MG6500, MG7100, iP8700 */
static const canon_inkset_t canon_30_K3C3M3Y2k3_c_inkset[] = {
        {'K',1.0,&canon_2b_3l_c_ink},
        {'C',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* standard for Borderless */
/* MG6500, MG7100 */
static const canon_inkset_t canon_30_K3C3M3Y2k3photo_c_inkset[] = {
        {'K',0.0,&canon_2b_3l_c_ink}, /* switched off */
        {'C',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_1b_2l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_3l_c_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* high */
/* MG6900, MG7700 */
static const canon_inkset_t canon_30_K3C6M6Y4k4off4bit_inkset[] = {
        {'K',1.0,&canon_4b_3l_ink},
        {'C',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {'k',0.0,&canon_4b_4l_ink}, /* will not use it, but need to specify it */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* high duplex */
/* MG6900, MG7700 */
static const canon_inkset_t canon_30_K3C6M6Y4k4on4bit_inkset[] = {
        {'K',1.0,&canon_4b_3l_ink},
        {'C',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* high */
/* MG6300, MG6500, MG7100, iP8700 */
static const canon_inkset_t canon_30_K3C6M6Y4k4off_c_inkset[] = {
        {'K',1.0,&canon_2b_3l_c_ink},
        {'C',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',0.0,&canon_2b_4l_ink}, /* will not use it, but need to specify it */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* CD photo */
/* reorder: KCcMmYyk*H* not sure what the 2 missing ones are but they are only needed for ud1 anyway */
/*static const canon_inkset_t canon_30_M5K5m4k4_inkset[] = {*/
static const canon_inkset_t canon_30_C5M5Y4k4_inkset[] = {
        {0,0.0,NULL},
        {'C',1.0,&canon_4b_5l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_5l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* MG6900, MG7700 */
/* CD, inkjet Hagaki */
static const canon_inkset_t canon_30_C5M5Y5k4_inkset[] = {
        {0,0.0,NULL},
        {'C',1.0,&canon_4b_5l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_5l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_4b_5l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_30_M6K6m4k4_inkset[] = {
        {0,0.0,NULL},
        {'C',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* photo standard */
/* MG6300, MG6500, MG7100 */
static const canon_inkset_t canon_30_M6K6m4k4H6off_inkset[] = {
        {0,0.0,NULL},
        {'C',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'H',0.0,&canon_4b_6l_ink},/* H is not supported, so only for modes where it is switched off */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* photo standard */
/* MG6900, MG7700 */
static const canon_inkset_t canon_30_M6K6m4k4H6off4bit_inkset[] = {
        {0,0.0,NULL},
        {'C',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_6l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {'H',0.0,&canon_4b_6l_ink},/* H is not supported, so only for modes where it is switched off */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

static const canon_inkset_t canon_30_M8K8m4k4_inkset[] = {
        {0,0.0,NULL},
        {'C',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* photo high */
/* MG6300, MG6500, MG7100 */
static const canon_inkset_t canon_30_M8K8m4k4H8off_inkset[] = {
        {0,0.0,NULL},
        {'C',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_2b_4l_ink},
        {0,0.0,NULL},
        {'H',0.0,&canon_4b_8l_ink},/* H is not supported, so only for modes where it is switched off */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

/* photo high */
/* MG6900, MG7700 */
static const canon_inkset_t canon_30_M8K8m4k4H8off4bit_inkset[] = {
        {0,0.0,NULL},
        {'C',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {'M',1.0,&canon_4b_8l_ink},
        {0,0.0,NULL},
        {'Y',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {'k',1.0,&canon_4b_4l_ink},
        {0,0.0,NULL},
        {'H',0.0,&canon_4b_8l_ink},/* H is not supported, so only for modes where it is switched off */
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
        {0,0.0,NULL},
};

#endif

