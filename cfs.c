#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include"get_sub_dir.h"

size_t single_file_size(const char *name)
{
#ifdef _WIN32
#else
	struct stat dat;
	stat(name, &dat);
	return dat.st_size;
#endif
}

static inline int has_file_extension(const char *fname, const char *ext)
{
	if(ext == NULL)
		return 1;
	else
	{
		size_t flen = strlen(fname), elen = strlen(ext);
		int status = 0;
		if(flen > elen)
		{
			if(fname[flen - elen - 1] == '.' && strcmp(ext, fname + flen - elen) == 0)
				status = 1;
		}
		return status;
	}
}

size_t count_file_sizes(const char *maindir, size_t extcnt, const char *const exts[])
{
	size_t tot = 0;
	size_t capa = 5, ocapa = 3;
	size_t len = 0;
	char **stack = malloc(capa * sizeof(char*));
	int cnt = 0;
	char **cont;
	enum file_or_directory *fds;
	char *maindircpy = malloc(strlen(maindir) + 1);
	strcpy(maindircpy, maindir);
	stack[len] = maindircpy;
	++len;
	char *curr;
	while(len)
	{
		curr = stack[--len];
		cnt = cfs____cnt_sub_dirs(curr);
		cont = malloc(cnt * sizeof(char*));
		fds = malloc(cnt * sizeof(char*));
		cfs____get_sub_dirs(curr, cont, fds);
		for(int i = 0; i < cnt; ++i)
		{
			puts(cont[i]);
			fflush(stdout);
			if(fds[i] == DIRECTORY)
			{
				if(len == capa)
				{
					capa += ocapa;
					stack = realloc(stack, sizeof(char*) * capa);
					ocapa = len;
				}
				stack[len] = cont[i];
				++len;
			}
			else
			{
#ifdef _WIN32
				WIN32_FIND_DATA dat;
				FindFirstFileA(cont[i], &dat);
#else
				struct stat dat;
				stat(cont[i], &dat);
				free(cont[i]);
				tot += dat.st_size;
#endif
			}
		}
		free(curr);
		free(cont);
		free(fds);
	}
	return tot;
}

int main(int argl, char *argv[])
{
	if(argl == 1)
		puts("Put the directory to count, and then all the extensions to count.");
	else
		printf("All files in %s are %lu combined.\n", argv[1], count_file_sizes(argv[1], argl - 2, argv + 2));
	return 0;
}
