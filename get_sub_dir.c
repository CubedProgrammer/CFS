#ifdef _WIN32
#include<windows.h>
#else
#include<dirent.h>
#endif
#include<stdlib.h>
#include<string.h>
#include"get_sub_dir.h"

void cfs____sort_dir_entries(size_t sz, char *fnames[], enum cfs____file_or_directory fd[])
{
	size_t lstk[100], rstk[100];
	size_t ssz = 0;
	lstk[ssz] = 0;
	rstk[ssz] = sz - 1;
}

int cfs____case_insensitive_strcmp(const void *x, const void *y)
{
	const char *xstr = *(const char *const*)x, *ystr = *(const char *const*)y;
	size_t xlen = strlen(xstr), ylen = strlen(ystr);
	size_t len = xlen < ylen ? xlen : ylen;
	char u, v;
	int res = 0;
	for(size_t i = 0; i < len; ++i)
	{
		u = xstr[i], v = ystr[i];
		if(u >= 'A' && u <= 'Z')
			u+=0x20;
		if(v >= 'A' && v <= 'Z')
			v+=0x20;
		if(u == v)
			u = xstr[i], v = ystr[i];
		if(u < v)
			res = -1, len = 0;
		else if(u > v)
			res = 1, len = 0;
	}
	return res;
}
int cfs____cnt_sub_dirs(const char *dir)
{
	// make the wildcard search
	char search[5000];
	strcpy(search, dir);
#ifdef _WIN32
	size_t len=strlen(dir);
	strcpy(search + len, "\\*");
#endif
	// find data and start looking
#ifdef _WIN32
	WIN32_FIND_DATA wff;
	HANDLE ff = FindFirstFileA(search, &wff);
#else
	DIR *dr = opendir(search);
	struct dirent *de = readdir(dr);
#endif
	int cnt=0;

	// keep looking while it can find more
	do
	{
#ifdef _WIN32
		if(strcmp(".", wff.cFileName) && strcmp("..", wff.cFileName))
#else
		if(strcmp(".", de->d_name) && strcmp("..", de->d_name))
#endif
			++cnt;
#ifndef _WIN32
		de = readdir(dr);
#endif
	}
#ifdef _WIN32
	while(FindNextFileA(ff, &wff));
#else
	while(de);
#endif
	return cnt;
}
void cfs____get_sub_dirs(const char *dir,char *names[],enum cfs____file_or_directory fd[])
{
	// make the wildcard search
	char search[5000];
	strcpy(search, dir);
	size_t len=strlen(dir);
#ifdef _WIN32
	strcpy(search + len, "\\*");
#endif

	// find data and start looking
#ifdef _WIN32
	WIN32_FIND_DATA wff;
	HANDLE ff = FindFirstFileA(search, &wff);
#else
	DIR *dr = opendir(search);
	struct dirent *de = readdir(dr);
#endif
	size_t cnt=0, fnlen;

	// keep looking while it can find more
	do
	{
#ifdef _WIN32
		if(strcmp(".", wff.cFileName) == 0 || strcmp("..", wff.cFileName) == 0)
#else
		if(strcmp(".", de->d_name) == 0 || strcmp("..", de->d_name) == 0)
#endif
		{
#ifndef _WIN32
			de = readdir(dr);
#endif
			continue;
		}
#ifdef _WIN32
		fnlen = strlen(wff.cFileName);
#else
		fnlen = strlen(de->d_name);
#endif
		names[cnt]=malloc(len + fnlen + 2 * sizeof(char));
		strcpy(names[cnt], dir);
#ifdef _WIN32
		names[cnt][len] = '\\';
		strcpy(names[cnt] + len + 1, wff.cFileName);
#else
		names[cnt][len] = '/';
		strcpy(names[cnt] + len + 1, de->d_name);
#endif
#ifdef _WIN32
		if(wff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
#else
		if(de->d_type == DT_DIR)
#endif
			fd[cnt]=DIRECTORY;
		else
			fd[cnt]=NFILE;
		++cnt;
#ifdef __linux__
		de = readdir(dr);
#endif
	}
#ifdef _WIN32
	while(FindNextFileA(ff, &wff));
#else
	while(de);
	// implement merge sort here to sort
#endif
}
