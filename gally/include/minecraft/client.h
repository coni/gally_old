#pragma once

#include "cjson/cJSON.h"
#include "launcher.h"

char* mc_DownloadClient(cJSON* manifest, GamePath gamePath, char* version);
char* mc_GetType(cJSON* manifest);
int mc_GetClientSize(cJSON* manifest);
int mc_GetClientSizeVersion(char* version, GamePath gamePath);
