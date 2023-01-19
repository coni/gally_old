#include "cjson/cJSON.h"
#include "curl/curl.h"
#include "launcher.h"

int compareLwjglVersion(char* new, char* old);
char** mc_DownloadLibraries(cJSON *manifest, GamePath gamePath);
char* mc_GetLwjglVersion(cJSON *manifest);
int mc_GetLibrariesSize(cJSON* manifest);
int mc_GetLibrariesSizeVersion(char* version, GamePath gamePath);
