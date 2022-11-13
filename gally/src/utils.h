#pragma once

#ifdef _WIN32
static const char OSNAME[] = "windows";
#define access	_access
#define R_OK    4
#define W_OK    2
#define F_OK    0
#endif

#ifdef __unix__
static const char OSNAME[] = "linux";
#define _mkdir(d) mkdir(d, 0777)
#endif

#ifdef defined(__amd64__) 
static const char ARCHNAME[] = "x64";
#elif defined(_M_AMD64)
static const char ARCHNAME[] = "x64";
#elif  defined(_M_IX86)
static const char ARCHNAME[] = "x86";
#elif defined(i386)
static const char ARCHNAME[] = "i386";
#endif

#include "cjson/cJSON.h"

int		system_FileExist(char *path);
void	system_Error(int code, char *string);
int		system_Mkdir(char *dir);
int		system_Exec(char *command);
#ifdef __unix__
int		system_MakeExec(char* file);
#endif

int		http_Download(char *url, char *filename);

cJSON	*json_ParseFile(char *filename);

char *str_replace(char *orig, char *rep, char *with) ;
