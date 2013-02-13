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


#include <sys/kmem.h>
#include <sys/lzo.h>
#include <spl-debug.h>

#ifdef DEBUG_SUBSYSTEM
#undef DEBUG_SUBSYSTEM
#endif

#define DEBUG_SUBSYSTEM SS_LZO

static spl_kmem_cache_t *lzo_workmem_cache;

/*
 * A kmem_cache is used for the lzo workmem to avoid having to vmalloc
 * and vfree for every call. See comment in spl_zlib.c for more details.
 */
static void *
lzo_workmem_alloc(int flags)
{
	return kmem_cache_alloc(lzo_workmem_cache, flags & ~(__GFP_FS));
}

static void
lzo_workmem_free(void *workmem)
{
	kmem_cache_free(lzo_workmem_cache, workmem);
}

/*
 * Wrapper for in-kernel LZO compressor
 */
int
spl_lzo_compress(void *dst, size_t *dstlen, const void *src, size_t srclen)
{
	void	*workmem;
	int	err;

	workmem = lzo_workmem_alloc(KM_SLEEP);
	err = lzo1x_1_compress(src, srclen, dst, dstlen, workmem);
	lzo_workmem_free(workmem);

	return (err);
}
EXPORT_SYMBOL(spl_lzo_compress);

int
spl_lzo_decompress(void *dst, size_t *dstlen, const void *src, size_t srclen)
{
	return (lzo1x_decompress_safe(src, srclen, dst, dstlen));
}
EXPORT_SYMBOL(spl_lzo_decompress);

int
spl_lzo_init(void)
{
	SENTRY;

	if (lzo_workmem_cache != NULL)
		SRETURN(1);

	lzo_workmem_cache = kmem_cache_create( "spl_lzo_workmem_cache",
	    LZO1X_1_MEM_COMPRESS, 0, NULL, NULL, NULL, NULL, NULL,
	    KMC_VMEM | KMC_NOEMERGENCY);
	if (lzo_workmem_cache == NULL)
		SRETURN(1);

	SRETURN(0);
}
EXPORT_SYMBOL(spl_lzo_init);

void
spl_lzo_fini(void)
{
	SENTRY;

	if (lzo_workmem_cache != NULL) {
		kmem_cache_destroy(lzo_workmem_cache);
		lzo_workmem_cache = NULL;
	}

	SEXIT;
}
EXPORT_SYMBOL(spl_lzo_fini);
