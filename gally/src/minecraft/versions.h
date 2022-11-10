#pragma once

#ifdef _WIN32
#include <cjson/cJSON.h>
#endif

#ifdef _WIN32
#include "../cjson/cJSON.h"
#endif

char	*mc_GetInherit(cJSON *manifest);
cJSON	*mc_GetMainManifest(char *path);
