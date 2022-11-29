#pragma once

#include "../cjson/cJSON.h"

char* mc_DownloadClient(cJSON* manifest, char* versionPath, char* version);
char* mc_GetType(cJSON* manifest);