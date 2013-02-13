/*
 *  Copyright (c) 2013 Cyril Plisko.
 *
 *  This file is part of the SPL, Solaris Porting Layer.
 *  For details, see <http://github.com/behlendorf/spl/>.
 *
 *  The SPL is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  The SPL is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the SPL.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SPL_LZO_H
#define _SPL_LZO_H

#include <sys/types.h>
#include <linux/lzo.h>

extern int spl_lzo_compress(void *dst, size_t *dstlen, const void *src,
    size_t srclen);
extern int spl_lzo_decompress(void *dst, size_t *dstlen, const void *src,
    size_t srclen);

extern int spl_lzo_init(void);
extern void spl_lzo_fini(void);

#endif /* SPL_LZO_H */
