#include <stdio.h>
#include <string.h>
#include "config.h"

/**
 * crcsync - routines to use crc for an rsync-like protocol.
 *
 * This is a complete library for synchronization using a variant of the
 * rsync protocol.
 *
 * Example:
 *	// Calculate checksums of file (3-arg mode)
 *	// Or print differences between file and checksums (4+ arg mode)
 *	#include <ccan/crcsync/crcsync.h>
 *	#include <ccan/grab_file/grab_file.h>
 *	#include <stdio.h>
 *	#include <stdlib.h>
 *	#include <err.h>
 *	
 *	static void print_result(long result)
 *	{
 *		if (result < 0)
 *			printf("MATCHED CRC %lu\n", -result - 1);
 *		else if (result > 0)
 *			printf("%lu literal bytes\n", result);
 *	}
 *	
 *	int main(int argc, char *argv[])
 *	{
 *		size_t len, used, blocksize;
 *		char *file;
 *		struct crc_context *ctx;
 *		uint32_t *crcs;
 *		long res, i;
 *	
 *		if (argc < 3 || (blocksize = atoi(argv[1])) == 0)
 *			errx(1, "Usage: %s <blocksize> <file> <crc>...\n"
 *			     "OR: %s <blocksize> <file>", argv[0], argv[0]);
 *	
 *		file = grab_file(NULL, argv[2], &len);
 *		if (!file)
 *			err(1, "Opening file %s", argv[2]);
 *	
 *		if (argc == 3) {
 *			// Short form prints CRCs of file for use in long form.
 *			used = (len + blocksize - 1) / blocksize;
 *			crcs = malloc(used * sizeof(uint32_t));
 *			crc_of_blocks(file, len, blocksize, 32, crcs);
 *			for (i = 0; i < used; i++)
 *				printf("%i ", crcs[i]);
 *			printf("\n");
 *			return 0;
 *		}
 *	
 *		crcs = malloc((argc - 3) * sizeof(uint32_t));
 *		for (i = 0; i < argc-3; i++)
 *			crcs[i] = atoi(argv[3+i]);
 *	
 *		ctx = crc_context_new(blocksize, 32, crcs, argc-3);
 *		for (used = 0; used < len; ) {
 *			used += crc_read_block(ctx, &res, file+used, len-used);
 *			print_result(res);
 *		}
 *		while ((res = crc_read_flush(ctx)) != 0)
 *			print_result(res);
 *	
 *		return 0;
 *	}
 *
 * Licence: LGPL (v2 or any later version)
 */
int main(int argc, char *argv[])
{
	if (argc != 2)
		return 1;

	if (strcmp(argv[1], "depends") == 0) {
		printf("ccan/crc\n");
		return 0;
	}

	return 1;
}
