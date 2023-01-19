#pragma once

#ifdef _WIN32
static const char CLASSSEPARATOR = ';';
#define access	_access
#define R_OK    4
#define W_OK    2
#define F_OK    0
#endif

#ifdef __unix__
static const char CLASSSEPARATOR = ':';
#define _mkdir(d) mkdir(d, 0777)
#endif

extern char* OSNAME;
extern char* ARCHNAME;

extern int DOWNLOAD_CUR;
extern int DOWNLOAD_TOTAL;
extern int DOWNLOAD_TEST;

#include "cjson/cJSON.h"

int		system_FileExist(char *path);
void	system_Error(int code, char *string);
int		system_Mkdir(char *dir);
int		system_Exec(char *command);
#ifdef __unix__
int		system_MakeExec(char* file);
#endif

int		http_Download(char *url, char *filename);
char* http_get(char* url);

cJSON	*json_ParseFile(char *filename);

char *str_replace(char *orig, char *rep, char *with);
char* str_split(char* text, char chr, int index);
char* str_cpyrange(char* src, int start, int size);
