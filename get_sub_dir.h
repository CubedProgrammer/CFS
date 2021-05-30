#ifndef GET_SUB_DIR_H_
#define GET_SUB_DIR_H_
enum cfs____file_or_directory
{NFILE = 997, DIRECTORY};
int cfs____case_insensitive_strcmp(const char *xstr, const char *ystr);
int cfs____cnt_sub_dirs(const char *dir);
void cfs____get_sub_dirs(const char *dir,char *names[],enum cfs____file_or_directory *fd);
#endif
