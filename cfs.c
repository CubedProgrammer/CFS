#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifdef _WIN32
#include<windows.h>
#else
#include<sys/stat.h>
#endif
#include"get_sub_dir.h"

static inline int cfs____has_file_extension(const char *fname, const char *ext)
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
	enum cfs____file_or_directory *fds;
	char *maindircpy = malloc(strlen(maindir) + 1);
	strcpy(maindircpy, maindir);
	stack[len] = maindircpy;
	++len;
	char *curr, *tmp;
	int valid;
	while(len)
	{
		curr = stack[--len];
		cnt = cfs____cnt_sub_dirs(curr);
		cont = malloc(cnt * sizeof(char*));
		fds = malloc(cnt * sizeof(char*));
		cfs____get_sub_dirs(curr, cont, fds);
		for(int i = 0; i < cnt; ++i)
		{
			if(fds[i] == DIRECTORY)
			{
				if(len == capa)
				{
					capa += ocapa;
					stack = realloc(stack, sizeof(char*) * capa);
					ocapa = len;
				}
				stack[len] = malloc(strlen(curr) + strlen(cont[i]) + 2);
				strcpy(stack[len], curr);
#ifndef _WIN32
				stack[len][strlen(curr)] = '/';
#else
				stack[len][strlen(curr)] = '\\';
#endif
				strcpy(stack[len] + strlen(curr) + 1, cont[i]);
				++len;
			}
			else
			{
				valid = extcnt == 0 ? 1 : 0;
				for(size_t j = 0; j < extcnt; ++j)
				{
					if(cfs____has_file_extension(cont[i], exts[j]))
					{
						valid = 1;
						j = 999;
					}
				}
				if(valid == 0)
					continue;
				tmp = malloc(strlen(curr) + strlen(cont[i]) + 2);
				strcpy(tmp, curr);
#ifndef _WIN32
				tmp[strlen(curr)] = '/';
#else
				tmp[strlen(curr)] = '\\';
#endif
				strcpy(tmp + strlen(curr) + 1, cont[i]);
#ifdef _WIN32
				WIN32_FIND_DATA dat;
				FindFirstFileA(cont[i], &dat);
				ULONGLONG sz = dat.nFileSizeHigh;
				sz = (sz << 32) + dat.nFileSizeLow;
				tot += sz;
#else
				struct stat dat;
				stat(tmp, &dat);
				tot += dat.st_size;
#endif
				free(tmp);
			}
			free(cont[i]);
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
		printf("All files in %s are %lu bytes combined.\n", argv[1], count_file_sizes(argv[1], argl - 2, argv + 2));
	return 0;
}
