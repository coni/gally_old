#include "cjson/cJSON.h"
#include <curl/curl.h>
#include "launcher.h"

int compareLwjglVersion(char* new, char* old);
char** mc_DownloadLibraries(cJSON *manifest, GamePath gamePath);
char* mc_GetLwjglVersion(cJSON *manifest);
