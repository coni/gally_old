#pragma once

#include "../cjson/cJSON.h"

char*	mc_GetInherit(cJSON* manifest);
cJSON*	mc_GetMainManifest(char* path);
cJSON*	mc_GetManifest(cJSON* versionManifest, char* version, char* path);
