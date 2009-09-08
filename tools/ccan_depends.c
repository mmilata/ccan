#include "tools.h"
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <ccan/str/str.h>
#include <ccan/talloc/talloc.h>

int main(int argc, char *argv[])
{
	char **deps;
	unsigned int i;
	bool compile = false;
	bool recurse = true;
	bool ccan = true;
	char *dirname, *basename;

	if (argv[1] && streq(argv[1], "--direct")) {
		argv++;
		argc--;
		recurse = false;
	}
	if (argv[1] && streq(argv[1], "--compile")) {
		argv++;
		argc--;
		compile = true;
	}
	if (argv[1] && streq(argv[1], "--non-ccan")) {
		argv++;
		argc--;
		ccan = false;
	}
	if (argc != 2)
		errx(1, "Usage: ccan_depends [--direct] [--compile] [--non-ccan] <dir>\n"
		        "Spits out all the ccan dependencies (recursively unless --direct)");

	/* We find depends without compiling by looking for ccan/ */
	if (!ccan && !compile)
		errx(1, "--non-ccan needs --compile");

	dirname = talloc_dirname(NULL, argv[1]);
	basename = talloc_basename(NULL, argv[1]);

	if (compile)
		deps = get_deps(talloc_autofree_context(),
				dirname, basename, recurse);
	else
		deps = get_safe_ccan_deps(talloc_autofree_context(),
					  dirname, basename, recurse);

	for (i = 0; deps[i]; i++)
		if (strstarts(deps[i], "ccan/") == ccan)
			printf("%s\n", deps[i]);
	return 0;
}
