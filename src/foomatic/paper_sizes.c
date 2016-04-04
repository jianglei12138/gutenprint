/*
 * "$Id: paper_sizes.c,v 1.10 2015/09/07 21:57:37 speachy Exp $"
 *
 *   Dump the per-printer options for the OpenPrinting database
 *
 *   Copyright 2000 Robert Krawitz (rlk@alum.mit.edu)
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <gutenprint/gutenprint.h>

int
main(int argc, char **argv)
{
  int i;

  stp_init();
  for (i = 0; i < stp_known_papersizes(); i++)
    {
      const stp_papersize_t *p = stp_get_papersize_by_index(i);
      if (p->paper_size_type == PAPERSIZE_TYPE_ENVELOPE ||
	  p->paper_size_type == PAPERSIZE_TYPE_STANDARD)
        {
	  printf("%s %d %d\n", p->name, p->width, p->height);
	}
    }
  return 0;
}
