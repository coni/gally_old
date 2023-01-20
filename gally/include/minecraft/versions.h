#pragma once

#include "cjson/cJSON.h"
#include "launcher.h"

char*	mc_GetInherit(cJSON* manifest);
cJSON*	mc_GetMainManifest(GamePath gamePath);
cJSON*	mc_GetManifest(cJSON* versionManifest, GamePath gamePath, char* version);
int mc_DoesVersionExist(char* version, GamePath gamePath);
