#pragma once

#ifdef _WIN32
#define access	_access
#define R_OK    4
#define W_OK    2
#define F_OK    0
#endif

#ifdef __unix__
#define _mkdir(d) mkdir(d, 0777)
#endif

#include "cjson/cJSON.h"

int		system_FileExist(char *path);
int		system_MakeExec(char *file);
void	system_Error(int code, char *string);
int		system_Mkdir(char *dir);
int		system_Exec(char *command);

int		http_Download(char *url, char *filename);

cJSON	*json_ParseFile(char *filename);
