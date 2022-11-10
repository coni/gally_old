#pragma once

#ifdef _WIN32
#include <cjson/cJSON.h>
#define access	_access
#define mkdir	_mkdir
#define R_OK    4
#define W_OK    2
#define F_OK    0
#endif


int		system_FileExist(char *path);
int		system_MakeExec(char *file);
void	system_Error(int code, char *string);
int		system_Mkdir(char *dir);
int		system_Exec(char *command);

int		http_Download(char *url, char *filename);

cJSON	*json_ParseFile(const char *filename);